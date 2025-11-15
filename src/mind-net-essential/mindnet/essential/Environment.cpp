//
// Created by robertvokac on 9/8/25.
//

#include "mindnet/essential/Environment.hpp"

#include <stdexcept>
#include <string>

namespace mindnet::essential
{
    std::string environment_to_string(const Environment& environment)
    {
        switch (environment)
        {
        case Environment::Production:
            return "Production";
        case Environment::Staging:
            return "Staging";
        case Environment::Development:
            return "Development";
        default:
            return "Unknown";
        }
    }

    std::string environment_to_string(int environment)
    {
        return environment_to_string(static_cast<Environment>(environment));
    }

    Environment string_to_environment(const std::string& env_str)
    {
        if (env_str == "Production") return Environment::Production;
        if (env_str == "Staging") return Environment::Staging;
        if (env_str == "Development") return Environment::Development;
        throw std::runtime_error("Invalid environment: " + env_str);
    }

    // model::EnumDefinition environment_to_enum_definition()
    // {
    //     return model::EnumDefinition{
    //         environment_to_string, 3, 0, 1, 2
    //     };
    // }
}
