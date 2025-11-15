//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/plugins/slipbox/models/MapCollectionItem.hpp"

namespace mindnet::plugins::slipbox::models
{
    entity_fields MapCollectionItem::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(map_collection_id);
        result.push_back(map_id);
        result.push_back(position);
        return result;
    }

    void MapCollectionItem::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        map_collection_id = number();
        map_id = number();
        position = number();
    }

    string MapCollectionItem::validate()
    {
        using columns::MapCollectionItemColumns;

        validator_chain_vector list{
            [this] { return test_ne(map_collection_id, 0, MapCollectionItemColumns::MAP_COLLECTION_ID); },
            [this] { return test_ne(map_id, 0, MapCollectionItemColumns::MAP_ID); }
        };
        return util::ValidatorChain::run(list);
    }
}
