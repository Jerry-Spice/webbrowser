// These two definitions are required for windows builds for whatever reason...
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A01  // Target Windows 10 / 11 (or 0x0601 for Windows 7)
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Windows headers are strict and must be included first
#include "Sockets.hpp"
#include <iostream>

// ---------- WINDOWS SOCKETS ----------

// Since I'm writing this initially to run on Windows 10 that means that I need to use WinSock...
// https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock

// Make sure to link Ws2_32.lib when compiling
int initialize_winsock() {
    WSADATA wsaData;
    int iResult;

    iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (iResult != 0) {
       return iResult;
    }
    return 0;
}

SOCKET connect_to_url(std::string url, std::string port) {
    struct addrinfo* result = NULL;
    struct addrinfo* ptr = NULL;
    struct addrinfo hints;

    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

    if (url.rfind("http://", 0) == 0) url.erase(0, 7);
    if (url.rfind("https://", 0) == 0) url.erase(0, 8);

    int iResult = getaddrinfo(url.c_str(), port.c_str(), &hints, &result);

    SOCKET connected_socket = INVALID_SOCKET;
    ptr = result;
    if (ptr == NULL) {
        std::cout << "Invalid url: '" << url << ":" << port << "'" << std::endl;
        return INVALID_SOCKET;
    }
    connected_socket = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
    if (connected_socket == INVALID_SOCKET) {
        WSACleanup();
        freeaddrinfo(result);
        return INVALID_SOCKET;
    }

    iResult = connect(connected_socket, ptr->ai_addr, (int)ptr->ai_addrlen);
    if (iResult == SOCKET_ERROR) {
        closesocket(connected_socket);
        WSACleanup();
        freeaddrinfo(result);
        return INVALID_SOCKET;
    }

    freeaddrinfo(result);

    if (connected_socket == INVALID_SOCKET) {
        WSACleanup();
        return INVALID_SOCKET;
    }
    return connected_socket;
}

int send_buffer(SOCKET socket, std::string buffer) {
    int iResult = send(socket, buffer.c_str(), buffer.length(), 0);
    if (iResult == SOCKET_ERROR) {
        closesocket(socket);
        WSACleanup();
    }
    return iResult;
}

int recieve_buffer(SOCKET socket, std::string* buffer) {
    char temp_buffer[4096]; // 1 Page of memory
    int iResult;
    do {
        iResult = recv(socket, temp_buffer, 4095, 0);
        temp_buffer[iResult + 1] = '\0';
        *buffer += temp_buffer;
    } while (iResult > 0);
    return buffer->length();
}

int close_socket(SOCKET socket) {
    int iResult = shutdown(socket, SD_SEND);
    if (iResult == SOCKET_ERROR) {
        closesocket(socket);
        WSACleanup();
        return 1;
    }
    closesocket(socket);
    WSACleanup();
    return 0;
}

// ---------- OPENSSL ----------

// Now I'm including OpenSSL headers so that I can interact with HTTPS servers
SSL_CTX* initialize_openssl() {
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    const SSL_METHOD* method = TLS_client_method();
    SSL_CTX* ctx = SSL_CTX_new(method);

    return ctx;
}

SSL* perform_handshake(SOCKET socket, SSL_CTX* ctx, std::string host) {
    SSL* ssl = SSL_new(ctx);

    SSL_set_fd(ssl, socket);

    SSL_set_tlsext_host_name(ssl, host.c_str());

    int status = SSL_connect(ssl);

    return ssl;
}

int secure_send(SSL* ssl, std::string content) {
    return SSL_write(ssl, content.c_str(), content.length());
}

int secure_recv(SSL* ssl, std::string* buffer) {
    char temp_buffer[4096]; // 1 Page of memory
    int b = 0;
    do {
        b = SSL_read(ssl, temp_buffer, 4095);
        temp_buffer[b + 1] = '\0';
        *buffer += temp_buffer;
    } while (b > 0);
    return buffer->length();
}

void cleanup_openssl(SSL* ssl, SSL_CTX* ctx) {
    SSL_shutdown(ssl);

    SSL_free(ssl);

    SSL_CTX_free(ctx);
}
