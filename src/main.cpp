#include "Requests/Sockets.hpp"
#include "Requests/Request.hpp"
#include "Requests/Response.hpp"

#include <iostream>
#include <vector>
#include <string>

// Test Main
int main() {
    std::vector<key_value*> headers;
    headers.push_back(new key_value{"User-Agent", "JLBrowser/0.1"});
    headers.push_back(new key_value{"Connection", "close"});
    Request r = Request("GET", "https://www.wikipedia.org/", headers, "");
    std::shared_ptr<Response> res = r.send_secure();
    std::cout << "----------" << std::endl;
    std::cout << res->version << " " << res->response_code << " " << res->message << std::endl;
    for (std::vector<key_value*>::iterator it = res->headers.begin(); it != res->headers.end(); it++) {
        std::cout << (*it)->key << ": " << (*it)->value << std::endl;
    }
    std::cout << "Body Length: " << res->body.length() << std::endl;
    return 0;
}
