//
// Created by robertvokac on 9/24/25.
//

#include "mindnet/http/InfoHealthEndpointsGenerator.h"

#include "mindnet/api/IService.h"
#include "mindnet/essential/Configuration.h"
#include "mindnet/essential/Version.h"
#include "mindnet/http/HttpUtils.h"
#include "mindnet/plugins/core/models/User.h"
#include "mindnet/util/Utils.h"

namespace mindnet::http
{
    using mindnet::essential::g_configuration;

    namespace Labels
    {
        static const std::string DAY = " day ";
        static const std::string DAYS = " days ";
        static const std::string HOUR = " hour ";
        static const std::string HOURS = " hours ";
        static const std::string MINUTE = " minute ";
        static const std::string MINUTES = " minutes ";
        static const std::string SECOND = " second";
        static const std::string SECONDS = " seconds";
        static const std::string MILLISECOND = " millisecond";
        static const std::string MILLISECONDS = " milliseconds";
    }

    void create_info_endpoint(
            const api::ServicePtr& service_ptr,
            crow::SimpleApp& crow_app
            )
    {

        //READ
        CROW_ROUTE(crow_app, "/info").methods(crow::HTTPMethod::GET)
        ([service_ptr](const crow::request& req)
        {
            check_maintenance_mode()

            nlohmann::ordered_json result;

            result["name"] = g_configuration.name;
            result["description"] = g_configuration.description;
            result["version"] = STRINGIFY(MIND_NET_VERSION);
            result["build_time"] = MIND_NET_BUILD_TIME;
            result["environment"] = environment_to_string(g_configuration.environment);
            //
            result["host"] = g_configuration.host;
            result["port"] = g_configuration.port;
            result["frontend_port"] = g_configuration.frontend_port;
            //
            result["access_mode"] = access_mode_to_string(g_configuration.access_mode);
            result["registration_mode"] = registration_mode_to_string(g_configuration.registration_mode);
            result["default_user_role"] = user_role_to_string(g_configuration.default_user_role);

            return crow::response(200, result.dump(2));
        });
    }

    void create_health_endpoint(
            const api::ServicePtr& service_ptr,
            crow::SimpleApp& crow_app
            )
    {
        //READ
        CROW_ROUTE(crow_app, "/health").methods(crow::HTTPMethod::GET)
        ([service_ptr](const crow::request& req)
        {
            auto print_duration = [](ll start_time, ll end_time, bool compact = false)
            {
                static const int MILLISECONDS_PER_SECOND = 1000;
                static const int MILLISECONDS_PER_MINUTE = 60 * MILLISECONDS_PER_SECOND;
                static const int MILLISECONDS_PER_HOUR = 60 * MILLISECONDS_PER_MINUTE;
                static const int MILLISECONDS_PER_DAY = 24 * MILLISECONDS_PER_HOUR;




                //#define test_health_endpoint
#ifdef test_health_endpoint
                std::random_device rd;
                std::mt19937 gen(rd());
                enum time_type
                {
                    seconds_ = 0,
                    minutes_ = 1,
                    hours_ = 2,
                    days_ = 3,
                    weeks_ = 4,
                    months_ = 5,
                    years_ = 6
                };

                std::uniform_int_distribution<> distrib0(0, 7);
                time_type random_time_type = static_cast<time_type>(distrib0(gen));
                int max = 0;
                switch (random_time_type)
                {
                case seconds_: max = 1 * 10;
                    break;
                case minutes_: max = 60 * 10;
                    break;
                case hours_: max = 3600 * 10;
                    break;
                case days_: max = 86400 * 10;
                    break;
                case weeks_: max = 86400 * 7 * 10;
                    break;
                case months_: max = 86400 * 30 * 10;
                    break;
                case years_: max = 86400 * 365 * 10;
                    break;
                default: max = 1000000;
                }
                std::uniform_int_distribution<> distrib(0, max);


                ll elapsed_seconds = end_time - start_time + distrib(gen);
#else

                ll elapsed_milliseconds = end_time - start_time;
#endif
                short days = 0;
                short hours = 0;
                short minutes = 0;
                short seconds = 0;
                short milliseconds = 0;

                days = elapsed_milliseconds / MILLISECONDS_PER_DAY;
                elapsed_milliseconds -= static_cast<ll>(days) * MILLISECONDS_PER_DAY;

                hours = elapsed_milliseconds / MILLISECONDS_PER_HOUR;
                elapsed_milliseconds -= static_cast<ll>(hours) * MILLISECONDS_PER_HOUR;

                minutes = elapsed_milliseconds / MILLISECONDS_PER_MINUTE;
                elapsed_milliseconds -= static_cast<ll>(minutes) * MILLISECONDS_PER_MINUTE;

                seconds = elapsed_milliseconds / MILLISECONDS_PER_SECOND;
                elapsed_milliseconds -= static_cast<ll>(seconds) * MILLISECONDS_PER_SECOND;

                milliseconds = static_cast<short>(elapsed_milliseconds);

                std::ostringstream oss;

                if (compact)
                {
                    if (days > 0) oss << days << "d ";
                    if (hours > 0) oss << hours << "h ";
                    if (minutes > 0) oss << minutes << "m ";
                    if (seconds > 0) oss << seconds << "s ";
                    if (milliseconds > 0) oss << milliseconds << "ms";
                    if (days == 0 && hours == 0 && minutes == 0 && seconds == 0 && milliseconds == 0)
                        oss << "0ms";
                }
                else
                {
                    if (days > 0)
                        oss << days << (days == 1 ? Labels::DAY : Labels::DAYS);
                    if (hours > 0)
                        oss << hours << (hours == 1 ? Labels::HOUR : Labels::HOURS);
                    if (minutes > 0)
                        oss << minutes << (minutes == 1 ? Labels::MINUTE : Labels::MINUTES);
                    if (seconds > 0)
                        oss << seconds << (seconds == 1 ? Labels::SECOND : Labels::SECONDS);
                    if (milliseconds > 0)
                        oss << milliseconds << (milliseconds == 1 ? Labels::MILLISECOND : Labels::MILLISECONDS);

                    if (days == 0 && hours == 0 && minutes == 0 && seconds == 0 && milliseconds == 0)
                        oss << "0 milliseconds";
                }


                return oss.str();
            };

            nlohmann::ordered_json result;

            auto now = util::Utils::current_unix_timestamp_ms();

            bool compact = false;
            if (req.url_params.get("compact"))
            {
                std::string val = req.url_params.get("compact");
                if (val == "1" || val == "true" || val == "yes")
                    compact = true;
            }

            result["status"] = g_configuration.access_mode == essential::AccessMode::MaintenanceMode || service_ptr->is_shutdown_scheduled() || service_ptr->is_restart_scheduled()
                                   ? "MAINTENANCE"
                                   : "UP";
            result["uptime"] = print_duration(essential::start_time, now, compact);
            result["timestamp"] = util::Utils::unixtime_to_string(now);
            result["started_at"] = util::Utils::unixtime_to_string(essential::start_time);

            return crow::response(200, result.dump(2));
        });
    }

    void InfoHealthEndpointsGenerator::create_info_health_endpoints(
        const api::ServicePtr& service_ptr,
        crow::SimpleApp& crow_app
    )
    {
        create_info_endpoint(service_ptr, crow_app);
        create_health_endpoint(service_ptr, crow_app);

    }
}
