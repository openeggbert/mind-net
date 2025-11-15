//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/MapCollection.hpp"

namespace mindnet::plugins::slipbox::models
{
    entity_fields MapCollection::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(name);
        result.push_back(description);
        result.push_back(created_by);
        result.push_back(cast64(is_public));
        return result;
    }

    void MapCollection::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        name = text();
        description = text();
        created_by = number();
        is_public = boolean();
    }

    string MapCollection::validate()
    {
        using columns::MapCollectionColumns;

        validator_chain_vector list{
            [this] { return testt_between(name, 1, 80, MapCollectionColumns::NAME); },
            [this] { return testt_between(description, 0, 255, MapCollectionColumns::DESCRIPTION); },
            [this] { return test_ne(created_by, 0, MapCollectionColumns::CREATED_BY); }
        };
        return util::ValidatorChain::run(list);
    }
}
