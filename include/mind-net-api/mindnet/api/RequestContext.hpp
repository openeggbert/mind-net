//
// Created by robertvokac on 8/6/25.
//
#ifndef REQUESTCONTEXT_H
#define REQUESTCONTEXT_H

#include "IPersistence.hpp"
#include "IRepository.hpp"
#include "mindnet/essential/UserStatus.hpp"
#include "AccessTokenContext.hpp"

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
