//
// Created by robertvokac on 9/7/25.
//
#ifndef ICRUDLVALIDATOR_H
#define ICRUDLVALIDATOR_H

#include <variant>
#include <map>
#include <vector>

#include "IPersistence.h"
#include "../../OperationResult.h"

namespace mindnet::http
{
    struct LoginToken;
}

namespace mindnet::persistence::api
{
    class ICrudlValidator;
    typedef std::function<ICrudlValidator*(const std::string&)> GetValidatorFunc;

    using string = std::string;

    typedef std::variant<std::string, int64_t> entity_field;
    typedef std::vector<entity_field> entity_fields;
    typedef std::map<std::string, std::string> string_map;

    class ICrudlValidator
    {
    public:
        virtual ~ICrudlValidator() = default;

        virtual OperationResult can_create(DbPtr& db, http::LoginToken& token, entity_fields& ef) const = 0;

        virtual OperationResult can_read(DbPtr& db, http::LoginToken& token, int id) const = 0;

        virtual OperationResult can_update(DbPtr& db, http::LoginToken& token, entity_fields& ef) const = 0;

        virtual OperationResult can_delete(DbPtr& db, http::LoginToken& token, int id) const = 0;

        virtual OperationResult can_list(DbPtr& db, http::LoginToken& token, string_map& filter) const = 0;

        [[nodiscard]] virtual string get_model_name() const = 0;

        virtual void set_validator_func(GetValidatorFunc func) = 0;
        [[nodiscard]] virtual GetValidatorFunc get_validator_func() const = 0;

        [[nodiscard]] ICrudlValidator* get_validator(const string& model_name) const
        {
            auto func = get_validator_func();
            if (!func) return nullptr;
            return func(model_name);
        }
    };
}
#endif // ICRUDLVALIDATOR_H
