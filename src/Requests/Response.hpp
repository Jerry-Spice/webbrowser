#include <string>
#include <vector>

#include "HTTP.hpp"

#ifndef Response_hpp
#define Response_hpp

class Response {

    public:
        std::string version;
        int response_code;
        std::string message;
        std::vector<key_value*> headers;
        std::string body;
        Response(std::string version, int response_code, std::string message, std::vector<key_value*> headers, std::string body);
        ~Response();


};

#endif
