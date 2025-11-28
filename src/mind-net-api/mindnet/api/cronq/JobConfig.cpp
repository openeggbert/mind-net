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
#include "mindnet/api/cronq/JobConfig.hpp"

namespace mindnet::api::cronq
{

    JobConfig::JobConfig(const std::string& configuration):
    config_sha256(util::Utils::compute_sha256(configuration))
    {
        parse(configuration);
    }

    const std::string& JobConfig::get_sha256() const
    {
        return config_sha256;
    }

    std::pair<std::optional<std::string>, std::string>
    JobConfig::get_string(const std::string& key) const
    {
        auto it = values.find(key);
        if (it == values.end())
            return {std::nullopt, ""};

        return {std::optional<std::string>(it->second), ""};
    }

    std::pair<std::optional<int>, std::string>
    JobConfig::get_int(const std::string& key) const
    {
        auto it = values.find(key);
        if (it == values.end())
            return {std::nullopt, ""};

        try {
            int v = std::stoi(it->second);
            return {v, ""};
        }
        catch (...) {
            return {std::nullopt, "Parsing integer failed: " + it->second};
        }
    }

    std::pair<std::optional<bool>, std::string>
    JobConfig::get_bool(const std::string& key) const
    {
        auto it = values.find(key);
        if (it == values.end())
            return {std::nullopt, ""};

        std::string v = to_lower(it->second);

        if (v == "1" || v == "true" || v == "yes" || v == "on")
            return {true, ""};
        if (v == "0" || v == "false" || v == "no" || v == "off")
            return {false, ""};

        return {std::nullopt, "Parsing boolean failed: " + it->second};
    }

    // ============================================================
    // GETTERS WITH DEFAULT VALUES
    // ============================================================

    std::pair<std::string, std::string>
    JobConfig::get_string_or_default(const std::string& key, const std::string& default_value) const
    {
        auto it = values.find(key);
        if (it == values.end())
            return {default_value, ""};

        return {it->second, ""}; // string always OK
    }

    std::pair<int, std::string>
    JobConfig::get_int_or_default(const std::string& key, int default_value) const
    {
        auto it = values.find(key);
        if (it == values.end())
            return {default_value, ""};

        try {
            int v = std::stoi(it->second);
            return {v, ""};
        }
        catch (...) {
            return {default_value, "Parsing integer failed: " + it->second};
        }
    }

    std::pair<bool, std::string>
    JobConfig::get_bool_or_default(const std::string& key, bool default_value) const
    {
        auto it = values.find(key);
        if (it == values.end())
            return {default_value, ""};

        std::string v = to_lower(it->second);

        if (v == "1" || v == "true" || v == "yes" || v == "on")
            return {true, ""};
        if (v == "0" || v == "false" || v == "no" || v == "off")
            return {false, ""};

        return {default_value, "Parsing boolean failed: " + it->second};
    }

    // ============================================================
    // helper functions
    // ============================================================

    std::string JobConfig::to_lower(std::string s)
    {
        std::transform(s.begin(), s.end(), s.begin(), ::tolower);
        return s;
    }

    std::vector<std::string> JobConfig::split(const std::string& s, const std::string& delim)
    {
        std::vector<std::string> out;
        size_t pos = 0, end;

        while ((end = s.find(delim, pos)) != std::string::npos)
        {
            out.push_back(s.substr(pos, end - pos));
            pos = end + delim.size();
        }
        out.push_back(s.substr(pos));
        return out;
    }

    inline std::string JobConfig::trim(const std::string& s)
    {
        size_t start = s.find_first_not_of(" \t\n\r");
        if (start == std::string::npos) return "";
        size_t end = s.find_last_not_of(" \t\n\r");
        return s.substr(start, end - start + 1);
    }

    void JobConfig::parse(const std::string& config)
    {
        if (config.empty())
            return;

        auto parts = split(config, "::");

        for (auto& p : parts)
        {
            p = trim(p);
            if (p.empty()) continue;

            auto kv = split(p, "=");
            if (kv.size() != 2) continue;

            std::string key = trim(kv[0]);
            std::string value = trim(kv[1]);

            if (!key.empty())
                values[key] = value;
        }
    }

}
