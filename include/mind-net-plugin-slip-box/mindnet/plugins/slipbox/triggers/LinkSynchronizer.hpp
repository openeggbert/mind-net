/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

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
            identification note_id,
            std::function<api::OperationResult(
                const model::ModelDefinition&,
                api::AccessTokenContext&,
                identification,
                int)> run_delete,
            std::function<std::pair<identification, api::OperationResult>(
                const model::ModelDefinition&,
                api::AccessTokenContext&,
                entity_fields&,
                int)> run_create
        );

        void sync_urls(const std::vector<std::string>& old_urls,
                       const std::map<std::string, identification>& old_urls_ids,
                       const std::vector<std::string>& new_urls);

        void sync_links(const std::vector<std::string>& old_links,
                        const std::map<std::string, identification>& old_links_ids,
                        const std::vector<WikiLink>& new_links,
                        const std::unordered_map<std::string, identification>& title_to_id);

        void sync_wanted_notes(const std::vector<std::string>& old_wanted,
                               const std::map<std::string, identification>& old_wanted_ids,
                               const std::vector<WikiLink>& new_wanted);

    private:
        api::AccessTokenContext& token;
        int stack_depth;
        identification note_id;
        std::function<api::OperationResult(
            const model::ModelDefinition&,
            api::AccessTokenContext&,
            identification,
            int)> run_delete_;
        std::function<std::pair<identification, api::OperationResult>(
            const model::ModelDefinition&,
            api::AccessTokenContext&,
            entity_fields&,
            int)> run_create_;


        template <typename Entity, typename MakeFn>
        void sync_entities(const std::vector<std::string>& old_items,
                           const std::map<std::string, identification>& old_ids,
                           const std::vector<std::string>& new_items,
                           const mindnet::model::ModelDefinition& def,
                           MakeFn make_entity_fn);
    };
}
