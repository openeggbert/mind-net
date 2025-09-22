//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/other/plugins/slipbox/models/Source.h"

namespace mindnet::plugins::slipbox::models
{
    entity_fields Source::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(cast64(map_id));
        result.push_back(title);
        result.push_back(author);
        result.push_back(cast64(year));
        result.push_back(publisher);
        result.push_back(edition);
        result.push_back(pages);
        result.push_back(url);
        result.push_back(cast64(type));

        return result;
    }

    void Source::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        //
        map_id = number();
        title = text();
        author = text();
        year = number();
        publisher = text();
        edition = text();
        pages = text();
        url = text();
        type = static_cast<enums::SourceType>(number());
    }

    string Source::validate()
    {
        using columns::SourceColumns;

        validator_chain_vector list{
            [this] { return test_ne(map_id, 0, SourceColumns::MAP_ID); },
            [this] { return testt_between(title, 0, 128, SourceColumns::TITLE); },
            [this] { return testt_between(author, 0, 64, SourceColumns::AUTHOR); },
            [this] { return test_at_most(year, 3000, SourceColumns::YEAR); },
            [this] { return testt_between(publisher, 0, 128, SourceColumns::PUBLISHER); },
            [this] { return testt_between(edition, 0, 64, SourceColumns::EDITION); },
            [this] { return testt_between(pages, 0, 64, SourceColumns::PAGES); },
            [this] { return testt_between(url, 0, 256, SourceColumns::URL); },

        };
        return util::ValidatorChain::run(list);
    }
}
