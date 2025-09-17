//
// Created by robertvokac on 9/8/25.
//
#ifndef ACCESSMODE_H
#define ACCESSMODE_H


namespace mindnet
{
    enum class AccessMode
    {
        AuthenticatedOnly = 0, // All requests require JWT
        UnauthenticatedCanRead = 1, // GET is public, others require JWT
        EveryoneCanDoEverything = 2,
        // No authentication at all. New user everybody is created and used for this purpose
        MaintenanceMode = 10 // API is disabled, only admins can access
    };

    inline std::string access_mode_to_string(AccessMode access_mode)
    {
        switch (access_mode)
        {
        case AccessMode::AuthenticatedOnly:
            return "AuthenticatedOnly";
        case AccessMode::UnauthenticatedCanRead:
            return "UnauthenticatedCanRead";
        case AccessMode::EveryoneCanDoEverything:
            return "EveryoneCanDoEverything";
        case AccessMode::MaintenanceMode:
            return "MaintenanceMode";
        default:
            return "Unknown";
        }
    }

    inline std::string access_mode_to_string(int access_mode)
    {
        return access_mode_to_string(static_cast<AccessMode>(access_mode));
    }

    inline AccessMode string_to_access_mode(const std::string& mode_str)
    {
        if (mode_str == "AuthenticatedOnly") return AccessMode::AuthenticatedOnly;
        if (mode_str == "UnauthenticatedCanRead") return AccessMode::UnauthenticatedCanRead;
        if (mode_str == "EveryoneCanDoEverything") return AccessMode::EveryoneCanDoEverything;
        if (mode_str == "MaintenanceMode") return AccessMode::MaintenanceMode;
        throw std::runtime_error("Invalid access mode: " + mode_str);
    }

    inline model::EnumDefinition access_mode_to_enum_definition()
    {
        return model::EnumDefinition{
            access_mode_to_string, 4, 0, 1, 2, 10
        };
    }
}
#endif // ACCESSMODE_H
