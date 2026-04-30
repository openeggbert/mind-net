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
#include <vector>

#include "Job.hpp"
#include "MigrationScripts.hpp"
#include "ModelRegistration.hpp"
#include "Query.hpp"
#include "Trigger.hpp"

namespace hive::model
{
    struct ModelDefinition;
}

namespace hive::api
{
    class RepositoryFactory;

    class Plugin
    {
    public:
        Plugin(
            const std::string& name_,
            const std::string& description_,
            std::vector<string> apps_ = {},
            const std::vector<std::string>& plugins_which_this_plugin_depends_on_ = {}
        );
        ~Plugin() = default;

        [[nodiscard]] const std::string& get_name() const;
        [[nodiscard]] const std::string& get_description() const;
        [[nodiscard]] const std::vector<std::string>& depends_on_plugins() const;
        [[nodiscard]] const std::vector<string>& get_apps() const;
        [[nodiscard]] const std::vector<string>& get_library_files() const;
        void register_library_file(const string& library_file);
        void close_for_changes();
        [[nodiscard]] bool is_closed_for_changes() const;
        void register_model(
            const model::ModelDefinition& model_definition,
            const std::shared_ptr<IValidator>& validator,
            std::shared_ptr<RepositoryFactory>& repository_factory);
        void register_trigger(const TriggerPtr& trigger);
        void register_query(const QueryPtr& query);
        void register_job(const JobPtr& job);
        void register_migrations(const MigrationScriptsPtr& migration_scripts);
        [[nodiscard]] MigrationScriptsPtr get_migration_scripts() const;
        void destroy_migration_scripts();
        [[nodiscard]] const std::vector<TriggerPtr>& get_triggers() const;
        [[nodiscard]] const std::vector<QueryPtr>& get_queries() const;
        [[nodiscard]] const std::vector<JobPtr>& get_jobs() const;
        [[nodiscard]] const std::vector<std::shared_ptr<ModelRegistration>>& get_model_registrations() const;

    private:
        std::string name;
        std::string description;
        std::vector<string> plugins_which_this_plugin_depends_on;
        std::vector<string> apps;
        std::vector<string> library_files;
        bool is_closed_for_changes_ = false;
        std::vector<std::shared_ptr<ModelRegistration>> model_registrations;
        std::vector<TriggerPtr> triggers;
        std::vector<QueryPtr> queries;
        std::vector<JobPtr> jobs;
        MigrationScriptsPtr migration_scripts_ = nullptr;
    };

    typedef std::shared_ptr<Plugin> PluginPtr;
}