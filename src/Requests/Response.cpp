#include <vector>
#include <string>

#include "../Util/key_value.h"
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

Response* string_to_response(std::string response) {
    // std::cout << response << std::endl;
    // std::cout << "----------" << std::endl;
     // TODO: There's a bug with the key-value parsing. It doesn't populate properly
    std::string version = "";
    std::string response_buffer = "";
    int response_code = 0;
    std::string message = "";
    std::vector<key_value*> headers;
    std::string body = "";

    // Start Line
    std::string::iterator it = response.begin();
    std::string::iterator start = it;
    int spaces = 0;
    while (*it != '\r' && *(it + 1) != '\n') {
        if (*it == ' ') {
            spaces++;
        } else {
            if (spaces == 0) {
                version += *it;
            } else if (spaces == 1) {
                response_buffer += *it;
            } else {
                message += *it;
            }
        }
        it++;
    }
    response_code = std::stoi(response_buffer);

    // Headers
    it+= 2;
    // if (*it == '\r') {
    //    std::cout << "\\r" << std::endl;
    // } else if (*it == '\n') {
    //     std::cout << "\\n" << std::endl;
    // } else {
    //     std::cout << *it << std::endl;
    // }
    int newlines_in_sequence = 0;
    std::string key_buffer = "";
    std::string value_buffer = "";
    int mode = 0;
    while (newlines_in_sequence < 1) {
        // if (*it == '\r') {
        //     std::cout << "[" << mode << "] "  << "\\r" << std::endl;
        // } else if (*it == '\n') {
        //     std::cout << "[" << mode << "] "  << "\\n" << std::endl;
        // } else {
        //     std::cout << "[" << mode << "] " << *it << std::endl;
        // }
        if (*it == '\r' && *(it + 1) == '\n') {
            mode = 0;
            if (key_buffer.compare("") == 0) {
                newlines_in_sequence++;
            } else {
                key_value* kv = new key_value{key_buffer, value_buffer};
                headers.push_back(kv);
                // std::cout << key_buffer << ": " << value_buffer << std::endl;
                key_buffer = "";
                value_buffer = "";
            }
            it++;
        } else if (*it == ':') {
            mode = 1;
        } else {
            if (mode == 0) {
                key_buffer += *it;
            } else if (mode == 1) {
                value_buffer += *it;
            }
        }
        it++;
    }
    // Empty Line

    // Body
    int body_length = 0;
    for (std::vector<key_value*>::iterator header = headers.begin(); header != headers.end(); header++) {
        if ((*header)->key.compare("content-length") == 0) {
            body_length = std::stoi((*header)->value);
        }
    }
    int characters = 0;
    while (it != response.end()) {
        body += *it;
        characters++;
        it++;
    }

    return new Response(version, response_code, message, headers, body);
}
