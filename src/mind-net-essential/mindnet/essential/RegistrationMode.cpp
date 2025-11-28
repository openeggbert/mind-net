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

#include "mindnet/essential/RegistrationMode.hpp"

#include <stdexcept>
#include <string>

namespace mindnet::essential
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