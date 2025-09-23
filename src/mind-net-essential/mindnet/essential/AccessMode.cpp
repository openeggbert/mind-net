//
// Created by robertvokac on 9/8/25.
//

#include "mindnet/essential/AccessMode.h"

namespace mindnet::essential
{
    using essential::Crudl;
    using essential::UserRole;

    bool is_access_mode_in(const AccessMode mode, const std::vector<AccessMode>& modes)
    {
        return std::find(modes.begin(), modes.end(), mode) != modes.end();
    }

    std::string access_mode_to_string(const AccessMode& access_mode)
    {
        switch (access_mode)
        {
        case AccessMode::MaintenanceMode:
            return "MaintenanceMode";
        case AccessMode::AdminsReadOnly:
            return "AdminsReadOnly";
        case AccessMode::AdminsReadWrite:
            return "AdminsReadWrite";
        case AccessMode::AuthenticatedReadOnly:
            return "AuthenticatedReadOnly";
        case AccessMode::AuthenticatedReadWrite:
            return "AuthenticatedReadWrite";
        case AccessMode::AuthenticatedFullAccess:
            return "AuthenticatedFullAccess";
        case AccessMode::PublicReadOnlyAuthenticatedReadOnly:
            return "PublicReadOnlyAuthenticatedReadOnly";
        case AccessMode::PublicReadOnlyAuthenticatedReadWrite:
            return "PublicReadOnlyAuthenticatedReadWrite";
        case AccessMode::PublicFullAccess:
            return "PublicFullAccess";
        default:
            return "Unknown";
        }
    }

    std::string access_mode_to_string(int access_mode)
    {
        return access_mode_to_string(static_cast<AccessMode>(access_mode));
    }

    AccessMode string_to_access_mode(const std::string& mode_str)
    {
        if (mode_str == "MaintenanceMode") return AccessMode::MaintenanceMode;
        if (mode_str == "AdminsReadOnly") return AccessMode::AdminsReadOnly;
        if (mode_str == "AdminsReadWrite") return AccessMode::AdminsReadWrite;
        if (mode_str == "AuthenticatedReadOnly") return AccessMode::AuthenticatedReadOnly;
        if (mode_str == "AuthenticatedReadWrite") return AccessMode::AuthenticatedReadWrite;
        if (mode_str == "AuthenticatedFullAccess") return AccessMode::AuthenticatedFullAccess;
        if (mode_str == "PublicReadOnlyAuthenticatedReadOnly") return AccessMode::PublicReadOnlyAuthenticatedReadOnly;
        if (mode_str == "PublicReadOnlyAuthenticatedReadWrite") return AccessMode::PublicReadOnlyAuthenticatedReadWrite;
        if (mode_str == "PublicFullAccess") return AccessMode::PublicFullAccess;
        throw std::runtime_error("Invalid access mode: " + mode_str);
    }

    // mindnet::model::EnumDefinition access_mode_to_enum_definition()
    // {
    //     return mindnet::model::EnumDefinition{
    //         access_mode_to_string, 9, 0, 1, 2, 3, 4, 5, 6, 7, 8
    //     };
    // }

    bool is_admin_authorized_to(
        const AccessMode mode,
        const Crudl action
    )
    {
        switch (mode)
        {
        case AccessMode::AdminsReadOnly: return is_read(action);
        case AccessMode::AdminsReadWrite: return true;
        case AccessMode::AuthenticatedReadOnly: return is_read(action);
        case AccessMode::AuthenticatedReadWrite: return true;
        case AccessMode::AuthenticatedFullAccess: return true;
        case AccessMode::PublicReadOnlyAuthenticatedReadWrite: return true;
        case AccessMode::PublicReadOnlyAuthenticatedReadOnly: return is_read(action);
        case AccessMode::PublicFullAccess: return true;
        default: throw std::runtime_error(std::string("Unknown access mode") + access_mode_to_string(mode));
        }
    }

