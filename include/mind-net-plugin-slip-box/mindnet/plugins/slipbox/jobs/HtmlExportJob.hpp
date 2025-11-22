//
// Created by robertvokac on 9/7/25.
//
#pragma once

#include "mindnet/api/Job.hpp"

namespace mindnet::plugins::slipbox::jobs
{
    class HtmlExportJob : public api::Job
    {
    public:
        HtmlExportJob();

        ~HtmlExportJob() = default;

        std::string run(api::cronq::JobConfig& job_config) override;
    private:
        std::string get_html_template();
        std::string get_styles_css();
        std::string get_script_css();

    private:
    };
}
