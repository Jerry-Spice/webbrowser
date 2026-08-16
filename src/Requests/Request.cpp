#include "Sockets.hpp"

#include "Response.hpp"
#include "Request.hpp"

#include <string>
#include <vector>
#include "HTTP.hpp"

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
    this->protocol = "";
    this->host = "";
    this->resource = "/";
    int mode = 0;
    for (std::string::iterator it = url.begin(); it != url.end(); ++it) {
        if (mode == 0) {
            if (*it == '/' && *(it + 1) == '/') {
                mode = 1; // host
                protocol += "//";
                it++;
            } else {
                protocol += *it;
            }
        } else if (mode == 1) {
            if (*it == '/') {
                mode = 2; // resource
            } else {
                host += *it;
            }
        } else {
            resource += *it;
        }
    }
    this->headers = std::vector<key_value*>();
    this->headers.push_back(new key_value{"Host", this->host});
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
    initialize_winsock();
    std::string port = "80";
    SOCKET socket = connect_to_url(this->protocol + this->host, port);

    std::string request_as_string = request_to_string(this);
    std::string response_as_string = "";

    int send_result = send_buffer(socket, request_as_string);
    int result_bytes = recieve_buffer(socket, &response_as_string);

    Response* response = string_to_response(response_as_string);

    close_socket(socket);
    return std::shared_ptr<Response>(response);
}

std::shared_ptr<Response> Request::send_secure() {
    initialize_winsock();
    SSL_CTX* ctx = initialize_openssl();

    std::string port = "443";
    SOCKET socket = connect_to_url(this->protocol + this->host, port);

    SSL* ssl = perform_handshake(socket, ctx, this->protocol + this->host);

    std::string request_as_string = request_to_string(this);
    std::string response_as_string = "";

    int send_result = secure_send(ssl, request_as_string);
    int result_bytes = secure_recv(ssl, &response_as_string);
    Response* response = string_to_response(response_as_string);

    close_socket(socket);
    return std::shared_ptr<Response>(response);
}
