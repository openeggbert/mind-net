//
// Created by robertvokac on 9/7/25.
//

#ifndef MIND_NET_CRUDLVALIDATORBASE_H
#define MIND_NET_CRUDLVALIDATORBASE_H

#include <expected>
#include <concepts>
#include <type_traits>
#include <utility>
#include "IPersistence.h"
#include "PersistenceMethods.h"
#include "mindnet/persistence/api/OperationResult.h"
#include "mindnet/Helper.h"
#include "mindnet/http/LoginToken.h"

namespace mindnet::persistence::api
{
    class Persistence;
}

namespace mindnet::persistence::api
{
    using db_ptr = mindnet::persistence::IPersistence*;
    typedef std::expected<void, OperationResult> result_t;

    struct ValidatorContext
    {
        db_ptr db;
        http::LoginToken& token;
        const models::User& logged_user;
    };


    template <typename Derived, typename Model>
    class CrudlValidatorBase
    {
        static_assert(std::is_base_of_v<models::misc::BaseModel, Model>,
                      "Model must derive from BaseModel");

    public:
        virtual ~CrudlValidatorBase() = default;

        OperationResult can_create(db_ptr db, http::LoginToken& token, entity_fields& ef) const
        {
            static_assert(
                requires(const Derived& d, ValidatorContext const& ctx, const Model& m)
                {
                    { d.validate_create(ctx, m) } -> std::convertible_to<result_t>;
                },
                "Derived must implement validate_create returning result_t"
            );


            auto [logged_user, logged_user_result] = find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            ValidatorContext context{db, token, logged_user};
            ////
            Model entity;
            entity.from_values(ef);
            //
            if (auto error = entity.validate(); !error.empty())
                return {400, error};
            ////
            if (auto res = derived().validate_create(context, entity); !res)
                return res.error();

            return ok_result;
        }

        OperationResult can_read(db_ptr db, http::LoginToken& token, int id)
        {
            static_assert(
                requires(const Derived& d, ValidatorContext const& ctx, const Model& m)
                {
                    { d.validate_read(ctx, m) } -> std::convertible_to<result_t>;
                },
                "Derived must implement validate_read returning result_t"
            );

            auto [logged_user, logged_user_result] = find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            ValidatorContext context{db, token, logged_user};
            ////
            auto [values, read_err] = db->read(id, Model::definition(), token);
            if (read_err.ko()) return read_err;
            //
            Model entity;
            entity.from_values(values);
            ////
            if (auto res = derived().validate_read(context, entity); !res)
                return res.error();

            return ok_result;
        }

        OperationResult can_update(db_ptr db, http::LoginToken& token, entity_fields& ef) const
        {
            static_assert(
                requires(const Derived& d, ValidatorContext const& ctx, const Model& old_m, const Model& new__m)
                {
                    { d.validate_update(ctx, old_m, new__m) } -> std::convertible_to<result_t>;
                },
                "Derived must implement validate_update returning result_t"
            );

            auto [logged_user, logged_user_result] = find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            ValidatorContext context{db, token, logged_user};
            ////
            Model new_entity;
            new_entity.from_values(ef);

            auto [old_values, read_err] = db->read(new_entity.get_id(), Model::definition(), token);
            if (read_err.ko()) return read_err;

            Model old_entity;
            old_entity.from_values(old_values);
            //
            if (auto error = new_entity.validate(); !error.empty())
                return {400, error};

            if (auto error = validate_readonly(old_values, ef, Model::definition()); !error.empty())
                return {400, error};
            ////
            if (auto res = derived().validate_update(
                context, new_entity, old_entity); !res.has_value())
                return res.error();

            return ok_result;
        }

        OperationResult can_delete(db_ptr db, http::LoginToken& token, int id) const
        {
            static_assert(
                requires(const Derived& d, ValidatorContext const& ctx, const Model& m)
                {
                    { d.validate_delete(ctx, m) } -> std::convertible_to<result_t>;
                },
                "Derived must implement validate_delete returning result_t"
            );

            auto [logged_user, logged_user_result] = find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            ValidatorContext context{db, token, logged_user};
            ////
            auto [values, read_err] = db->read(id, Model::definition(), token);
            if (read_err.ko()) return read_err;

            Model entity;
            entity.from_values(values);
            ////
            if (auto res = derived().validate_delete(context, entity); !res)
                return res.error();

            return ok_result;
        };

        OperationResult can_list(db_ptr db, http::LoginToken& token, string_map& filter) const
        {
            static_assert(
                requires(const Derived& d, ValidatorContext const& ctx, const string_map& fm)
                {
                    { d.validate_list(ctx, fm) } -> std::convertible_to<result_t>;
                },
                "Derived must implement validate_list returning result_t"
            );

            auto [logged_user, logged_user_result] = find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            ValidatorContext context{db, token, logged_user};
            ////
            if (auto res = derived().validate_list(context, filter); !res)
                return res.error();

            return ok_result;
        };
        [[nodiscard]] virtual string get_model_name() const = 0;

    private:
        const Derived& derived() const { return static_cast<const Derived&>(*this); }
    };
}
#endif //MIND_NET_CRUDLVALIDATORBASE_H
