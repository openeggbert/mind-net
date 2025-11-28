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

#include "mindnet/http/SuperAdminEndpointsGenerator.hpp"

#include "mindnet/api/IService.hpp"
#include "mindnet/essential/Configuration.hpp"
#include "mindnet/plugins/core/models/SuperAdminLog.hpp"
#include "mindnet/plugins/core/models/User.hpp"
#include "mindnet/essential/Global.hpp"
#include "mindnet/util/Utils.hpp"
#include <jemalloc/jemalloc.h>

namespace mindnet::http
{
    constexpr auto configure_get_template = FMT_STRING(R"(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Mind Net Configuration</title>
  <link rel="icon" type="image/png" href="../../../web/favicon.png">
  <style>
    body {{
      font-family: Arial, sans-serif;
      max-width: 800px;
      margin: 2rem auto;
      background: #f9f9f9;
      padding: 2rem;
      border-radius: 12px;
      box-shadow: 0 0 10px rgba(0,0,0,0.1);
    }}
    h1 {{ text-align: center; }}
    label {{ display: block; margin-top: 1rem; font-weight: bold; }}

input[type="text"],
input[type="number"],
input[type="password"],
select, textarea {{
      width: 100%;
      padding: 0.5rem;
      margin-top: 0.3rem;
      border: 1px solid #ccc;
      border-radius: 6px;
    }}
    button {{
      margin-top: 2rem;
      padding: 0.7rem 1.5rem;
      background: #4CAF50;
      color: white;
      border: none;
      border-radius: 6px;
      cursor: pointer;
      font-size: 1rem;
    }}
    button:hover {{ background: #45a049; }}

input[type="checkbox"] {{
  width: auto;
  transform: scale(1.5);
  margin-right: 0.5rem;
}}

label.checkbox {{
  display: flex;
  align-items: center;
  font-weight: bold;
  margin-top: 1rem;
}}

label.checkbox input[type="checkbox"] {{
  transform: scale(1.5);
  margin-right: 1rem;
}}

.restart_needed{{
color:orange;
font-weight: normal;
}}
.restart_needed:before{{
content: " (takes effect after restart) ";
}}

.toast {{
  font-size:150%;
  position: fixed;
  bottom: 20px;
  right: 20px;
  background: #9F9;
  color: #333;
  padding: 12px 20px;
  border-radius: 8px;
  box-shadow: 0 2px 6px rgba(0,0,0,0.3);
  opacity: 0;
  transform: translateY(20px);
  transition: opacity 0.3s ease, transform 0.3s ease;
  z-index: 1000;
}}
.toast.show {{
  opacity: 1;
  transform: translateY(0);
}}

  </style>
</head>
<body>
  <h1><a href="configure">Mind Net Configuration</a></h1>
  <i><a href="../web">Mind Net</a></i>

  <form id="configForm">
    <!-- Identification -->
    <label for="name">Name</label>
    <input type="text" id="name" name="name" value="{name}">

    <label for="description">Description</label>
    <textarea id="description" name="description">{description}</textarea>

    <label for="environment">Environment</label>
    <select id="environment" name="environment">
      {environment}
    </select>

    <label for="host">Host<span class="restart_needed"></span></label>
    <input type="text" id="host" name="host" value="{host}">

    <label for="port">Port<span class="restart_needed"></span></label>
    <input type="number" id="port" name="port" min="1" max="65535" value="{port}">

    <label for="frontend_port">Frontend Port<span class="restart_needed"></span></label>
    <input type="number" id="frontend_port" name="frontend_port" min="1" max="65535" value="{frontend_port}">

    <label for="database_type">Database Type<span class="restart_needed"></span></label>
    <select id="database_type" name="database_type">
      {database_type}
    </select>

    <!-- Access -->
    <label for="access_mode">Access Mode</label>
    <select id="access_mode" name="access_mode">
      {access_mode}
    </select>

    <label for="registration_mode">Registration Mode</label>
    <select id="registration_mode" name="registration_mode">
      {registration_mode}
    </select>

    <label for="default_user_role">Default User Role</label>
    <select id="default_user_role" name="default_user_role">
      {default_user_role}
    </select>

    <!-- Database -->
    <label for="db_host">Database Host<span class="restart_needed"></span></label>
    <input type="text" id="db_host" name="db_host" value="{db_host}">

    <label for="db_port">Database Port<span class="restart_needed"></span></label>
    <input type="number" id="db_port" name="db_port" min="1" max="65535" value="{db_port}">

    <label for="db_name">Database Name<span class="restart_needed"></span></label>
    <input type="text" id="db_name" name="db_name" value="{db_name}">

    <label for="db_user">Database User<span class="restart_needed"></span></label>
    <input type="text" id="db_user" name="db_user" value="{db_user}">

    <label for="db_password">Database Password<span class="restart_needed"></span></label>
    <input type="password" id="db_password" name="db_password" value="{db_password}">

    <!-- Other -->
    <label for="max_log_level">Max Log Level</label>
    <select id="max_log_level" name="max_log_level">
      {max_log_level}
    </select>

    <label for="allowed_plugins">Allowed Plugins (comma separated)<span class="restart_needed"></span></label>
    <input type="text" id="allowed_plugins" name="allowed_plugins" value="{allowed_plugins}">

    <label for="access_token_expires_in">Access token expires in (minutes)</label>
    <input type="number" id="access_token_expires_in" name="access_token_expires_in" value="{access_token_expires_in}" min="5" max="43200">

    <label for="refresh_token_expires_in">Refresh token expires in (minutes)</label>
    <input type="number" id="refresh_token_expires_in" name="refresh_token_expires_in" value="{refresh_token_expires_in}" min="1440" max="432000">

    <label for="refresh_token_rotation_threshold_in">Refresh token rotation threshold (minutes)</label>
    <input type="number" id="refresh_token_expires_in" name="refresh_token_rotation_threshold_in" value="{refresh_token_rotation_threshold_in}" min="60" max="432000">

    <label for="read_cache_capacity_size">Read cache capacity count (max)label>
    <input type="number" id="read_cache_capacity_size" name="read_cache_capacity_size" value="{read_cache_capacity_size}" min="60" max="432000">

    <label for="read_cache_capacity_bytes">Read cache capacity in bytes (max)</label>
    <input type="number" id="read_cache_capacity_bytes" name="read_cache_capacity_bytes" value="{read_cache_capacity_bytes}" min="60" max="432000">

    <label class="checkbox">
    <input type="checkbox" id="schedule_restart" name="schedule_restart">
    Schedule restart
    </label>

<button type="submit">Save Configuration</button>
  </form>
<script>
console.log("Script parsed OK");

function showToast(message, timeout = 5000) {{
  const toast = document.createElement("div");
  toast.className = "toast";
  toast.textContent = message;
  document.body.appendChild(toast);
  requestAnimationFrame(() => toast.classList.add("show"));
  setTimeout(() => {{
    toast.classList.remove("show");
    setTimeout(() => toast.remove(), 300);
  }}, timeout);
}}

window.addEventListener("load", () => {{
  requestAnimationFrame(() => {{
    requestAnimationFrame(() => {{
      const params = new URLSearchParams(window.location.search);
      if (params.has("message")) {{
        showToast(params.get("message"));
      }}
    }});
  }});
}});

console.log("JS loaded");

const form = document.getElementById("configForm");
if (!form) {{
  console.error("Form #configForm not found!");
}} else {{
  console.log("submit handler attached");

  form.addEventListener("submit", async e => {{
    e.preventDefault();
    console.log("submit intercepted");

    const formData = new FormData(form);

    const scheduleRestart = document.getElementById("schedule_restart");
    if (scheduleRestart.checked) {{
      const confirmed = confirm("Restart is scheduled. Do you really want to save?");
      if (!confirmed) return;
    }}

    const res = await fetch("/api/v1/superadmin/configure", {{
      method: "POST",
      headers: {{
        "Authorization": `Bearer ${{localStorage.getItem("access_token")}}`
      }},
      body: new URLSearchParams(formData)
    }});

    if (!res.ok) {{
      const t = await res.text();
      alert("Failed: " + t);
      return;
    }}

    window.location.href = "/api/v1/superadmin/configure?message=Changes%20were%20saved";
  }});
}}

</script>

</body>
</html>
)");

    inline std::optional<crow::response> require_superadmin(const plugins::core::models::User& user)
    {
        if (user.role < essential::UserRole::SuperAdmin)
        {
            return crow::response(403, "Forbidden: only SuperAdmin can perform this action");
        }
        return std::nullopt;
    }

    using UserOrResponse = std::variant<plugins::core::models::User, crow::response>;

    inline UserOrResponse
    load_current_user(const crow::request& req, const api::ServicePtr& service_ptr)
    {
        api::AccessTokenContext login_token{req, service_ptr};
        if (login_token.user_id == 0)
        {
            return crow::response(401, "Unauthorized users cannot access this resource");
        }
        auto user_values = service_ptr->read(
            plugins::core::models::USER_DEFINITION,
            login_token,
            login_token.user_id, 0
        );

        if (user_values.second.ko())
        {
            return crow::response(500, "Loading user role failed. " + user_values.second.error);
        }

        plugins::core::models::User user;
        user.from_values(user_values.first);
        return user;
    }

    std::string url_decode(const std::string& in)
    {
        std::string out;
        out.reserve(in.size());

        for (size_t i = 0; i < in.size(); ++i)
        {
            if (in[i] == '%')
            {
                if (i + 2 < in.size())
                {
                    std::string hex = in.substr(i + 1, 2);
                    char ch = static_cast<char>(std::stoi(hex, nullptr, 16));
                    out.push_back(ch);
                    i += 2;
                }
            }
            else if (in[i] == '+')
            {
                out.push_back(' ');
            }
            else
            {
                out.push_back(in[i]);
            }
        }
        return out;
    }

    std::unordered_map<std::string, std::string> parse_urlencoded(const std::string& body)
    {
        std::unordered_map<std::string, std::string> params;
        std::istringstream ss(body);
        std::string token;
        while (std::getline(ss, token, '&'))
        {
            auto pos = token.find('=');
            if (pos != std::string::npos)
            {
                auto key = token.substr(0, pos);
                auto val = token.substr(pos + 1);
                params[key] = url_decode(val);
            }
        }
        return params;
    }

#define assert_super_admin() \
    plugins::core::models::User user; \
    { \
        api::AccessTokenContext ctx{req, service_ptr}; \
        if (ctx.status != 200 || ctx.user_id == 0) { \
            crow::response resp{401, "Unauthorized: invalid or missing token"}; \
            api::AccessTokenContext system_token{0, "system", resp.code}; \
            log_request(service_ptr, req, system_token, resp.code, "Unauthorized access", resp.body); \
            return resp; \
        } \
        auto user_values = service_ptr->read( \
            plugins::core::models::USER_DEFINITION, \
            ctx, \
            ctx.user_id, 0 \
        ); \
        if (user_values.second.ko()) { \
            crow::response resp{500, "Failed to load user: " + user_values.second.error}; \
            api::AccessTokenContext system_token{0, "system", resp.code}; \
            log_request(service_ptr, req, system_token, resp.code, "User load failed", resp.body); \
            return resp; \
        } \
        if (user_values.first.empty()) { \
            crow::response resp{404, "User not found"}; \
            api::AccessTokenContext system_token{0, "system", resp.code}; \
            log_request(service_ptr, req, system_token, resp.code, "User not found", resp.body); \
            return resp; \
        } \
        user.from_values(user_values.first); \
        if (user.role < essential::UserRole::SuperAdmin) { \
            crow::response resp{403, "Forbidden: only SuperAdmin can perform this action"}; \
            api::AccessTokenContext system_token{0, "system", resp.code}; \
            log_request(service_ptr, req, system_token, resp.code, "Forbidden access", resp.body); \
            return resp; \
        } \
    }

    void SuperAdminEndpointsGenerator::create_superadmin_endpoints(
        const api::ServicePtr& service_ptr,
        crow::SimpleApp& crow_app,
        procedure_ptr request_restart,
        procedure_ptr request_shutdown
    )
    {
        // CROW_ROUTE(crow_app, "/api/v1/superadmin/purge")
        // ([]()
        // {
        //     purge_all_arenas();
        //     return crow::response(200, "OK");
        // });

        auto log_request = [](
            const api::ServicePtr& service_ptr,
            const crow::request& req,
            api::AccessTokenContext& login_token,
            int status_code,
            const std::string& action,
            const std::string& error = "",
            const std::string& diff = ""
        )
        {
            auto log_object = plugins::core::models::super_admin_log_from_crow_request(
                req,
                login_token.user_id,
                status_code,
                action,
                error,
                diff);
            auto log = log_object.to_values();
            int64_t now = static_cast<int64_t>(mindnet::util::Utils::current_unix_timestamp_ms());
            log[1] = now;
            log[2] = now;

            if (service_ptr == nullptr)
            {
                essential::warn <<
                    "Saving record to the table super_admin_log failed for this reason: service_ptr == nullptr " <<
                    log_object.to_json() << essential::commit;
                return;
            }

            auto result = service_ptr->create(
                plugins::core::models::SUPER_ADMIN_LOG_DEFINITION,
                login_token,
                log, 0);
            if (result.second.ko())
            {
                essential::warn << "Saving record to the table super_admin_log failed for this reason: " << result.
                    second.error <<
                    log_object.to_json() << essential::commit;
            }
        };

        CROW_ROUTE(crow_app, "/api/v1/superadmin/shutdown").methods("POST"_method)
        ([this, &service_ptr, request_shutdown, &log_request](const crow::request& req)
        {
            assert_super_admin()

            api::AccessTokenContext login_token{req, service_ptr};
            log_request(service_ptr, req, login_token, 200, "Shutdown scheduled");

            request_shutdown();
            return crow::response(200, "Shutdown scheduled");;
        });

        CROW_ROUTE(crow_app, "/api/v1/superadmin/restart").methods("POST"_method)
        ([this, &service_ptr, request_restart, &log_request](const crow::request& req)
        {
            assert_super_admin()

            api::AccessTokenContext login_token{req, service_ptr};
            log_request(service_ptr, req, login_token, 200, "Restart scheduled");

            request_restart();
            return crow::response(200, "Restart scheduled");;
        });

        CROW_ROUTE(crow_app, "/api/v1/superadmin/configure").methods("GET"_method)
        ([this, &service_ptr, &log_request](const crow::request& req)
        {
            auto params = req.url_params; // crow::query_string

            if (params.get("message") == nullptr)
            {
                assert_super_admin()
            }
            else
            {
                string msg = params.get("message");
                return crow::response(200, R"(
<!DOCTYPE html>
<html lang="en">
<head>
  <meta charset="UTF-8">
  <title>Mind Net Configuration</title>
  <link rel="icon" type="image/png" href="../../../web/favicon.png">
  <style>
    body {
      font-family: Arial, sans-serif;
      max-width: 800px;
      margin: 2rem auto;
      background: #f9f9f9;
      padding: 2rem;
      border-radius: 12px;
      box-shadow: 0 0 10px rgba(0,0,0,0.1);
    }
    h1 { text-align: center; }

.toast {
  font-size:150%;
  position: fixed;
  bottom: 20px;
  right: 20px;
  background: #9F9;
  color: #333;
  padding: 12px 20px;
  border-radius: 8px;
  box-shadow: 0 2px 6px rgba(0,0,0,0.3);
  opacity: 0;
  transform: translateY(20px);
  transition: opacity 0.3s ease, transform 0.3s ease;
  z-index: 1000;
}
.toast.show {
  opacity: 1;
  transform: translateY(0);
}

  </style>
</head>
<body>
  <h1><a href="configure">Mind Net Configuration</a></h1>
  <a style="padding:5px; border:2px solid grey; background:#ddd;text-align:center;" href="/web">Go to Main Menu</a>

<script>

function showToast(message, timeout = 5000) {
  const toast = document.createElement("div");
  toast.className = "toast";
  toast.textContent = message;
  document.body.appendChild(toast);
  requestAnimationFrame(() => toast.classList.add("show"));
  setTimeout(() => {
    toast.classList.remove("show");
    setTimeout(() => toast.remove(), 300);
  }, timeout);
}

window.addEventListener("load", () => {
  requestAnimationFrame(() => {
    requestAnimationFrame(() => {
      const params = new URLSearchParams(window.location.search);
      if (params.has("message")) {
        showToast(params.get("message"));
      }
    });
  });
});

showToast(")"
                                      + msg +
                                      R"(           ");
</script>

</body>
</html>)"

                );
            }

            api::AccessTokenContext login_token{req, service_ptr};
            log_request(service_ptr, req, login_token, 200, "Configure GET");

            return crow::response(200, fmt::vformat(configure_get_template,
                                                    mindnet::essential::g_configuration.to_fmt_store()));
        });

        CROW_ROUTE(crow_app, "/api/v1/superadmin/configure").methods("POST"_method)
        ([this, &service_ptr, request_restart, &log_request](const crow::request& req)
        {
            assert_super_admin()

            string_map new_configuration_map;
            auto params = parse_urlencoded(req.body);

            for (const auto& key : params | std::views::keys)
            {
                const auto& value = params[key];
                new_configuration_map.insert({key, value});
            }

            // auto read_configuration = []()
            // {
            //     std::string text;
            //     std::string line;
            //     std::ifstream mindnet_properties_file("mindnet.properties");
            //
            //     while (getline(mindnet_properties_file, line))
            //     {
            //         text += line;
            //         text += '\n';
            //     }
            //
            //     mindnet_properties_file.close();
            //     return text;
            // };

            auto old_string_map = essential::load_mind_net_properties("mindnet.properties");


            // std::string old_value = read_configuration();
            essential::Configuration new_configuration = essential::Configuration(new_configuration_map);;
            std::string validation_result = new_configuration.validate();
            if (!validation_result.empty())
            {
                // std::string error;
                // error.reserve(validation_result.size());
                // for (char c : validation_result)
                // {
                //     if (c == ' ')
                //         error += "%20";
                //     else
                //         error += c;
                // }

                return crow::response(400, validation_result);
            }
            essential::g_configuration = new_configuration;
            essential::g_configuration.save_mind_net_properties();
            // std::string new_value = read_configuration();
            auto new_string_map = essential::load_mind_net_properties("mindnet.properties");

            api::AccessTokenContext login_token{req, service_ptr};
            auto diff_maps_ = mindnet::util::Utils::diff_maps(old_string_map, new_string_map);
            auto diff_ = mindnet::util::Utils::diff_maps_to_string(diff_maps_);
            log_request(
                service_ptr,
                req,
                login_token,
                303,
                "Configure POST",
                "",
                diff_
            );

            if (params.contains("schedule_restart"))
            {
                request_restart();
            }
            crow::response res;
            res.code = 303;
            res.set_header("Location", "/api/v1/superadmin/configure?message=Changes%20were%20saved");
            return res;
        });
    }
}