//
// Created by robertvokac on 9/24/25.
//

#include "../../../../include/mind-net-http/mindnet/http/SuperAdminEndpointsGenerator.h"

#include "mindnet/api/IService.h"
#include "mindnet/essential/Configuration.h"
#include "mindnet/plugins/core/models/User.h"

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
  <h1><a href="configure">MindNet Configuration</a></h1>
  <form method="post" id="configForm">
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

    <!-- Secrets -->
    <label for="jwt_secret">JWT Secret</label>
    <input type="password" id="jwt_secret" name="jwt_secret" value="{jwt_secret}">

    <!-- Other -->
    <label for="max_log_level">Max Log Level</label>
    <select id="max_log_level" name="max_log_level">
      {max_log_level}
    </select>

    <label for="allowed_plugins">Allowed Plugins (comma separated)<span class="restart_needed"></span></label>
    <input type="text" id="allowed_plugins" name="allowed_plugins" value="{allowed_plugins}">

    <label class="checkbox">
    <input type="checkbox" id="schedule_restart" name="schedule_restart">
    Schedule restart
    </label>


    <button type="submit">Save Configuration</button>
  </form>
<script>
function showToast(message, timeout = 10000) {{
  const toast = document.createElement("div");
  toast.className = "toast";
  toast.textContent = message;
  document.body.appendChild(toast);

  // small delay because of animation
  requestAnimationFrame(() => toast.classList.add("show"));

  setTimeout(() => {{
    toast.classList.remove("show");
    setTimeout(() => toast.remove(), 300); // wait for animation
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

  document.getElementById("configForm").addEventListener("submit", function(event) {{
    const scheduleRestart = document.getElementById("schedule_restart");
    if (scheduleRestart.checked) {{
      const confirmed = confirm("Restart is scheduled. Do you really want to save?");
      if (!confirmed) {{
        event.preventDefault(); // prevents form submission
      }}
    }}
  }});

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
        api::LoginToken login_token{req};
        if (login_token.user_id == 0)
        {
            return crow::response(401, "Unauthorized users cannot access this resource");
        }
        auto user_values = service_ptr->read(
            plugins::core::models::USER_DEFINITION,
            login_token,
            login_token.user_id
        );

        if (user_values.second.ko())
        {
            return crow::response(500, "Loading user role failed. " + user_values.second.error);
        }

        plugins::core::models::User user;
        user.from_values(user_values.first);
        return user;
    }

    std::string url_decode(const std::string& in) {
        std::string out;
        out.reserve(in.size());

        for (size_t i = 0; i < in.size(); ++i) {
            if (in[i] == '%') {
                if (i + 2 < in.size()) {
                    std::string hex = in.substr(i + 1, 2);
                    char ch = static_cast<char>(std::stoi(hex, nullptr, 16));
                    out.push_back(ch);
                    i += 2;
                }
            } else if (in[i] == '+') {
                out.push_back(' ');
            } else {
                out.push_back(in[i]);
            }
        }
        return out;
    }

    std::unordered_map<std::string, std::string> parse_urlencoded(const std::string& body) {
        std::unordered_map<std::string, std::string> params;
        std::istringstream ss(body);
        std::string token;
        while (std::getline(ss, token, '&')) {
            auto pos = token.find('=');
            if (pos != std::string::npos) {
                auto key = token.substr(0, pos);
                auto val = token.substr(pos + 1);
                params[key] = url_decode(val);
            }
        }
        return params;
    }

#define assert_super_admin()
#define assert_super_admin_()\
    auto result = load_current_user(req, service_ptr);\
    if (auto resp = std::get_if<crow::response>(&result))\
    {\
        return std::move(*resp);\
    }\
    auto& user = std::get<plugins::core::models::User>(result);\
\
    if (auto forbidden = require_superadmin(user))\
    {\
        return std::move(*forbidden);\
    }

    void SuperAdminEndpointsGenerator::create_superadmin_endpoints(
        const api::ServicePtr& service_ptr,
        crow::SimpleApp& crow_app,
        procedure_ptr request_restart,
        procedure_ptr request_shutdown
        )
    {
        CROW_ROUTE(crow_app, "/api/v1/superadmin/shutdown").methods("POST"_method)
        ([this, &service_ptr, &request_shutdown](const crow::request& req)
        {
            assert_super_admin()

            request_shutdown();
            return crow::response(200, "Shutdown scheduled");;
        });

        CROW_ROUTE(crow_app, "/api/v1/superadmin/restart").methods("POST"_method)
        ([this, &service_ptr, &request_restart](const crow::request& req)
        {
            assert_super_admin()

            request_restart();
            return crow::response(200, "Restart scheduled");;
        });

        CROW_ROUTE(crow_app, "/api/v1/superadmin/configure").methods("GET"_method)
        ([this, &service_ptr](const crow::request& req)
        {
            assert_super_admin()

            //request_restart();
            return crow::response(200, fmt::vformat(configure_get_template, g_configuration.to_fmt_store()));
        });

        CROW_ROUTE(crow_app, "/api/v1/superadmin/configure").methods("POST"_method)
([this, &service_ptr, &request_restart](const crow::request& req)
{
    assert_super_admin()

    string_map new_configuration;
    auto params = parse_urlencoded(req.body);

    for (const auto& key : params | std::views::keys)
    {
        const auto& value = params[key];
        new_configuration.insert({key, value});
    }

    essential::g_configuration = essential::Configuration(new_configuration);
    essential::g_configuration.save_mind_net_properties();

    if (params.contains("schedule_restart")) {
        request_restart();
    }
    crow::response res;
    res.code = 303;
    res.set_header("Location", "/api/v1/superadmin/configure?message=Changes%20were%20saved");
    return res;
});
    }
}
