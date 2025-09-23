//
// Created by robertvokac on 9/8/25.
//
#ifndef REGISTRATIONMODE_H
#define REGISTRATIONMODE_H

#include <string>

namespace mindnet::essential
{
    enum class RegistrationMode
    {
        Free = 0, // Anybody can register, account is active immediately.
        RequiresAdminApproval = 1, // User is registered, but must be approved by admin
        AdminAddsUsers = 2, // Endpoint /register is disabled. Only admin can add new users.
    };

    std::string registration_mode_to_string(RegistrationMode registration_mode);

    inline std::string registration_mode_to_string(int registration_mode)
    {
        return registration_mode_to_string(static_cast<RegistrationMode>(registration_mode));
    }

    RegistrationMode string_to_registration_mode(const std::string& mode_str);

    //model::EnumDefinition registration_mode_to_enum_definition();
}
#endif // REGISTRATIONMODE_H
