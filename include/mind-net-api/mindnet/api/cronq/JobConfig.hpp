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

#pragma once
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <optional>
#include <algorithm>

#include "mindnet/util/Utils.hpp"

namespace mindnet::api::cronq
{
    class JobConfig
    {
    public:
        JobConfig(const std::string& configuration);

        // ============================================================
        // SHA-256 getter
        // ============================================================
        const std::string& get_sha256() const;

        // ============================================================
        // BASIC GETTERS (WITHOUT DEFAULT)
        // ============================================================

        std::pair<std::optional<std::string>, std::string>
        get_string(const std::string& key) const;

        std::pair<std::optional<int>, std::string>
        get_int(const std::string& key) const;

        std::pair<std::optional<bool>, std::string>
        get_bool(const std::string& key) const;

        // ============================================================
        // GETTERS WITH DEFAULT VALUES
        // ============================================================

        std::pair<std::string, std::string>
        get_string_or_default(const std::string& key, const std::string& default_value) const;

        std::pair<int, std::string>
        get_int_or_default(const std::string& key, int default_value) const;

        std::pair<bool, std::string>
        get_bool_or_default(const std::string& key, bool default_value) const;

    private:
        std::unordered_map<std::string, std::string> values;
        std::string config_sha256;

        // ============================================================
        // helper functions
        // ============================================================

        static std::string to_lower(std::string s);

        static std::vector<std::string> split(const std::string& s, const std::string& delim);

        static inline std::string trim(const std::string& s);

        void parse(const std::string& config);
    };
}