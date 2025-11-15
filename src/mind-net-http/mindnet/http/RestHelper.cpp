//
// Created by robertvokac on 7/31/25.
//

#include "mindnet/http/RestHelper.hpp"

#include <string>
#include <crow.h>
#include <nlohmann/json.hpp>

#include "mindnet/essential/Global.hpp"
#include "mindnet/essential/DatabaseType.hpp"
#include "mindnet/model/ModelDefinition.hpp"

namespace mindnet::http
{
    using_loggers()

    crow::json::wvalue RestHelper::model_to_wvalue(const entity_fields& values,
                                                   const model::ModelDefinition& def,
                                                   const std::set<std::string>& fields_vector_filter)
    {
        if (values.empty()) { throw std::runtime_error("Empty values"); }
        crow::json::wvalue res;
        auto columns = def.get_columns();

        bool filter_by_fields = !fields_vector_filter.empty();
        for (int i = 0; i < columns.size(); i++)
        {
            if (filter_by_fields && !fields_vector_filter.contains(columns[i].get_column_name()))
            {
                continue;
            }
            auto column_name = columns[i].get_column_name();
            auto value = values[i];

            std::visit([&res, &column_name](auto&& val)
            {
                res[column_name] = crow::json::wvalue(val);
            }, values[i]);
        }
        return res;
    }

    string RestHelper::check_body_is_valid(const crow::json::rvalue& body, const model::ModelDefinition& def,
                                           const bool id_wanted)
    {
        for (auto column_ : def.get_columns())
        {
            debug << "Checking " << column_.get_column_name() << commit;
            if (column_.get_column_name() == "id")
            {
                if (!id_wanted && body.has(column_.get_column_name()))
                {
                    auto msg = "Id is not allowed in body";
                    err << msg << commit;
                    return msg;
                }
                if (id_wanted && !body.has(column_.get_column_name()))
                {
                    auto msg = "Mandatory column id is missing";
                    err << msg << std::endl;
                    return msg;
                }
                continue;
            }
            if (column_.is_mandatory() && !column_.is_auto() && !body.has(column_.get_column_name()))
            {
                auto msg = "Mandatory column " + column_.get_column_name() + " is missing";
                err << msg << std::endl;
                return msg;
            }
        }
        debug << "Body is valid" << commit;
        return "";
    }

#include <crow.h>
#include <nlohmann/json.hpp>

    nlohmann::json RestHelper::crow_json_to_nlohmann_json(const crow::json::rvalue& crow_json)
    {
        nlohmann::json result;

        for (const auto& key : crow_json.keys())
        {
            const auto& val = crow_json[key];

            if (val.t() == crow::json::type::Null)
            {
                result[key] = nullptr;
            }
            else if (val.t() == crow::json::type::String)
            {
                result[key] = val.s();
            }
            else if (val.t() == crow::json::type::Number)
            {
                result[key] = val.d();
            }
            else if (val.t() == crow::json::type::True || val.t() == crow::json::type::False)
            {
                result[key] = val.b();
            }
            else if (val.t() == crow::json::type::List)
            {
                nlohmann::json arr = nlohmann::json::array();
                for (size_t i = 0; i < val.size(); ++i)
                {
                    arr.push_back(crow_json_to_nlohmann_json(val[i]));
                }
                result[key] = arr;
            }
            else if (val.t() == crow::json::type::Object)
            {
                result[key] = crow_json_to_nlohmann_json(val);
            }
        }

        return result;
    }

    crow::json::wvalue RestHelper::rjson_to_wjson(const crow::json::rvalue& rjson)
    {
        crow::json::wvalue wjson;

        for (const auto& kv : rjson)
        {
            const std::string& key = kv.key();
            const auto& val = kv;

            switch (val.t())
            {
            case crow::json::type::Null:
                wjson[key] = crow::json::wvalue();
                break;

            case crow::json::type::String:
                wjson[key] = val.s();
                break;

            case crow::json::type::Number:
                {
                    double d = val.d();
                    if (std::isfinite(d) &&
                        std::floor(d) == d &&
                        d >= static_cast<double>(std::numeric_limits<long long>::min()) &&
                        d <= static_cast<double>(std::numeric_limits<long long>::max()))
                    {
                        wjson[key] = static_cast<long long>(d);
                    }
                    else
                    {
                        wjson[key] = d;
                    }
                    break;
                }

            case crow::json::type::True:
            case crow::json::type::False:
                wjson[key] = val.b();
                break;

            case crow::json::type::List:
                {
                    crow::json::wvalue::list list;
                    list.reserve(val.size());
                    for (size_t i = 0; i < val.size(); ++i)
                        list.push_back(rjson_to_wjson(val[i]));
                    wjson[key] = std::move(list);
                    break;
                }

            case crow::json::type::Object:
                wjson[key] = rjson_to_wjson(val);
                break;

            default:
                throw std::runtime_error("Unknown crow::json type: " + std::to_string(static_cast<int>(val.t())));
            }
        }

        return wjson;
    }
}
