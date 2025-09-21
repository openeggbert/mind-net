#include <gtest/gtest.h>
#include "mindnet/AccessMode.h"
#include "mindnet/api/RequestContext.h"
#include "mindnet/api/ValidatorBase.h"

using namespace mindnet;
using namespace mindnet::plugins::core::enums;

namespace mindnet
{
// Mock configuration
struct
{
    AccessMode access_mode;
} test_configuration;

    // Dummy persistence
    static std::shared_ptr<api::IPersistence> dummy_db;

    // Dummy login token
    static http::LoginToken dummy_token{"user", 1, "", 0};


// Helper RequestContext constructor
api::RequestContext make_ctx(UserRole role)
{
    std::string user = "user";
    std::string error;
    return api::RequestContext{dummy_db, dummy_token, role, UserStatus::Active};
}
} // namespace mindnet

//
// Tests for is_authorization_enabled
//
class AuthorizationEnabledTest : public ::testing::Test
{
};

TEST_F(AuthorizationEnabledTest, MaintenanceMode_AllRoles)
{
    mindnet::test_configuration.access_mode = AccessMode::MaintenanceMode;
    EXPECT_TRUE(is_authorization_enabled(mindnet::make_ctx(UserRole::Admin)));
    EXPECT_TRUE(is_authorization_enabled(mindnet::make_ctx(UserRole::Guest)));
}

TEST_F(AuthorizationEnabledTest, PublicFullAccess_AllRoles)
{
    mindnet::test_configuration.access_mode = AccessMode::PublicFullAccess;
    EXPECT_FALSE(is_authorization_enabled(mindnet::make_ctx(UserRole::Admin)));
    EXPECT_FALSE(is_authorization_enabled(mindnet::make_ctx(UserRole::Guest)));
}

TEST_F(AuthorizationEnabledTest, AuthenticatedFullAccess_AdminVsGuest)
{
    mindnet::test_configuration.access_mode = AccessMode::AuthenticatedFullAccess;
    EXPECT_FALSE(is_authorization_enabled(mindnet::make_ctx(UserRole::Admin)));
    EXPECT_FALSE(is_authorization_enabled(mindnet::make_ctx(UserRole::Reader)));
    EXPECT_TRUE(is_authorization_enabled(mindnet::make_ctx(UserRole::Guest)));
}

//
// Tests for is_authorized_to
//
class AuthorizedToTest : public ::testing::Test
{
};

TEST_F(AuthorizedToTest, AdminsReadOnly_Admin)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Admin, AccessMode::AdminsReadOnly, Crudl::Read));
    EXPECT_FALSE(is_authorized_to(UserRole::Admin, AccessMode::AdminsReadOnly, Crudl::Create));
}

TEST_F(AuthorizedToTest, AdminsReadWrite_Admin)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Admin, AccessMode::AdminsReadWrite, Crudl::Create));
    EXPECT_TRUE(is_authorized_to(UserRole::Admin, AccessMode::AdminsReadWrite, Crudl::Delete));
}

TEST_F(AuthorizedToTest, AuthenticatedReadOnly)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Editor, AccessMode::AuthenticatedReadOnly, Crudl::Read));
    EXPECT_FALSE(is_authorized_to(UserRole::Editor, AccessMode::AuthenticatedReadOnly, Crudl::Update));

    // Admin has an exception - can also Create
    EXPECT_TRUE(is_authorized_to(UserRole::Admin, AccessMode::AuthenticatedReadOnly, Crudl::Create));
}

TEST_F(AuthorizedToTest, AuthenticatedReadWrite)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Reviewer, AccessMode::AuthenticatedReadWrite, Crudl::Update));
    EXPECT_TRUE(is_authorized_to(UserRole::Editor, AccessMode::AuthenticatedReadWrite, Crudl::Delete));

    // Reader only has Read access
    EXPECT_TRUE(is_authorized_to(UserRole::Reader, AccessMode::AuthenticatedReadWrite, Crudl::Read));
    EXPECT_FALSE(is_authorized_to(UserRole::Reader, AccessMode::AuthenticatedReadWrite, Crudl::Update));
}

TEST_F(AuthorizedToTest, PublicReadOnlyAuthenticatedRO)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Guest, AccessMode::PublicReadOnlyAuthenticatedReadOnly, Crudl::Read));
    EXPECT_FALSE(is_authorized_to(UserRole::Guest, AccessMode::PublicReadOnlyAuthenticatedReadOnly, Crudl::Create));

    EXPECT_TRUE(is_authorized_to(UserRole::Editor, AccessMode::PublicReadOnlyAuthenticatedReadOnly, Crudl::Read));
    EXPECT_FALSE(is_authorized_to(UserRole::Editor, AccessMode::PublicReadOnlyAuthenticatedReadOnly, Crudl::Update));
}

TEST_F(AuthorizedToTest, PublicReadOnlyAuthenticatedRW)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Guest, AccessMode::PublicReadOnlyAuthenticatedReadWrite, Crudl::Read));
    EXPECT_FALSE(is_authorized_to(UserRole::Guest, AccessMode::PublicReadOnlyAuthenticatedReadWrite, Crudl::Update));

    EXPECT_TRUE(is_authorized_to(UserRole::Editor, AccessMode::PublicReadOnlyAuthenticatedReadWrite, Crudl::Update));
}

TEST_F(AuthorizedToTest, PublicFullAccess_AllRoles)
{
    EXPECT_TRUE(is_authorized_to(UserRole::Guest, AccessMode::PublicFullAccess, Crudl::Delete));
    EXPECT_TRUE(is_authorized_to(UserRole::Reader, AccessMode::PublicFullAccess, Crudl::Create));
}
