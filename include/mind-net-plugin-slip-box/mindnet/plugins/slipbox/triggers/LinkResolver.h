//
// Created by robertvokac on 10/23/25.
//
#ifndef LINKRESOLVER_H
#define LINKRESOLVER_H


#include <string>
#include <unordered_map>
#include <vector>
#include "nlohmann/json.hpp"

#include "mindnet/essential/Helper.h"


namespace mindnet::plugins::slipbox::triggers
{
    struct LinkResolution
    {
        std::vector<std::string> existing;
        std::vector<std::string> missing;
        std::unordered_map<std::string, i64> title_to_id;
    };


    class LinkResolver
    {
    public:
        static LinkResolution resolve(
            i64 map_id,
            const std::vector<std::string>& wikilinks,
            std::function<nlohmann::json(const std::string&, nlohmann::json&)>& call);
    };
}
#endif // LINKRESOLVER_H
