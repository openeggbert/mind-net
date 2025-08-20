//
// Created by robertvokac on 8/4/25.
//

#include "mindnet/models/Map.h"

namespace mindnet::models
{

    entity_fields Map::get_values() const
    {
        entity_fields result;
        result.push_back(id);
        result.push_back(name);
        result.push_back(description);
        result.push_back(static_cast<int64_t>(created_at));
        return result;
    }

    void Map::from_values(const entity_fields& values)
    {
        int i = 0;
        for (const auto& entity_field : values)
        {

            if (std::holds_alternative<std::string>(entity_field)) {
                str text = std::get<std::string>(entity_field);
                switch (i)
                {
                case 1: name = text; break;
                case 2: description = text; break;
                default: throw std::runtime_error("Unknown column");
                }
            } else if (std::holds_alternative<int64_t>(entity_field)) {
                int64_t number = std::get<int64_t>(entity_field);
                if (i == 0) {set_id(number);} else
                {
                    switch (i)
                    {
                    case 3: created_at = number; break;
                    default: throw std::runtime_error("Unknown column");
                    }
                }
            } else
            {
                throw std::runtime_error("Unknown type");
            }

            i++;
        }

    };
}
