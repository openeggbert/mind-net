//
// Created by robertvokac on 8/16/25.
//

#ifndef MINI_WIKI_HTTPUTILS_H
#define MINI_WIKI_HTTPUTILS_H

#include "crow.h"
#include <string>


namespace mindnet::http {
    inline const std::string& http_method_to_string(crow::HTTPMethod method) {
        static const std::string methodNames[] = {
            "GET", "POST", "PUT", "DELETE", "PATCH", "OPTIONS", "HEAD"
        };

        static const std::string unknown = "UNKNOWN";

        size_t index = static_cast<size_t>(method);
        if (index < sizeof(methodNames) / sizeof(methodNames[0])) {
            return methodNames[index];
        } else {
            return unknown;
        }
    }

}
#endif //MINI_WIKI_HTTPUTILS_H