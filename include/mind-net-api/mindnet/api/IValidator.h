//
// Created by robertvokac on 9/7/25.
//
#ifndef IVALIDATOR_H
#define IVALIDATOR_H

#include <functional>
#include <memory>

#include "LoginToken.h"
#include "OperationResult.h"
#include "mindnet/essential/Helper.h"

namespace mindnet::api {

    class IPersistence; }

namespace mindnet::api
{
    class IValidator;
    typedef std::function<IValidator*(const std::string&)> GetValidatorFunc;

    using string = std::string;

    class IValidator
    {
    public:
        virtual ~IValidator() = default;

        typedef std::shared_ptr<IPersistence> DbPtr;

        virtual OperationResult can_create(DbPtr& db, api::LoginToken& token, entity_fields& ef) const = 0;

        virtual OperationResult can_read(DbPtr& db, api::LoginToken& token, int id) const = 0;

        virtual OperationResult can_update(DbPtr& db, api::LoginToken& token, entity_fields& ef) const = 0;

        virtual OperationResult can_delete(DbPtr& db, api::LoginToken& token, int id) const = 0;

        virtual OperationResult can_list(DbPtr& db, api::LoginToken& token, string_map& filter) const = 0;

        [[nodiscard]] virtual string get_model_name() const = 0;

        virtual void set_validator_func(GetValidatorFunc func) = 0;
        [[nodiscard]] virtual GetValidatorFunc get_validator_func() const = 0;

        [[nodiscard]] IValidator* get_validator(const string& model_name) const
        {
            auto func = get_validator_func();
            if (!func) return nullptr;
            return func(model_name);
        }
    };
}
#endif // IVALIDATOR_H
