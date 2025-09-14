//
// Created by robertvokac on 9/8/25.
//
#ifndef REGISTRATIONMODE_H
#define REGISTRATIONMODE_H

#include <string>

namespace mindnet
{
    enum class RegistrationMode
    {
        Free = 0, // Anybody can register, account is active immediately.
        RequiresAdminApproval = 1, // User is registered, but must be approved by admin
        AdminAddsUsers = 2, // Endpoint /register is disabled. Only admin can add new users.
    };

    inline std::string registration_mode_to_string(RegistrationMode registration_mode)
    {
        switch (registration_mode)
        {
        case RegistrationMode::Free:
            return "Free";
        case RegistrationMode::RequiresAdminApproval:
            return "RequiresAdminApproval";
        case RegistrationMode::AdminAddsUsers:
            return "AdminAddsUsers";
        default:
            return "Unknown";
        }
    }

    inline std::string registration_mode_to_string(int registration_mode)
    {
        return registration_mode_to_string(static_cast<RegistrationMode>(registration_mode));
    }

    inline RegistrationMode string_to_registration_mode(const std::string& mode_str)
    {
        if (mode_str == "Free") return RegistrationMode::Free;
        if (mode_str == "RequiresAdminApproval") return RegistrationMode::RequiresAdminApproval;
        if (mode_str == "AdminAddsUsers") return RegistrationMode::AdminAddsUsers;
        throw std::runtime_error("Invalid registration mode: " + mode_str);
    }

    inline model::EnumDefinition registration_mode_to_enum_definition()
    {
        return model::EnumDefinition{
            registration_mode_to_string, 3, 0, 1, 2
        };
    }
}
#endif // REGISTRATIONMODE_H
