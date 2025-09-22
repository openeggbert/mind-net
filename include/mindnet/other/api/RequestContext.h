//
// Created by robertvokac on 8/6/25.
//
#ifndef REQUESTCONTEXT_H
#define REQUESTCONTEXT_H

#include "../../other/api/IPersistence.h"
#include "IRepository.h"
#include "../../other/plugins/core/enums/UserStatus.h"
#include "mindnet/other/http/LoginToken.h"

namespace mindnet::api
{
    struct RequestContext
    {
        DbPtr& db;
        http::LoginToken& token;
        plugins::core::enums::UserRole role{};
        plugins::core::enums::UserStatus status{};
    };
}
#endif // REQUESTCONTEXT_H
