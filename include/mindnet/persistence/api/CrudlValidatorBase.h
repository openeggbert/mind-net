//
// Created by robertvokac on 9/7/25.
//

#ifndef MIND_NET_CRUDLVALIDATORBASE_H
#define MIND_NET_CRUDLVALIDATORBASE_H

#include <expected>
#include <concepts>
#include <type_traits>

#include "ICrudlValidator.h"
#include "OperationResult.h"
#include "PersistenceMethods.h"
#include "PersistenceTypedefs.h"
#include "RequestContext.h"
#include "mindnet/models/misc/BaseModel.h"

#include "mindnet/Helper.h"

#define create_method_prototypes_for_CrudlValidatorBase(M)\
api::OperationResult validate_create(const RequestContext&, const M& entity) const ;\
api::OperationResult validate_read(const RequestContext&, const M& entity) const;\
api::OperationResult validate_update(const RequestContext&, const M& old_entity, const M& new_entity) const;\
api::OperationResult validate_delete(const RequestContext&, const M& entity) const;\
api::OperationResult validate_list(const RequestContext&, const string_map&) const;\
[[nodiscard]] string get_model_name() const override;

#define return_if(condition, status, message) if (condition) return OperationResult(status, message);\

#define mandatory_filter(field)\
if (filter.find( STRING(field) ) == filter.end()) return {403, std::string("You can't filter without ") + STRING(field) + "."};

#define find_model(model, id) api:: XPASTE(find_,model) (ctx, id);
#define check_found(f) if (!f.second.empty()) return{400,f.second};

namespace mindnet::persistence::api {
    class IPersistence;
    class ICrudlValidator;
}

namespace mindnet::http {
    class LoginToken;
}

namespace mindnet::models {
    namespace misc {
        class BaseModel;
        class ModelDefinition;
    }
    class User;
}

namespace mindnet::persistence::api
{
    template <typename Derived, typename Model>
    class CrudlValidatorBase : public api::ICrudlValidator
    {
        static_assert(std::is_base_of_v<models::misc::BaseModel, Model>,
                      "Model must derive from BaseModel");

    public:
        virtual ~CrudlValidatorBase() = default;

        OperationResult can_create(IPersistence* db, http::LoginToken& token, entity_fields& ef) const
        {
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_create(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_create returning result_t"
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
            if (auto error = models::misc::validate_enums(ef, db->get_model_definition(get_model_name()).value()) ; !error.empty())
                return {400, error};
            ////
            if (auto res = derived().validate_create(context, entity); !res.ok())
                return res;

            return ok_result;
        }

        OperationResult can_read(IPersistence* db, http::LoginToken& token, int id) const
        {
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_read(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_read returning result_t"
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

        OperationResult can_update(IPersistence* db, http::LoginToken& token, entity_fields& ef) const
        {
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& old_m, const Model& new_m)
                {
                    { d.validate_update(ctx, old_m, new_m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_update returning result_t"
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
            if (auto error = models::misc::validate_enums(ef, db->get_model_definition(get_model_name()).value()) ; !error.empty())
                return {400, error};
            ////
            if (auto res = derived().validate_update(
                context, new_entity, old_entity); !res.ok())
                return res;

            return ok_result;
        }

        OperationResult can_delete(IPersistence* db, http::LoginToken& token, int id) const
        {
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_delete(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_delete returning result_t"
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

        OperationResult can_list(IPersistence* db, http::LoginToken& token, string_map& filter) const
        {
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const string_map& fm)
                {
                    { d.validate_list(ctx, fm) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_list returning result_t"
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

    private:
        const Derived& derived() const { return static_cast<const Derived&>(*this); }
    };
}
#endif //MIND_NET_CRUDLVALIDATORBASE_H
