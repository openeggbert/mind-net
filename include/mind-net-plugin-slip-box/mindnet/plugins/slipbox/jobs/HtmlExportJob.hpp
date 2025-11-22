//
// Created by robertvokac on 9/7/25.
//
#pragma once

#include "mindnet/api/Job.hpp"
#include "mindnet/plugins/slipbox/models/Map.hpp"
#include "mindnet/plugins/slipbox/models/Note.hpp"
#include "mindnet/plugins/core/models/User.hpp"

namespace mindnet::plugins::slipbox::jobs
{
    using std::string;

    class HtmlExportJob : public api::Job
    {
    public:
        HtmlExportJob();

        ~HtmlExportJob() = default;

        string run(api::cronq::JobConfig& job_config) override;
    private:
        string generate_map(std::filesystem::path& export_map_dir, models::Map& map, api::AccessTokenContext& token);
        string generate_page(models::Note& note, api::AccessTokenContext& token);

    };
}
