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
#include "mindnet/plugins/repetition/RepetitionPluginFactory.hpp"

#include "mindnet/plugins/repetition/validators/RGlobalSettingValidator.hpp"
#include "mindnet/plugins/repetition/validators/RUserSettingValidator.hpp"
#include "mindnet/plugins/repetition/validators/RSessionValidator.hpp"
#include "mindnet/plugins/repetition/validators/RReviewValidator.hpp"
#include "mindnet/plugins/repetition/validators/R0StateValidator.hpp"
#include "mindnet/plugins/repetition/validators/R2StateValidator.hpp"
#include "mindnet/plugins/repetition/validators/R4StateValidator.hpp"
#include "mindnet/plugins/repetition/validators/R18StateValidator.hpp"
#include "mindnet/plugins/repetition/validators/R18PerfAggValidator.hpp"
#include "mindnet/plugins/repetition/validators/R18PredictionLogValidator.hpp"

#include "mindnet/plugins/repetition/migrations/RepetitionSQLiteMigrationScripts.hpp"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/GetRSessionSelectedItemsSQLiteQuery.hpp"
#include "mindnet/plugins/repetition/triggers/AfterUpdateContentSemanticVersionTrigger.hpp"
#include "mindnet/plugins/repetition/triggers/RReviewAfterCreateTrigger.hpp"
#include "mindnet/plugins/repetition/triggers/RSessionBeforeCreateTrigger.hpp"

namespace mindnet::plugins::repetition
{
    api::PluginPtr RepetitionPluginFactory::create(std::shared_ptr<api::RepositoryFactory>& repository_factory) const
    {
        auto plugin = std::make_shared<api::Plugin>(
            REPETITION_PLUGIN_NAME,
            "Repetition",
            std::vector<std::string>{"repetition"},
            std::vector<std::string>{"slip_box"}
        );

        REGISTER_MIGRATIONS(Repetition, SQLite)

        REGISTER_MODEL(r_global_setting, RGlobalSetting, R_GLOBAL_SETTING)
        REGISTER_MODEL(r_user_setting, RUserSetting, R_USER_SETTING)
        REGISTER_MODEL(r_session, RSession, R_SESSION)
        REGISTER_MODEL(r_review, RReview, R_REVIEW)
        REGISTER_MODEL(r0_state, R0State, R0_STATE)
        REGISTER_MODEL(r2_state, R2State, R2_STATE)
        REGISTER_MODEL(r4_state, R4State, R4_STATE)
        REGISTER_MODEL(r18_state, R18State, R18_STATE)
        REGISTER_MODEL(r18_perf_agg, R18PerfAgg, R18_PERF_AGG)
        REGISTER_MODEL(r18_prediction_log, R18PredictionLog, R18_PREDICTION_LOG)

        plugin->register_trigger(std::make_shared<triggers::RSessionBeforeCreateTrigger>());
        plugin->register_trigger(std::make_shared<triggers::RReviewAfterCreateTrigger>());
        plugin->register_trigger(std::make_shared<triggers::AfterUpdateContentSemanticVersionTrigger>());

        plugin->register_query(std::make_shared<db::sqlite::queries::GetRSessionSelectedItemsSQLiteQuery>());
        plugin->close_for_changes();
        return plugin;
    }
}