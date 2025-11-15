#include "mindnet/http/HttpServer.hpp"

#include <unordered_set>
#include <utility>
#include <chrono>
#include <fstream>
#include <filesystem>

#include "mindnet/http/SuperAdminEndpointsGenerator.hpp"
#include "mindnet/essential/Configuration.hpp"
#include "mindnet/api/Service.hpp"
#include "mindnet/http/AuthEndpointsGenerator.hpp"
#include "mindnet/http/InfoHealthEndpointsGenerator.hpp"
#include "mindnet/http/ModelDefinitionEndpointsGenerator.hpp"
#include "mindnet/http/WebEndpointsGenerator.hpp"
#include "mindnet/plugins/core/CorePersistenceMethods.hpp"
#include "mindnet/util/Utils.hpp"


namespace mindnet::http
{
    using mindnet::essential::g_configuration;

    HttpServer::HttpServer(api::ServicePtr& service_ptr,
                           std::string directory_for_static_files_)
        : service_ptr_(service_ptr),
          directory_for_static_files(std::move(directory_for_static_files_))
    {
        create_web_endpoints(service_ptr);

        create_model_definition_endpoints(service_ptr);
        create_authentication_endpoints(service_ptr);
        create_superadmin_endpoints(service_ptr);
        create_info_health_endpoints(service_ptr);

        //READ
        CROW_ROUTE(crow_app, "/ok").methods(crow::HTTPMethod::GET)
        ([service_ptr](const crow::request& req)
        {
            return crow::response(200, "ok");
        });
    }

    void HttpServer::request_shutdown()
    {
        std::thread([this]
        {
            service_ptr_->schedule_shutdown();
            std::this_thread::sleep_for(std::chrono::seconds{15L});
            crow_app.stop();
            std::exit(0); // clean exit, systemd won't restart unless Restart=always
        }).detach();
    }

    void HttpServer::request_restart()
    {
        std::thread([this]
        {
            service_ptr_->schedule_restart();
            std::this_thread::sleep_for(std::chrono::seconds{15L});
            crow_app.stop();
            std::_Exit(42); // restart, systemd will handle
        }).detach();
    }

    void HttpServer::run(const string& host, int port, int frontend_port)
    {
        namespace fs = std::filesystem;


        //#define generate_js_files_to_js_txt
#ifdef generate_js_files_to_js_txt
        std::ofstream js_file(fs::path(directory_for_static_files) / "js.txt");
        std::vector<fs::path> js_files;
        for (const auto& entry : fs::directory_iterator(directory_for_static_files))
        {
            const auto& file_name = entry.path().filename().string();
            if (
                entry.path().extension() == ".js" &&
                !file_name.starts_with("app_") &&
                file_name != "markdown-it.min.js" &&
                file_name != "markdown-it-emoji.min.js" &&
                file_name != "highlight.min.js")
            {
                js_files.push_back(entry.path());
            }
        }
        std::sort(js_files.begin(), js_files.end());

        for (const auto& file : js_files)
        {
            js_file << "//" << file.filename().string() << ":" << std::endl;
            std::ifstream input(file, std::ios::binary);
            js_file << input.rdbuf() << std::endl;
        }
        ///
#endif

        //
        fs::path conf_js_path = fs::path(directory_for_static_files) / "conf.js";
        if (fs::exists(conf_js_path))
        {
            fs::remove(conf_js_path);
        }
        std::ofstream conf_js(conf_js_path);
        conf_js << "export const HOST = \"" << host << "\";" << std::endl;
        conf_js << "export const PORT = " << frontend_port << ";" << std::endl;
        conf_js.close();
        //crow_app.concurrency(1);
        crow_app.port(port).multithreaded().run();
    }

    // crow::SimpleApp& HttpServer::get_crow_app()
    // {
    //     return crow_app;
    // }


    void HttpServer::create_web_endpoints(const api::ServicePtr& service_ptr)
    {
        web_endpoints_generator_ = std::make_shared<WebEndpointsGenerator>(directory_for_static_files);
        web_endpoints_generator_->create_web_endpoints(service_ptr, crow_app);
    }

    void HttpServer::create_model_definition_endpoints(const api::ServicePtr& service_ptr)
    {
        ModelDefinitionEndpointsGenerator gen;
        gen.create_model_definition_endpoints(service_ptr, crow_app);
    }

    void HttpServer::create_info_health_endpoints(const api::ServicePtr& shared)
    {
        InfoHealthEndpointsGenerator gen;
        gen.create_info_health_endpoints(service_ptr_, crow_app);
    }

    void HttpServer::create_superadmin_endpoints(const api::ServicePtr& service_ptr)
    {
        SuperAdminEndpointsGenerator gen;
        gen.create_superadmin_endpoints(
            service_ptr,
            crow_app,
            [this] { request_restart(); },
            [this] { request_shutdown(); }
        );
    }

    void HttpServer::create_authentication_endpoints(api::ServicePtr& service_ptr)
    {
        AuthEndpointsGenerator gen;
        gen.create_auth_endpoints(
            service_ptr,
            crow_app
        );
    }
}
