//
// Created by robertvokac on 10/23/25.
//
#pragma once


#include <string>
#include <unordered_map>
#include <vector>

#include "mindnet/api/AccessTokenContext.hpp"

#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/model/ModelDefinition.hpp"
#include <map>

#include "mindnet/api/OperationResult.hpp"
#include "mindnet/plugins/slipbox/triggers/ContentLinkParser.hpp"

namespace mindnet::plugins::slipbox::triggers
{
    class LinkSynchronizer
    {
    public:
        LinkSynchronizer(
            api::AccessTokenContext& token,
            int stack_depth,
            i64 note_id,
            std::function<api::OperationResult(
                const model::ModelDefinition&,
                api::AccessTokenContext&,
                int,
                int)> run_delete,
            std::function<std::pair<int, api::OperationResult>(
                const model::ModelDefinition&,
                api::AccessTokenContext&,
                entity_fields&,
                int)> run_create
        );

        void sync_urls(const std::vector<std::string>& old_urls,
                       const std::map<std::string, i64>& old_urls_ids,
                       const std::vector<std::string>& new_urls);

        void sync_links(const std::vector<std::string>& old_links,
                        const std::map<std::string, i64>& old_links_ids,
                        const std::vector<WikiLink>& new_links,
                        const std::unordered_map<std::string, i64>& title_to_id);

        void sync_wanted_notes(const std::vector<std::string>& old_wanted,
                               const std::map<std::string, i64>& old_wanted_ids,
                               const std::vector<WikiLink>& new_wanted);

    private:
        api::AccessTokenContext& token;
        int stack_depth;
        i64 note_id;
        std::function<api::OperationResult(
            const model::ModelDefinition&,
            api::AccessTokenContext&,
            int,
            int)> run_delete_;
        std::function<std::pair<int, api::OperationResult>(
            const model::ModelDefinition&,
            api::AccessTokenContext&,
            entity_fields&,
            int)> run_create_;


        template <typename Entity, typename MakeFn>
        void sync_entities(const std::vector<std::string>& old_items,
                           const std::map<std::string, i64>& old_ids,
                           const std::vector<std::string>& new_items,
                           const mindnet::model::ModelDefinition& def,
                           MakeFn make_entity_fn);
    };
}
