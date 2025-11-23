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
#include <gtest/gtest.h>
#include "mindnet/essential/AccessMode.hpp"
#include "mindnet/api/RequestContext.hpp"
#include "mindnet/api/ValidatorBase.hpp"


using namespace mindnet;

namespace mindnet::api
{


// Mock configuration


    // Dummy persistence
    static std::shared_ptr<api::IPersistence> dummy_db;

    // Dummy login token
    static api::AccessTokenContext dummy_token{1, "", 0};

    using mindnet::essential::UserRole;
// Helper RequestContext constructor
api::RequestContext make_ctx(UserRole role)
{
    std::string user = "user";
    std::string error;
    return api::RequestContext{dummy_db, dummy_token, role, essential::UserStatus::Active};
}
} // namespace mindnet

//
// Tests for is_authorization_enabled
//
class AuthorizationEnabledTest : public ::testing::Test
{
};

using mindnet::essential::AccessMode;
using api::is_authorization_enabled;
using mindnet::essential::UserRole;

TEST_F(AuthorizationEnabledTest, MaintenanceMode_AllRoles)
{
    essential::g_configuration.access_mode = AccessMode::MaintenanceMode;
    EXPECT_TRUE(is_authorization_enabled(mindnet::api::make_ctx(UserRole::Admin)));
    EXPECT_TRUE(is_authorization_enabled(mindnet::api::make_ctx(UserRole::Guest)));
}

TEST_F(AuthorizationEnabledTest, PublicFullAccess_AllRoles)
{
    essential::g_configuration.access_mode = AccessMode::PublicFullAccess;
    EXPECT_FALSE(is_authorization_enabled(mindnet::api::make_ctx(UserRole::Admin)));
    EXPECT_FALSE(is_authorization_enabled(mindnet::api::make_ctx(UserRole::Guest)));
}

TEST_F(AuthorizationEnabledTest, AuthenticatedFullAccess_AdminVsGuest)
{
    essential::g_configuration.access_mode = AccessMode::AuthenticatedFullAccess;
    EXPECT_FALSE(is_authorization_enabled(mindnet::api::make_ctx(UserRole::Admin)));
    EXPECT_FALSE(is_authorization_enabled(mindnet::api::make_ctx(UserRole::Reader)));
    EXPECT_TRUE(is_authorization_enabled(mindnet::api::make_ctx(UserRole::Guest)));
}

//
// Tests for is_authorized_to
//
class AuthorizedToTest : public ::testing::Test
{
};
using essential::Crudl;
TEST_F(AuthorizedToTest, AdminsReadOnly_Admin)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Admin, AccessMode::AdminsReadOnly, Crudl::Read, false));
    EXPECT_FALSE(is_authorized_to(UserRole::Admin, AccessMode::AdminsReadOnly, Crudl::Create, false));
}

TEST_F(AuthorizedToTest, AdminsReadWrite_Admin)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Admin, AccessMode::AdminsReadWrite, Crudl::Create, false));
    EXPECT_TRUE(is_authorized_to(UserRole::Admin, AccessMode::AdminsReadWrite, Crudl::Delete, false));
}

TEST_F(AuthorizedToTest, AuthenticatedReadOnly)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Editor, AccessMode::AuthenticatedReadOnly, Crudl::Read, false));
    EXPECT_FALSE(is_authorized_to(UserRole::Editor, AccessMode::AuthenticatedReadOnly, Crudl::Update, false));

    // Admin has an exception - can also Create
    EXPECT_FALSE(is_authorized_to(UserRole::Admin, AccessMode::AuthenticatedReadOnly, Crudl::Create, false));
}

TEST_F(AuthorizedToTest, AuthenticatedReadWrite)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Reviewer, AccessMode::AuthenticatedReadWrite, Crudl::Update, false));
    EXPECT_TRUE(is_authorized_to(UserRole::Editor, AccessMode::AuthenticatedReadWrite, Crudl::Delete, false));

    // Reader only has Read access
    EXPECT_TRUE(is_authorized_to(UserRole::Reader, AccessMode::AuthenticatedReadWrite, Crudl::Read, false));
    EXPECT_FALSE(is_authorized_to(UserRole::Reader, AccessMode::AuthenticatedReadWrite, Crudl::Update, false));
}

TEST_F(AuthorizedToTest, PublicReadOnlyAuthenticatedRO)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Guest, AccessMode::PublicReadOnlyAuthenticatedReadOnly, Crudl::Read, false));
    EXPECT_FALSE(is_authorized_to(UserRole::Guest, AccessMode::PublicReadOnlyAuthenticatedReadOnly, Crudl::Create, false));

    EXPECT_TRUE(is_authorized_to(UserRole::Editor, AccessMode::PublicReadOnlyAuthenticatedReadOnly, Crudl::Read, false));
    EXPECT_FALSE(is_authorized_to(UserRole::Editor, AccessMode::PublicReadOnlyAuthenticatedReadOnly, Crudl::Update, false));
}

TEST_F(AuthorizedToTest, PublicReadOnlyAuthenticatedRW)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Guest, AccessMode::PublicReadOnlyAuthenticatedReadWrite, Crudl::Read, false));
    EXPECT_FALSE(is_authorized_to(UserRole::Guest, AccessMode::PublicReadOnlyAuthenticatedReadWrite, Crudl::Update, false));

    EXPECT_TRUE(is_authorized_to(UserRole::Editor, AccessMode::PublicReadOnlyAuthenticatedReadWrite, Crudl::Update, false));
}

TEST_F(AuthorizedToTest, PublicFullAccess_AllRoles)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Guest, AccessMode::PublicFullAccess, Crudl::Delete, false));
    EXPECT_TRUE(is_authorized_to(UserRole::Reader, AccessMode::PublicFullAccess, Crudl::Create, false));
}
