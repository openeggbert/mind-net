/*
 * MIT License
 * Copyright (c) 2025 Robert Vokac
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#pragma once

#include "expected"
#include <concepts>
#include <type_traits>

#include "IValidator.hpp"
#include "OperationResult.hpp"
#include "RequestContext.hpp"
#include "../plugins/core/CorePersistenceMethods.hpp"
#include "mindnet/model/BaseModel.hpp"

#include "mindnet/essential/DatabaseType.hpp"

#define create_method_prototypes_for_ValidatorBase(M)\
mindnet::api::OperationResult validate_create_integrity(const RequestContext&, const M& entity) const ;\
mindnet::api::OperationResult validate_read_integrity(const RequestContext&, const M& entity) const;\
mindnet::api::OperationResult validate_update_integrity(const RequestContext&, const M& old_entity, const M& new_entity) const;\
mindnet::api::OperationResult validate_delete_integrity(const RequestContext&, const M& entity) const;\
mindnet::api::OperationResult validate_list_integrity(const RequestContext&, const string_map&) const;\
\
mindnet::api::OperationResult validate_create_authorization(const RequestContext&, const M& entity) const ;\
mindnet::api::OperationResult validate_read_authorization(const RequestContext&, const M& entity) const;\
mindnet::api::OperationResult validate_update_authorization(const RequestContext&, const M& old_entity, const M& new_entity) const;\
mindnet::api::OperationResult validate_delete_authorization(const RequestContext&, const M& entity) const;\
mindnet::api::OperationResult validate_list_authorization(const RequestContext&, const string_map&) const;\
[[nodiscard]] string get_model_name() const override;

#define return_if(condition, status, message) if (condition) return OperationResult(status, message);
#define assert_role(ROLE) \
return_if (ctx.role < mindnet::essential::UserRole:: ROLE, 403, "User does not have permission for this action.")

#define assert_superadmin() assert_role(Admin)
#define assert_admin() assert_role(Admin)
#define assert_editor() assert_role(Editor)
#define assert_reader() assert_role(Reader)
#define assert_guest() assert_role(Guest)

#define mandatory_filter(field)\
if (filter.find( STRINGIFY(field) ) == filter.end()) return {403, std::string("You can't filter without ") + STRINGIFY(field) + "."};

#define find_model(model, id) XPASTE(find_,model) (ctx, id);
#define check_found(f) if (!f.second.empty()) return{400,f.second};

namespace mindnet::api
{
    class IPersistence;
    class IValidator;
}

namespace mindnet::api
{
    using essential::g_configuration;
    using essential::AccessMode;
    using essential::Crudl;
    using mindnet::plugins::core::find_logged_user;

    inline bool is_authorization_enabled(const RequestContext& ctx)
    {
        using namespace mindnet;
        if (ctx.token.user_id == 0 && ctx.token.msg == "system") { return false; }

        switch (g_configuration.access_mode)
        {
        case AccessMode::MaintenanceMode:
            return true;
        case AccessMode::PublicFullAccess:
            return false;
        case AccessMode::AuthenticatedFullAccess:
            return ctx.role == essential::UserRole::Guest;
        default:
            return true;
        }
    }

    typedef std::function<std::shared_ptr<IValidator>(const std::string &)> GetValidatorFunc;

    template <typename Derived, typename Model>
    class ValidatorBase : public api::IValidator
    {
        static_assert(std::is_base_of_v<model::BaseModel, Model>,
                      "Model must derive from BaseModel");

    private:
        GetValidatorFunc get_validator_func_ = nullptr;

    public:
        virtual ~ValidatorBase() = default;

        OperationResult can_create(DbPtr& db, api::AccessTokenContext& token, entity_fields& ef) const override
        {
            auto action = Crudl::Create;
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_create_authorization(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_create_authorization returning OperationResult"
            );

            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_create_integrity(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_create_integrity returning OperationResult"
            );

            auto [logged_user, logged_user_result] = plugins::core::find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            RequestContext context{db, token, logged_user.role, logged_user.status};
            ////
            Model entity;
            entity.from_values(ef);
            //
            if (auto error = entity.validate(); !error.empty())
                return {400, error};
            auto def = db->get_model_definition(get_model_name()).value();
            if (auto error = validate_internal(ef, def); !error.empty())
                return {400, error};
            if (auto error = model::validate_enums(ef, def); !error.
                empty())
                return {400, error};
            ////

            if (is_authorization_enabled(context))
            {
                auto def = db->get_model_definition(get_model_name());
                auto authorized_to = token.is_system() || is_authorized_to(
                    logged_user.role, g_configuration.access_mode, action, def->is_reader_can_write());
                if (!authorized_to)
                    return {
                        403, "You are not authorized to access resource. " + def->get_model_name() + " CREATE"
                    };
                if (logged_user.role < essential::UserRole::Admin)
                    if (auto res = derived().validate_create_authorization(context, entity); !res.ok())
                        return res;
            }
            if (auto res = derived().validate_create_integrity(context, entity); !res.ok())
                return res;

            return ok_result;
        }

        OperationResult can_read(DbPtr& db, api::AccessTokenContext& token, identification id) const override
        {
            auto action = Crudl::Read;
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_read_authorization(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_read_authorization returning OperationResult"
            );
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_read_integrity(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_read_integrity returning OperationResult"
            );

            auto [logged_user, logged_user_result] = plugins::core::find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            RequestContext context{db, token, logged_user.role, logged_user.status};
            ////
            auto def = db->get_model_definition(get_model_name());
            Model entity;
            if (!def.value().is_no_table())
            {
                auto [values, read_err] = db->read(
                    db->get_model_definition(derived().get_model_name()).value(), token, id
                );
                if (read_err.ko()) return read_err;
                entity.from_values(values);
            }
            ////
            if (is_authorization_enabled(context))
            {
                auto authorized_to = token.is_system() || is_authorized_to(
                    logged_user.role, g_configuration.access_mode, action, def->is_reader_can_write());
                if (!authorized_to)
                    return {
                        403, "You are not authorized to access resource. " + def->get_model_name() + " READ"
                    };

                if (logged_user.role < essential::UserRole::Admin)
                    if (auto res = derived().validate_read_authorization(context, entity); !res.ok())
                        return res;
            }
            if (auto res = derived().validate_read_integrity(context, entity); !res.ok())
                return res;

            return ok_result;
        }

        OperationResult can_update(DbPtr& db, api::AccessTokenContext& token, entity_fields& new_fields,
                                   entity_fields& old_fields) const override
        {
            auto action = Crudl::Update;
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& old_m, const Model& new_m)
                {
                    { d.validate_update_authorization(ctx, old_m, new_m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_update_authorization returning OperationResult"
            );
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& old_m, const Model& new_m)
                {
                    { d.validate_update_integrity(ctx, old_m, new_m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_update_integrity returning OperationResult"
            );

            auto [logged_user, logged_user_result] = plugins::core::find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            RequestContext context{db, token, logged_user.role, logged_user.status};
            ////

            Model new_entity;
            new_entity.from_values(new_fields);

            auto [old_values, read_err] = db->read(db->get_model_definition(derived().get_model_name()).value(),
                                                   token, new_entity.get_id());
            if (read_err.ko()) return read_err;
            old_fields = old_values;

            bool has_hidden_column = false;
            auto def = db->get_model_definition(derived().get_model_name()).value();
            int column_index = 0;
            for (const model::ColumnDefinition& c : def.get_columns())
            {
                if (c.is_hidden())
                {
                    has_hidden_column = true;
                    auto type = c.get_column_type();
                    auto primitive_type = model::column_type_to_primitive_column_type(type);
                    if (primitive_type == model::PrimitiveColumnType::Text && std::get<string>(new_fields[column_index])
                        == "*")
                    {
                        new_fields[column_index] = std::get<string>(old_fields[column_index]);
                    }

                    if (primitive_type == model::PrimitiveColumnType::Number && std::get<i64>(new_fields[column_index])
                        == 0)
                    {
                        new_fields[column_index] = std::get<i64>(old_fields[column_index]);
                    }
                }
                column_index++;
            }

            if (has_hidden_column) new_entity.from_values(new_fields);

            Model old_entity;
            old_entity.from_values(old_values);
            //
            if (auto error = new_entity.validate(); !error.empty())
                return {400, error};

            if (auto error = validate_readonly(old_values, new_fields, def); !error.empty())
                return {400, error};
            if (auto error = validate_internal(old_values, new_fields, def); !error.empty())
                return {400, error};
            if (auto error = model::validate_enums(new_fields, db->get_model_definition(get_model_name()).value()); !
                error.
                empty())
                return {400, error};
            ////

            if (is_authorization_enabled(context))
            {
                auto def = db->get_model_definition(get_model_name());
                auto authorized_to = token.is_system() || is_authorized_to(
                    logged_user.role, g_configuration.access_mode, action, def->is_reader_can_write());
                if (!authorized_to)
                    return {
                        403, "You are not authorized to access resource. " + def->get_model_name() + " UPDATE"
                    };

                if (logged_user.role < essential::UserRole::Admin)
                    if (auto res = derived().validate_update_authorization(
                        context, old_entity, new_entity); !res.ok())
                        return res;
            }
            if (auto res = derived().validate_update_integrity(
                context, old_entity, new_entity); !res.ok())
                return res;

            return ok_result;
        }

        OperationResult can_delete(DbPtr& db, api::AccessTokenContext& token, identification id) const override
        {
            auto action = Crudl::Delete;
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_delete_authorization(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_delete_authorization returning OperationResult"
            );
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const Model& m)
                {
                    { d.validate_delete_integrity(ctx, m) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_delete_integrity returning OperationResult"
            );

            auto [logged_user, logged_user_result] = plugins::core::find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            RequestContext context{db, token, logged_user.role, logged_user.status};
            ////
            auto [values, read_err] = db->read(db->get_model_definition(derived().get_model_name()).value(), token, id);
            if (read_err.ko()) return read_err;

            Model entity;
            entity.from_values(values);
            ////
            if (is_authorization_enabled(context))
            {
                auto def = db->get_model_definition(get_model_name());
                auto authorized_to = token.is_system() || is_authorized_to(
                    logged_user.role, g_configuration.access_mode, action, def->is_reader_can_write());
                if (!authorized_to)
                    return {
                        403, "You are not authorized to access resource. " + def->get_model_name() + " DELETE"
                    };

                if (logged_user.role < essential::UserRole::Admin)
                    if (auto res = derived().validate_delete_authorization(context, entity); !res.ok())
                        return res;
            }
            if (auto res = derived().validate_delete_integrity(context, entity); !res.ok())
                return res;

            return ok_result;
        };

        OperationResult can_list(DbPtr& db, api::AccessTokenContext& token, string_map& filter) const override
        {
            auto action = Crudl::List;
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const string_map& fm)
                {
                    { d.validate_list_authorization(ctx, fm) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_list_authorization returning OperationResult"
            );
            static_assert(
                requires(const Derived& d, RequestContext const& ctx, const string_map& fm)
                {
                    { d.validate_list_integrity(ctx, fm) } -> std::convertible_to<OperationResult>;
                },
                "Derived must implement validate_list_integrity returning OperationResult"
            );

            auto [logged_user, logged_user_result] = plugins::core::find_logged_user(db, token);
            if (logged_user_result.ko()) return logged_user_result;
            RequestContext context{db, token, logged_user.role, logged_user.status};
            ////

            if (is_authorization_enabled(context))
            {
                auto def = db->get_model_definition(get_model_name());
                auto authorized_to = token.is_system() || is_authorized_to(
                    logged_user.role, g_configuration.access_mode, action, def->is_reader_can_write());
                if (!authorized_to)
                    return {
                        logged_user.role == essential::UserRole::Guest ? 401 : 403,
                        "You are not authorized to access resource. " + def->get_model_name() + " LIST"
                    };

                if (logged_user.role < essential::UserRole::Admin)
                    if (auto res = derived().validate_list_authorization(context, filter); !res.ok())
                        return res;
            }
            if (auto res = derived().validate_list_integrity(context, filter); !res.ok())
                return res;

            return ok_result;
        };
        [[nodiscard]] string get_model_name() const override = 0;

        void set_validator_func(GetValidatorFunc func) override { get_validator_func_ = func; }
        [[nodiscard]] GetValidatorFunc get_validator_func() const override { return get_validator_func_; }

    private:
        const Derived& derived() const { return static_cast<const Derived&>(*this); }
    };
}