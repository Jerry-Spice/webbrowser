#include <iostream>
#include <string>
#include <vector>
#include "HTTP.hpp"
#include "Request.hpp"

// Since I'm writing this initially to run on Windows 10 that means that I need to use WinSock...
// https://learn.microsoft.com/en-us/windows/win32/winsock/getting-started-with-winsock

#include <winsock2.h>
#include <ws2tcpip.h>
// Make sure to link Ws2_32.lib when compiling
boolean initialized_winsock = false;
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

    int iResult = getaddrinfo(url.c_str(), port.c_str(), &hints, &result);

    SOCKET connected_socket = INVALID_SOCKET;
    ptr = result;
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
        iResult = recv(socket, temp_buffer, 4096, 0);
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

std::string request_to_string(Request* request) {
    std::string result = "";

    // Start Line
    result += request->method;
    result += " ";
    result += request->resource;
    result += " ";
    result += "HTTP/1.1\n";

    // Headers
    for (std::vector<key_value*>::iterator it = request->headers.begin(); it != request->headers.end(); it++) {
        result += (*it)->key + ": " + (*it)->value + "\n";
    }

    // Empty Line
    result += "\n";

    // Body
    result += request->body;

    return result;
}

Response* string_to_response(std::string response) {

    // Start line
    std::string version = "";
    std::string response_code_string = "";
    int response_code = -1;
    std::string message = "";

    std::string::iterator it = response.begin();
    std::string::iterator start = it;
    std::string::iterator dest = version.begin();
    int mode = 0;
    while ((*it) != '\n') {
        if ((*it) == ' ') {
            if (mode == 0) {
                dest = version.begin();
            } else if (mode == 1) {
                dest = response_code_string.begin();
            } else {
                dest = message.begin();
            }
            std::copy(start, --it, dest);
            it++; // To counteract the '--it'
            start = it + 1;
            mode++;
        }
        it++;
    }
    std::copy(start, --it, dest);
    // Final copy needed since the response ends the message in a newline
    it++; // To counteract the '--it'
    start = it + 1;
    it++; // To move forward past the newline

    // Headers
    std::vector<key_value*> headers;
    int newlines_in_sequence = 0;
    std::string key_buffer = "";
    std::string value_buffer = "";
    start = it;
    while (newlines_in_sequence < 2) {
        if ((*it) == '\n') {
            newlines_in_sequence ++;
            std::copy(start, --it, value_buffer);
            it++; // To counteract the --it
            key_value* kv = new key_value{key_buffer, value_buffer};
            headers.push_back(kv);
        } else if ((*it) == ':') {
            std::copy(start, --it, key_buffer);
            it++; // To counteract the --it
        }
        it++;
    }

    // Empty Line
    it++;
    start = it;
    // Body
    std::string body = "";
    while ((*it) != EOF) {
        it++;
    }
    std::copy(start, --it, body);

    return new Response(version, response_code, message, headers, body);
}

/**
 * Simple Constructor for the Request class, this allows specification for all of the necessary fields of an HTTP request:
 * See this article for that specification https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Messages
 * This constructor also creater copies of each of the arguments rather than using the arguments as given.
 * @param method One of the predefined HTTP headers
 * @param url The url to request.
 * @param header The list of headers for the request to manage
 * @param body The body text to send with the message
 */
Request::Request(std::string method, std::string url,
    std::vector<key_value*> headers, std::string body) {
    this->method = method;
    this->resource = url;
    this->headers = std::vector<key_value*>();
    this->body = body;

    // Copy the elements from the argument 'headers' to the local 'headers' field
    for (std::vector<key_value*>::iterator it = headers.begin(); it != headers.end(); it++ ) {
        this->headers.push_back(*it); // Dereference the iterator
    }

}

Request::~Request() {
    for (std::vector<key_value*>::iterator it = this->headers.begin(); it != this->headers.end(); it++) {
       delete (*it);
    }
}

std::shared_ptr<Response> Request::send() {
    if (!initialized_winsock) {
        initialize_winsock();
        initialized_winsock = true;
    }
    std::string port = "80";
    SOCKET socket = connect_to_url(this->resource, port);

    std::string request_as_string = request_to_string(this);
    std::string response_as_string = "";

    int send_result = send_buffer(socket, request_as_string);
    int result_bytes = recieve_buffer(socket, &response_as_string);

    std::cout << response_as_string << std::endl;
    // Response* response = string_as_response(response_as_string);

    close_socket(socket);
}
