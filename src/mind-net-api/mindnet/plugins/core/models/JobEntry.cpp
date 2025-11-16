//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/core/models/JobEntry.hpp"

namespace mindnet::plugins::core::models
{
    entity_fields JobEntry::to_values() const
    {
        entity_fields result;
        result.emplace_back(id);
        result.emplace_back(cast64(created_at));
        result.emplace_back(cast64(updated_at));
        result.emplace_back(name);
        result.emplace_back(description);
        result.emplace_back(plugin_name);
        result.emplace_back(cron_expression);
        result.emplace_back(run_once_when_missed);
        result.emplace_back(enabled);
        result.emplace_back(configuration);
        result.emplace_back(cast64(last_run));
        result.emplace_back(cast64(next_run));
        return result;
    }

    void JobEntry::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        name = text();
        description = text();
        plugin_name = text();
        cron_expression = text();
        run_once_when_missed = boolean();
        enabled = boolean();
        configuration = text();
        last_run = number();
        next_run = number();
    }

    string JobEntry::validate()
    {
        using columns::JobEntryColumns;

        validator_chain_vector list{
            [this] { return testt_not_empty(name, JobEntryColumns::NAME); },
            [this] { return testt_not_empty(plugin_name, JobEntryColumns::PLUGIN_NAME); },
            [this] { return testt_not_empty(cron_expression, JobEntryColumns::CRON_EXPRESSION); },
            [this] { return test_at_least(last_run, 0, JobEntryColumns::LAST_RUN); },
            [this] { return test_at_least(next_run, 0, JobEntryColumns::NEXT_RUN); }
        };
        return util::ValidatorChain::run(list);
    }
}
