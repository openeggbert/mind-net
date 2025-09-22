//
// Created by robertvokac on 9/8/25.
//

#include "mindnet/core/RegistrationMode.h"

#include <stdexcept>
#include <string>

namespace mindnet::core
{

    std::string registration_mode_to_string(RegistrationMode registration_mode)
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

    RegistrationMode string_to_registration_mode(const std::string& mode_str)
    {
        if (mode_str == "Free") return RegistrationMode::Free;
        if (mode_str == "RequiresAdminApproval") return RegistrationMode::RequiresAdminApproval;
        if (mode_str == "AdminAddsUsers") return RegistrationMode::AdminAddsUsers;
        throw std::runtime_error("Invalid registration mode: " + mode_str);
    }

    // model::EnumDefinition registration_mode_to_enum_definition()
    // {
    //     return model::EnumDefinition{
    //         registration_mode_to_string, 3, 0, 1, 2
    //     };
    // }
}
