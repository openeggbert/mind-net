//
// Created by robertvokac on 9/8/25.
//
#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

#include <string>

namespace mindnet::essential
{
    enum class Environment
    {
        Production = 0,
        Staging = 1,
        Development = 2
    };

    std::string environment_to_string(const Environment& environment);

    std::string environment_to_string(int environment);

    Environment string_to_environment(const std::string& env_str);

    // model::EnumDefinition environment_to_enum_definition()
}
#endif // ENVIRONMENT_H
