//
// Created by robertvokac on 9/24/25.
//

#include "mindnet/http/CrowLoggerAdapter.h"
namespace mindnet::http
{

    void CrowLoggerAdapter::log(const std::string& message, CrowLogLevel level) {
        using namespace mindnet::essential;

        switch (level) {
        case CrowLogLevel::Debug:
            debug << "[Crow] " << message << commit;
            break;
        case CrowLogLevel::Info:
            info << "[Crow] " << message << commit;
            break;
        case CrowLogLevel::Warning:
            warn << "[Crow] " << message << commit;
            break;
        case CrowLogLevel::Error:
            err << "[Crow] " << message << commit;
            break;
        case CrowLogLevel::Critical:
            fatal << "[Crow] " << message << commit;
            break;
        }
    }
}