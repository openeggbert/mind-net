//
// Created by robertvokac on 8/6/25.
//
#ifndef REQUESTCONTEXT_H
#define REQUESTCONTEXT_H

#include "mindnet/api/IPersistence.h"
#include "IRepository.h"
#include "mindnet/plugins/core/enums/UserStatus.h"
#include "mindnet/http/LoginToken.h"

namespace mindnet::api
{
    struct RequestContext
    {
        DbPtr& db;
        http::LoginToken& token;
        core::UserRole role{};
        plugins::core::enums::UserStatus status{};
    };
}
#endif // REQUESTCONTEXT_H
