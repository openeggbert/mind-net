//
// Created by robertvokac on 9/2/25.
//
#ifndef USERCRUDLVALIDATOR_H
#define USERCRUDLVALIDATOR_H


#include <memory>

#include "mindnet/Helper.h"
#include "mindnet/persistence/api/CrudlValidator.h"
#include "mindnet/persistence/api/CrudlValidatorBase.h"

namespace mindnet::persistence
{
    class Persistence;
}

namespace mindnet::persistence::impl::sqlite::validators
{
    using db_ = mindnet::persistence::Persistence*;
    using api::ValidatorContext;
    using models::User;

    class UserCrudlValidator : public api::CrudlValidatorBase<UserCrudlValidator, models::User>
    {
    public:
        UserCrudlValidator() = default;
        ~UserCrudlValidator() = default; // explicitly make it destructible

        using Model = User;


        api::result_t validate_create(const ValidatorContext&, const User&) const ;
        api::result_t validate_read(const ValidatorContext&, const User&) ;
        api::result_t validate_update(const ValidatorContext&, const User& new_entity,  User& old_entity) const;
        api::result_t validate_delete(const ValidatorContext&, const User&) ;
        api::result_t validate_list(const ValidatorContext&, const string_map&) ;

        [[nodiscard]] string get_model_name() const;


        //create_h_methods()

    };

}

#endif // USERCRUDLVALIDATOR_H
