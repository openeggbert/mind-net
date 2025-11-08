#include "mindnet/plugins/repetition/RepetitionPluginFactory.h"

#include "mindnet/plugins/repetition/validators/RGlobalSettingValidator.h"
#include "mindnet/plugins/repetition/validators/RUserSettingValidator.h"
#include "mindnet/plugins/repetition/validators/RSessionValidator.h"
#include "mindnet/plugins/repetition/validators/RReviewValidator.h"
#include "mindnet/plugins/repetition/validators/R0StateValidator.h"
#include "mindnet/plugins/repetition/validators/R2StateValidator.h"
#include "mindnet/plugins/repetition/validators/R4StateValidator.h"
#include "mindnet/plugins/repetition/validators/R18StateValidator.h"
#include "mindnet/plugins/repetition/validators/R18PerfAggValidator.h"
#include "mindnet/plugins/repetition/validators/R18PredictionLogValidator.h"

#include "mindnet/plugins/repetition/migrations/RepetitionSQLiteMigrationScripts.h"
#include "../../../../../include/mind-net-db-sqlite/mindnet/db/sqlite/queries/GetRSessionSelectedItemsSQLiteQuery.h"
#include "mindnet/plugins/repetition/triggers/AfterUpdateContentSemanticVersionTrigger.h"
#include "mindnet/plugins/repetition/triggers/RReviewAfterCreateTrigger.h"
#include "mindnet/plugins/repetition/triggers/RSessionBeforeCreateTrigger.h"

namespace mindnet::plugins::repetition
{
    const string REPETITION_PLUGIN_NAME = "repetition";

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
