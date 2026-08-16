// These two definitions are required for windows builds for whatever reason...
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A01  // Target Windows 10 / 11 (or 0x0601 for Windows 7)
#endif
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Windows headers are strict and must be included first
#include <winsock2.h>
#include <ws2tcpip.h>

// Now include OpenSSL
#include <openssl/ssl.h>
#include <openssl/err.h>

#include <string>

#ifndef Sockets_hpp
#define Sockets_hpp


// ---------- WINDOWS SOCKETS ----------

int initialize_winsock();

SOCKET connect_to_url(std::string url, std::string port);

int send_buffer(SOCKET socket, std::string buffer);

int recieve_buffer(SOCKET socket, std::string* buffer);

int close_socket(SOCKET socket);

// ---------- OPENSSL ----------

// Now I'm including OpenSSL headers so that I can interact with HTTPS servers
SSL_CTX* initialize_openssl();

SSL* perform_handshake(SOCKET socket, SSL_CTX* ctx, std::string host);

int secure_send(SSL* ssl, std::string content);

int secure_recv(SSL* ssl, std::string* buffer);

void cleanup_openssl(SSL* ssl, SSL_CTX* ctx);

#endif
