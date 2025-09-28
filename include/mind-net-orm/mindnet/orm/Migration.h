//
// Created by robertvokac on 9/28/25.
//

#ifndef MIND_NET_MIGRATION_H
#define MIND_NET_MIGRATION_H
#include <string>

namespace mindnet::orm
{
    using std::string;

    struct Migration
    {

        string plugin_name;
        int version;
        string description;

        string script;
        string hash_sha256;
        string installed_on;

        // in microseconds
        int execution_time;
    };
}
#endif //MIND_NET_MIGRATION_H
