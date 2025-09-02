//
// Created by robertvokac on 9/2/25.
//

#ifndef MIND_NET_CRUDLVALIDATOR_H
#define MIND_NET_CRUDLVALIDATOR_H

namespace mindnet::persistence::api{

class CrudlValidator
{
    virtual bool can_create(const User& user, const Entity& entity) const = 0;
    virtual bool can_read(const User& user, const Entity& entity) const = 0;
    virtual bool can_update(const User& user, const Entity& entity) const = 0;
    virtual bool can_delete(const User& user, const Entity& entity) const = 0;
    virtual bool can_list(const User& user, const Entity& entity) const = 0;

    // Vrací podmínky pro SQL WHERE filtraci při listování
    virtual std::string getListFilter(const User& user) const = 0;

    virtual ~CrudlValidator() = default;
};
}


#endif //MIND_NET_CRUDLVALIDATOR_H