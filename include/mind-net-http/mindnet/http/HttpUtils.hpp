//
// Created by robertvokac on 8/16/25.
//

#ifndef MINI_WIKI_HTTPUTILS_H
#define MINI_WIKI_HTTPUTILS_H

#define check_maintenance_mode()\
if (\
                essential::g_configuration.access_mode == essential::AccessMode::MaintenanceMode ||\
                service_ptr->is_shutdown_scheduled() ||\
                service_ptr->is_restart_scheduled()\
                ) return\
                crow::response(503, "Maintenance Mode. Service Unavailable.");

#include "crow.h"
#include <string>

namespace mindnet::http
{
}
#endif //MINI_WIKI_HTTPUTILS_H
