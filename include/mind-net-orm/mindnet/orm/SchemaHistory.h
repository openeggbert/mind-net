//
// Created by robertvokac on 9/28/25.
//

#ifndef MIND_NET_MIGRATION_H
#define MIND_NET_MIGRATION_H
#include <string>

namespace mindnet::orm
{
    using std::string;

    struct SchemaHistory
    {

        string plugin_name;
        int version;
        string description;

        string script;
        string checksum;
        string chain_hash;
        string installed_on;

        // in microseconds
        int execution_time;
        bool success;
    };
}
#endif //MIND_NET_MIGRATION_H
