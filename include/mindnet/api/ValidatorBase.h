//
// Created by robertvokac on 9/7/25.
//
#ifndef VALIDATORBASE_H
#define VALIDATORBASE_H

#include "expected"
#include <concepts>
#include <type_traits>

#include "IValidator.h"
#include "../OperationResult.h"
#include "PersistenceMethods.h"
#include "../api/RequestContext.h"
#include "../model/BaseModel.h"

#include "mindnet/Helper.h"

#define create_method_prototypes_for_ValidatorBase(M)\
mindnet::OperationResult validate_create(const RequestContext&, const M& entity) const ;\
mindnet::OperationResult validate_read(const RequestContext&, const M& entity) const;\
mindnet::OperationResult validate_update(const RequestContext&, const M& old_entity, const M& new_entity) const;\
mindnet::OperationResult validate_delete(const RequestContext&, const M& entity) const;\
mindnet::OperationResult validate_list(const RequestContext&, const string_map&) const;\
[[nodiscard]] string get_model_name() const override;

#define return_if(condition, status, message) if (condition) return OperationResult(status, message);
#define assert_role(ROLE) \
return_if (ctx.role < mindnet::plugins::core::enums::UserRole:: ROLE ,403, "User does not have permission for this action.")

#define assert_admin() assert_role(ADMIN)
#define assert_editor() assert_role(EDITOR)

#define mandatory_filter(field)\
if (filter.find( STRINGIFY(field) ) == filter.end()) return {403, std::string("You can't filter without ") + STRINGIFY(field) + "."};

#define find_model(model, id) XPASTE(find_,model) (ctx, id);
#define check_found(f) if (!f.second.empty()) return{400,f.second};

namespace mindnet::api
{
    class IPersistence;
    class IValidator;
}

namespace mindnet::http
{
    class LoginToken;
}

namespace mindnet::api
{
    typedef std::function<IValidator*(const std::string&)> GetValidatorFunc;

    template <typename Derived, typename Model>
    class ValidatorBase : public api::IValidator
    {
        static_assert(std::is_base_of_v<model::BaseModel, Model>,
                      "Model must derive from BaseModel");

    private:
        GetValidatorFunc get_validator_func_ = nullptr;

    public:
        virtual ~ValidatorBase() = default;

        OperationResult can_create(DbPtr& db, http::LoginToken& token, entity_fields& ef) const
        {
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_create(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_create returning OperationResult"
            );

            auto [logged_user, logged_user_result] = find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            RequestContext context{db, token, logged_user.role, logged_user.status};
            ////
            Model entity;
            entity.from_values(ef);
            //
            if (auto error = entity.validate(); !error.empty())
                return {400, error};
            if (auto error = model::validate_enums(ef, db->get_model_definition(get_model_name()).value()); !error.
                empty())
                return {400, error};
            ////
            if (auto res = derived().validate_create(context, entity); !res.ok())
                return res;

            return ok_result;
        }

        OperationResult can_read(DbPtr& db, http::LoginToken& token, int id) const
        {
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_read(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_read returning OperationResult"
            );

            auto [logged_user, logged_user_result] = find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            RequestContext context{db, token, logged_user.role, logged_user.status};
            ////
            auto [values, read_err] = db->read(
                //todo
                db->get_model_definition(derived().get_model_name()).value(), token, id
            );
            if (read_err.ko()) return read_err;
            //
            Model entity;
            entity.from_values(values);
            ////
            if (auto res = derived().validate_read(context, entity); !res.ok())
                return res;

            return ok_result;
        }

        OperationResult can_update(DbPtr& db, http::LoginToken& token, entity_fields& ef) const
        {
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& old_m, const Model& new_m)
                {
                    { d.validate_update(ctx, old_m, new_m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_update returning OperationResult"
            );

            auto [logged_user, logged_user_result] = find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            RequestContext context{db, token, logged_user.role, logged_user.status};
            ////
            Model new_entity;
            new_entity.from_values(ef);

            auto [old_values, read_err] = db->read(db->get_model_definition(derived().get_model_name()).value(),
                                                   token, new_entity.get_id());
            if (read_err.ko()) return read_err;

            Model old_entity;
            old_entity.from_values(old_values);
            //
            if (auto error = new_entity.validate(); !error.empty())
                return {400, error};

            auto def = db->get_model_definition(derived().get_model_name()).value();
            if (auto error = validate_readonly(old_values, ef, def); !error.empty())
                return {400, error};
            if (auto error = model::validate_enums(ef, db->get_model_definition(get_model_name()).value()); !error.
                empty())
                return {400, error};
            ////
            if (auto res = derived().validate_update(
                context, new_entity, old_entity); !res.ok())
                return res;

            return ok_result;
        }

        OperationResult can_delete(DbPtr& db, http::LoginToken& token, int id) const
        {
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_delete(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_delete returning OperationResult"
            );

            auto [logged_user, logged_user_result] = find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            RequestContext context{db, token, logged_user.role, logged_user.status};
            ////
            auto [values, read_err] = db->read(db->get_model_definition(derived().get_model_name()).value(), token, id);
            if (read_err.ko()) return read_err;

            Model entity;
            entity.from_values(values);
            ////
            if (auto res = derived().validate_delete(context, entity); !res.ok())
                return res;

            return ok_result;
        };

        OperationResult can_list(DbPtr& db, http::LoginToken& token, string_map& filter) const
        {
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const string_map& fm)
                {
                    { d.validate_list(ctx, fm) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_list returning OperationResult"
            );

            auto [logged_user, logged_user_result] = find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            RequestContext context{db, token, logged_user.role, logged_user.status};
            ////
            if (auto res = derived().validate_list(context, filter); !res.ok())
                return res;

            return ok_result;
        };
        [[nodiscard]] virtual string get_model_name() const = 0;

        void set_validator_func(GetValidatorFunc func) override { get_validator_func_ = func; }
        [[nodiscard]] GetValidatorFunc get_validator_func() const { return get_validator_func_; }

    private:
        const Derived& derived() const { return static_cast<const Derived&>(*this); }
    };
}
#endif // VALIDATORBASE_H
