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
     * @enum RepetitionSchedule
     *
     * Defines how notes and their associated questions are ordered and traversed
     * during a repetition (review) session.
     *
     * Each mode represents a different cognitive learning strategy and traversal pattern
     * through the hierarchical note structure (Zettelkasten-like trees).
     *
     * These scheduling modes determine whether the repetition session follows logical context,
     * broad topic coverage, or pure recall testing without context.
     *
     * ---
     * ### Overview of modes:
     *
     * | Mode | Description | Cognitive effect | Typical use |
     * |------|--------------|------------------|--------------|
     * | **DepthFirst** | Traverses notes hierarchically, from parent to children recursively. | Strong contextual learning, topic immersion. | Learning new material (NewOnly). |
     * | **BreadthFirst** | Traverses level by level across branches. | Balanced exposure, good topic variety. | Mixed sessions (DueAndNew). |
     * | **Random** | Fully random order of notes/questions. | Context-free recall testing. | Testing, exam mode (DueOnly). |
     * | **DepthFirstShuffled** | DFS traversal but shuffles order within branches. | Combines structure with variation. | Second-phase reviews. |
     * | **Interleaved** | Alternates between different branches. | Promotes discrimination learning and long-term retention. | Advanced practice sessions. |
     * | **DifficultySorted** | Orders notes by difficulty (ascending). | Builds momentum, progressive challenge. | Bootstrapping new topics. |
     * | **Chronological** | Orders by creation or update date. | Reflective, chronological review of knowledge. | Manual or historical review. |
     *
     * ---
     * ### Cognitive notes:
     *
     * - **DepthFirst** is best for initial comprehension and building conceptual hierarchy.
     * - **BreadthFirst** prevents overfitting to a single topic by mixing different branches.
     * - **Random** provides the purest measurement of memory retention.
     * - **Interleaved** is widely supported by cognitive science as one of the most effective study strategies.
     * - **DifficultySorted** supports scaffolding and gradual difficulty increase.
     * - **Chronological** helps users reflect on their learning progression over time.
     *
     * ---
     * ### Implementation hints:
     *
     * - DepthFirst: use recursive traversal.
     * - BreadthFirst: use a queue (FIFO).
     * - Random: shuffle all eligible notes/questions.
     * - Interleaved: interleave multiple branch queues.
     * - DifficultySorted: sort by difficulty ascending.
     * - Chronological: order by created_at.
     *
     * ---
     * @author
     *   Robert Vokac
     */
#define REPETITION_SCHEDULE_LIST(X, ENUM_NAME)        \
    X(DepthFirst, 0, ENUM_NAME)                       \
    X(BreadthFirst, 1, ENUM_NAME)                     \
    X(Random, 2, ENUM_NAME)                           \
    X(DepthFirstShuffled, 3, ENUM_NAME)               \
    X(Interleaved, 4, ENUM_NAME)                      \
    X(DifficultySorted, 5, ENUM_NAME)                 \
    X(Chronological, 6, ENUM_NAME)

    DECLARE_ENUM(RepetitionSchedule, repetition_schedule, REPETITION_SCHEDULE_LIST)
} // namespace mindnet::plugins::repetition::enums
