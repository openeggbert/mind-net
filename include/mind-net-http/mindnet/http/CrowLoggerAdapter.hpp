#pragma once

#include "crow.h"
#include "mindnet/essential/Logger.hpp"
#include "mindnet/essential/ConsolePrinter.hpp"
#include "mindnet/essential/Global.hpp"

namespace mindnet::http
{
    class CrowLoggerAdapter : public crow::ILogHandler
    {
        typedef crow::LogLevel CrowLogLevel;

    public:
        ~CrowLoggerAdapter() override = default;
        void log(const std::string& message, CrowLogLevel level) override;
    };
}
