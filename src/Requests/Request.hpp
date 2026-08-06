/**
 * Header for the Request class
 * @author Jerry-Spice
 */
#include <memory>
#include <vector>
#include "HTTP.hpp"
#include "Response.hpp"

#ifndef Request_hpp
#define Request_hpp

/**
 * This is the Request class, it's only job is to handle creating and sending requests to webpages via a socket connection.
 */
class Request {
    public:
        std::string method;
        std::string resource;
        std::string version;
        std::vector<key_value*> headers;
        std::string body;
        /**
         * Simple Constructor for the Request class, this allows specification for all of the necessary fields of an HTTP request:
         * See this article for that specification https://developer.mozilla.org/en-US/docs/Web/HTTP/Guides/Messages
         * This constructor also creater copies of each of the arguments rather than using the arguments as given.
         * @param method One of the predefined HTTP headers
         * @param url The url to request.
         * @param header The list of headers for the request to manage
         * @param body The body text to send with the message
         */
        Request(std::string method, std::string url, std::vector<key_value*> headers, std::string body);

        /**
         * Simple desctructor, this should realistically just handle the vectors since those are copied.
         */
        ~Request();

        /**
         * This is an asynchronous operation. It will block by default and returns a shared pointer to a Response obejct. See the Response.hpp file for more information
         */
        std::shared_ptr<Response> send();
};

#endif
