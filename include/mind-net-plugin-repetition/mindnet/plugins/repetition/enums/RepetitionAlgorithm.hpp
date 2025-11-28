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
#include "mindnet/model/EnumDefinition.hpp"
#include "mindnet/essential/EnumUtils.hpp"

namespace mindnet::plugins::repetition::enums
{
    /**
     *
     * @author robertvokac
     */
#define REPETITION_ALGORITHM_LIST(X, ENUM_NAME) \
X(Repetition0, 0, ENUM_NAME)               \
X(Repetition2, 2, ENUM_NAME)               \
X(Repetition4, 4, ENUM_NAME)               \
X(Repetition18, 18, ENUM_NAME)

    DECLARE_ENUM(RepetitionAlgorithm, repetition_algorithm, REPETITION_ALGORITHM_LIST)
} // namespace mindnet::plugins::repetition::enums
