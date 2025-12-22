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
#include <array>
#include <chrono>
#include <random>
#include <sstream>
#include <iomanip>

#include "mindnet/plugins/core/ErrorBuilder.hpp"

#include "mindnet/essential/Version.hpp"
#include "mindnet/util/Utils.hpp"

namespace mindnet::plugins::core
{

    std::array<uint8_t, 16> generate_uuid_v7_array()
    {
        std::array<uint8_t, 16> uuid{};

        // timestamp (Unix epoch ms, 48 bit)
        const auto now = std::chrono::time_point_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now()
        );
        uint64_t ms = static_cast<uint64_t>(now.time_since_epoch().count());

        uuid[0] = (ms >> 40) & 0xFF;
        uuid[1] = (ms >> 32) & 0xFF;
        uuid[2] = (ms >> 24) & 0xFF;
        uuid[3] = (ms >> 16) & 0xFF;
        uuid[4] = (ms >> 8) & 0xFF;
        uuid[5] = ms & 0xFF;

        // random part
        static thread_local std::mt19937_64 rng{ std::random_device{}() };
        uint64_t r1 = rng();
        uint64_t r2 = rng();

        // version 7
        uuid[6] = 0x70 | ((r1 >> 8) & 0x0F);
        uuid[7] = r1 & 0xFF;

        // variant RFC 4122 (10xx)
        uuid[8] = 0x80 | ((r2 >> 56) & 0x3F);

        uuid[9]  = (r2 >> 48) & 0xFF;
        uuid[10] = (r2 >> 40) & 0xFF;
        uuid[11] = (r2 >> 32) & 0xFF;
        uuid[12] = (r2 >> 24) & 0xFF;
        uuid[13] = (r2 >> 16) & 0xFF;
        uuid[14] = (r2 >> 8) & 0xFF;
        uuid[15] = r2 & 0xFF;

        return uuid;
    }

    std::string uuid_to_string(const std::array<uint8_t, 16>& u)
    {
        std::ostringstream ss;
        ss << std::hex << std::setfill('0');

        for (size_t i = 0; i < 16; ++i)
        {
            ss << std::setw(2) << static_cast<int>(u[i]);
            if (i == 3 || i == 5 || i == 7 || i == 9)
                ss << '-';
        }
        return ss.str();
    }

    std::string generate_uuid_v7()
    {
        return uuid_to_string(generate_uuid_v7_array());
    }

    std::string generate_code_6()
    {
        static constexpr char alphabet[] =
            "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        static constexpr size_t alphabet_size = sizeof(alphabet) - 1;

        static thread_local std::mt19937 rng{ std::random_device{}() };
        std::uniform_int_distribution<size_t> dist(0, alphabet_size - 1);

        std::string result;
        result.reserve(6);

        for (int i = 0; i < 6; ++i)
        {
            result += alphabet[dist(rng)];
        }

        return result;
    }

    std::string today_yyyymmdd()
    {
        using namespace std::chrono;

        auto today = floor<days>(system_clock::now());
        year_month_day ymd{today};

        return std::format("{:%Y%m%d}", ymd);
    }

    static const std::string MN_ERR_ = "MN-ERR-";
    std::string generate_human_identification()
    {
        string random_string = generate_code_6();
        string today = today_yyyymmdd();
        return MN_ERR_ + today + "-" + random_string;
    }

    ErrorBuilder::ErrorBuilder(
            const std::string& message,
            enums::ErrorSeverity s,
            const enums::ErrorOrigin origin,
            const enums::ExecutionLayer layer)
    {
        model_.message = message;
        model_.severity = s;
        model_.origin = origin;
        model_.layer = layer;
    }

    ErrorBuilder& ErrorBuilder::http_status(int status)
    {
        model_.http_status = status;
        return *this;
    }

    ErrorBuilder& ErrorBuilder::plugin(std::string v)
    {
        model_.plugin = v;
        return *this;
    }

    ErrorBuilder& ErrorBuilder::operation(essential::Crudl o)
    {
        model_.operation = o;
        return *this;
    }

    ErrorBuilder& ErrorBuilder::cpp(const string& cpp_namespace, const string& cpp_class, const string& cpp_symbol)
    {
        model_.cpp_namespace = cpp_namespace;
        model_.cpp_class = cpp_class;
        model_.cpp_symbol = cpp_symbol;
        return *this;
    }

    ErrorBuilder& ErrorBuilder::sql(std::string sql)
    {
        model_.sql_query = sql;
        return *this;
    }

    ErrorBuilder& ErrorBuilder::user(identification id)
    {
        model_.user_id = id;
        return *this;
    }

    ErrorBuilder& ErrorBuilder::request(std::string id)
    {
        model_.request_identification = id;
        return *this;
    }

    ErrorBuilder& ErrorBuilder::exception(const std::exception& e)
    {
        model_.exception_type = typeid(e).name();
        model_.exception_message = e.what();

        if (model_.message.empty())
            model_.message = "Internal server error";

        return *this;
    }

    models::Error ErrorBuilder::build()
    {
        if (model_.message.empty())
            throw std::logic_error("ErrorBuilder: message is mandatory");

        if (model_.severity == enums::ErrorSeverity::Info &&
            model_.http_status == 0)
            model_.http_status = 200;

        if (model_.http_status == 0)
            model_.http_status = 500;

        model_.human_identification = generate_human_identification();
        model_.uuid = generate_uuid_v7();

        model_.mindnet_version = STRINGIFY(MIND_NET_VERSION);

        return model_;    }
    ;

}
