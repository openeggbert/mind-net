//
// Created by robertvokac on 9/28/25.
//

#ifndef MIND_NET_DATABASECREDENTIALS_H
#define MIND_NET_DATABASECREDENTIALS_H
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
#endif //MIND_NET_DATABASECREDENTIALS_H
