#include <vector>
#include <string>

#include "HTTP.hpp"
#include "Response.hpp"


Response::Response(std::string version, int response_code, std::string message, std::vector<key_value*> headers, std::string body) {
    this->version = version;
    this->response_code = response_code;
    this->message = message;
    for (std::vector<key_value*>::iterator it = headers.begin(); it != headers.end(); it++) {
        this->headers.push_back(*it);
    }
    this->body = body;
}

Response::~Response() {
    for (std::vector<key_value*>::iterator it = this->headers.begin(); it != this->headers.end(); it++) {
        delete *it;
    }
}
