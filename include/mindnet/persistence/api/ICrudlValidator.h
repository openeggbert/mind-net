//
// Created by robertvokac on 9/7/25.
//
#ifndef ICRUDLVALIDATOR_H
#define ICRUDLVALIDATOR_H

#include <variant>
#include <map>
#include <vector>

#include "OperationResult.h"

namespace mindnet::persistence::api { class IPersistence; }

namespace mindnet::http { struct LoginToken; }

namespace mindnet::persistence::api
{
    using string = std::string;

    typedef std::variant<std::string, int64_t> entity_field;
    typedef std::vector<entity_field> entity_fields;
    typedef std::map<std::string, std::string> string_map;

    class ICrudlValidator
    {
    public:
        virtual ~ICrudlValidator() = default;

        virtual OperationResult can_create(IPersistence* db, http::LoginToken& token, entity_fields& ef) const = 0;

        virtual OperationResult can_read(IPersistence* db, http::LoginToken& token, int id) const = 0;

        virtual OperationResult can_update(IPersistence* db, http::LoginToken& token, entity_fields& ef) const = 0;

        virtual OperationResult can_delete(IPersistence* db, http::LoginToken& token, int id) const = 0;

        virtual OperationResult can_list(IPersistence* db, http::LoginToken& token, string_map& filter) const = 0;

        [[nodiscard]] virtual string get_model_name() const = 0;

    };
}
#endif // ICRUDLVALIDATOR_H
