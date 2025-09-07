//
// Created by robertvokac on 8/6/25.
//
#ifndef REQUESTCONTEXT_H
#define REQUESTCONTEXT_H


#include "IPersistence.h"
#include "IRepository.h"
#include "mindnet/enums/UserStatus.h"
#include "mindnet/http/LoginToken.h"

namespace mindnet::persistence::api
{
    struct RequestContext
    {
        IPersistence* db;
        http::LoginToken& token;
        enums::UserRole role{};
        enums::UserStatus status{};

    };
}
#endif // REQUESTCONTEXT_H
