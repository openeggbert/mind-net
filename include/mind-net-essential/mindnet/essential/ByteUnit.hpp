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
///////////////////////////////////////////////////////////////////////////////////////////////
// mind-net : A generic, high-performance REST backend framework written in modern C++.
// Copyright ...
///////////////////////////////////////////////////////////////////////////////////////////////
#pragma once

#include <stdexcept>
#include <string>
#include <vector>
#include <cstdint>


namespace mindnet::essential
{
    /**
     * @enum ByteUnit
     * @brief Represents standard data size units based on 1024 (B, KB, MB, GB, TB).
     */
    enum class ByteUnit
    {
        B  = 0,
        KB = 1,
        MB = 2,
        GB = 3,
        TB = 4
    };

    inline std::string byte_unit_to_string(ByteUnit unit)
    {
        switch (unit)
        {
        case ByteUnit::B:  return "B";
        case ByteUnit::KB: return "KB";
        case ByteUnit::MB: return "MB";
        case ByteUnit::GB: return "GB";
        case ByteUnit::TB: return "TB";
        default: return "Unknown";
        }
    }

    inline std::vector<ByteUnit> byte_unit_values()
    {
        static std::vector<ByteUnit> values = {
            ByteUnit::B,
            ByteUnit::KB,
            ByteUnit::MB,
            ByteUnit::GB,
            ByteUnit::TB
        };
        return values;
    }

    inline ByteUnit string_to_byte_unit(const std::string& s)
    {
        if (s == "B")  return ByteUnit::B;
        if (s == "KB") return ByteUnit::KB;
        if (s == "MB") return ByteUnit::MB;
        if (s == "GB") return ByteUnit::GB;
        if (s == "TB") return ByteUnit::TB;

        throw std::runtime_error("Invalid ByteUnit string: " + s);
    }

    /**
     * @brief Convert a value and unit to raw bytes.
     * @example to_bytes(2, ByteUnit::MB) == 2 * 1024 * 1024
     */
    inline std::uint64_t to_bytes(std::uint64_t value, ByteUnit unit)
    {
        switch (unit)
        {
        case ByteUnit::B:  return value;
        case ByteUnit::KB: return value * 1024ULL;
        case ByteUnit::MB: return value * 1024ULL * 1024ULL;
        case ByteUnit::GB: return value * 1024ULL * 1024ULL * 1024ULL;
        case ByteUnit::TB: return value * 1024ULL * 1024ULL * 1024ULL * 1024ULL;
        default:
            throw std::runtime_error("Invalid ByteUnit");
        }
    }

    /**
     * @brief Convert raw bytes to a specific ByteUnit.
     * @example from_bytes(1048576, ByteUnit::MB) == 1
     */
    inline double from_bytes(std::uint64_t bytes, ByteUnit unit)
    {
        switch (unit)
        {
        case ByteUnit::B:  return static_cast<double>(bytes);
        case ByteUnit::KB: return bytes / 1024.0;
        case ByteUnit::MB: return bytes / (1024.0 * 1024.0);
        case ByteUnit::GB: return bytes / (1024.0 * 1024.0 * 1024.0);
        case ByteUnit::TB: return bytes / (1024.0 * 1024.0 * 1024.0 * 1024.0);
        default:
            throw std::runtime_error("Invalid ByteUnit");
        }
    }

    /**
     * @brief Convert a value between units (e.g. MB → KB)
     * @example convert(1, ByteUnit::MB, ByteUnit::KB) == 1024
     */
    inline double convert(double value, ByteUnit from, ByteUnit to)
    {
        std::uint64_t bytes = to_bytes(static_cast<std::uint64_t>(value), from);
        return from_bytes(bytes, to);
    }
}