    bool is_reviewer_authorized_to(
        const AccessMode mode,
        const Crudl action
    )
    {
        switch (mode)
        {
        case AccessMode::AdminsReadOnly: return false;
        case AccessMode::AdminsReadWrite: return false;
        case AccessMode::AuthenticatedReadOnly: return is_read(action);
        case AccessMode::AuthenticatedReadWrite: return true;
        case AccessMode::AuthenticatedFullAccess: return true;
        case AccessMode::PublicReadOnlyAuthenticatedReadWrite: return true;
        case AccessMode::PublicReadOnlyAuthenticatedReadOnly: return is_read(action);
        case AccessMode::PublicFullAccess: return true;
        default: throw std::runtime_error(std::string("Unknown access mode") + access_mode_to_string(mode));
        }
    }

    bool is_editor_authorized_to(
        const AccessMode mode,
        const Crudl action
    )
    {
        switch (mode)
        {
        case AccessMode::AdminsReadOnly: return false;
        case AccessMode::AdminsReadWrite: return false;
        case AccessMode::AuthenticatedReadOnly: return is_read(action);
        case AccessMode::AuthenticatedReadWrite: return true;
        case AccessMode::AuthenticatedFullAccess: return true;
        case AccessMode::PublicReadOnlyAuthenticatedReadWrite: return true;
        case AccessMode::PublicReadOnlyAuthenticatedReadOnly: return is_read(action);
        case AccessMode::PublicFullAccess: return true;
        default: throw std::runtime_error(std::string("Unknown access mode") + access_mode_to_string(mode));
        }
    }

    bool is_reader_authorized_to(
        const AccessMode mode,
        const Crudl action,
        bool reader_can_write
    )
    {
        bool is_read_action = is_read(action);
        switch (mode)
        {
        case AccessMode::AdminsReadOnly: return false;
        case AccessMode::AdminsReadWrite: return false;
        case AccessMode::AuthenticatedReadOnly: return is_read_action || reader_can_write;
        case AccessMode::AuthenticatedReadWrite: return is_read_action || reader_can_write;
        case AccessMode::AuthenticatedFullAccess: return true;
        case AccessMode::PublicReadOnlyAuthenticatedReadWrite: return is_read_action || reader_can_write;
        case AccessMode::PublicReadOnlyAuthenticatedReadOnly: return is_read_action || reader_can_write;
        case AccessMode::PublicFullAccess: return true;
        default: throw std::runtime_error(std::string("Unknown access mode") + access_mode_to_string(mode));
        }
    }

    bool is_guest_authorized_to(
        const AccessMode mode,
        const Crudl action
    )
    {
        switch (mode)
        {
        case AccessMode::AdminsReadOnly: return false;
        case AccessMode::AdminsReadWrite: return false;
        case AccessMode::AuthenticatedReadOnly: return false;
        case AccessMode::AuthenticatedReadWrite: return false;
        case AccessMode::AuthenticatedFullAccess: return false;
        case AccessMode::PublicReadOnlyAuthenticatedReadWrite: return is_read(action);
        case AccessMode::PublicReadOnlyAuthenticatedReadOnly: return is_read(action);
        case AccessMode::PublicFullAccess: return true;
        default: throw std::runtime_error(std::string("Unknown access mode") + access_mode_to_string(mode));
        }
    }

    bool is_authorized_to(
        const UserRole role,
        const AccessMode mode,
        const Crudl action,
        const bool reader_can_write
    )
    {
        if (mode == AccessMode::MaintenanceMode) return false;
        if (mode == AccessMode::PublicFullAccess) return true;
        if (role == UserRole::System) return true;

        bool authenticated = role > UserRole::Guest;

        if (!authenticated && is_authenticated_mode(mode)) return false;

        switch (role)
        {
        case UserRole::Admin: return is_admin_authorized_to(mode, action);
        case UserRole::Reviewer: return is_reviewer_authorized_to(mode, action);
        case UserRole::Editor: return is_editor_authorized_to(mode, action);
        case UserRole::Reader: return is_reader_authorized_to(mode, action, reader_can_write);
        case UserRole::Guest: return is_guest_authorized_to(mode, action);
        default: throw std::runtime_error(
                std::string("Unknown role ") + user_role_to_string(role));
        }
    }
}
