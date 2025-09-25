//
// Created by robertvokac on 8/6/25.
//
#ifndef REQUESTCONTEXT_H
#define REQUESTCONTEXT_H

#include "IPersistence.h"
#include "IRepository.h"
#include "mindnet/essential/UserStatus.h"
#include "AccessTokenContext.h"

namespace mindnet::api
{
    struct RequestContext
    {
        DbPtr& db;
        api::AccessTokenContext& token;
        essential::UserRole role{};
        essential::UserStatus status{};
    };
}
#endif // REQUESTCONTEXT_H
