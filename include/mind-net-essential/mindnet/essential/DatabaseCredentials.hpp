//
// Created by robertvokac on 9/28/25.
//

#pragma once

#include <string>

namespace mindnet::essential
{
    struct DatabaseCredentials
    {
        std::string host;
        int port{};
        std::string name;
        std::string user;
        std::string password;
    };
}
