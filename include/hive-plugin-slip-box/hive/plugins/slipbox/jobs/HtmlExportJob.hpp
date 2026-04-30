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

#include "hive/api/Job.hpp"
#include "hive/plugins/slipbox/models/Map.hpp"
#include "hive/plugins/slipbox/models/Note.hpp"
#include "hive/plugins/slipbox/models/Content.hpp"
#include "hive/plugins/core/models/User.hpp"

namespace hive::plugins::slipbox::jobs
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
        string generate_page(models::Note& note, api::AccessTokenContext& token, string& author_display_name,
                             models::Map& map, std::filesystem::
                             path& export_map_dir);
    };
}