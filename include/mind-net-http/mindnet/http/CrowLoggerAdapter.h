#ifndef CROWLOGGERADAPTER_H
#define CROWLOGGERADAPTER_H
#include "crow.h"
#include "mindnet/essential/Logger.h"
#include "mindnet/essential/ConsolePrinter.h"
#include "mindnet/essential/Global.h"

namespace mindnet::http {
class CrowLoggerAdapter : public crow::ILogHandler {
    typedef crow::LogLevel CrowLogLevel;

public:
    ~CrowLoggerAdapter() override = default;
    void log(const std::string& message, CrowLogLevel level) override;
};

}
#endif // CROWLOGGERADAPTER_H
