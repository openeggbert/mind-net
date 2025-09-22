//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/other/plugins/slipbox/models/CollectionItem.h"

namespace mindnet::plugins::slipbox::models
{
    entity_fields CollectionItem::to_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(cast64(created_at));
        result.push_back(cast64(updated_at));
        result.push_back(collection_id);
        result.push_back(note_id);
        result.push_back(order_index);
        return result;
    }

    void CollectionItem::from_values(const entity_fields& values)
    {
        int i = 0;

        def_helper_lambdas()

        set_id(number());
        created_at = number();
        updated_at = number();
        collection_id = number();
        note_id = number();
        order_index = number();
    };

    string CollectionItem::validate()
    {
        using columns::CollectionItemColumns;

        validator_chain_vector list{
            [this] { return test_ne(collection_id, 0, CollectionItemColumns::COLLECTION_ID); },
            [this] { return test_ne(note_id, 0, CollectionItemColumns::NOTE_ID); },
        };
        return util::ValidatorChain::run(list);
    }
}
