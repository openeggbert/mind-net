//
// Created by robertvokac on 8/16/25.
//

#ifndef MINI_WIKI_HTTPUTILS_H
#define MINI_WIKI_HTTPUTILS_H

#define check_maintenance_mode()\
if (mindnet::essential::g_configuration.access_mode == essential::AccessMode::MaintenanceMode)\
return crow::response(503, "Maintenance Mode. Service Unavailable.");

#include "crow.h"
#include <string>

namespace mindnet::http
{
}
#endif //MINI_WIKI_HTTPUTILS_H
