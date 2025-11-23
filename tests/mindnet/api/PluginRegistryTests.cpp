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
#include <gtest/gtest.h>
#include "../../../include/mind-net-api/mindnet/api/PluginRegistry.hpp"
#include "../../../include/mind-net-api/mindnet/api/Plugin.hpp"


using namespace mindnet::api;

// Helper function to create a plugin
PluginPtr make_plugin(const std::string& name,
                      const std::vector<std::string>& apps = {},
                      const std::vector<std::string>& deps = {}
                      )
{
    auto plugin = std::make_shared<Plugin>(name, "desc", apps, deps);
    plugin->close_for_changes(); // required by registry
    return plugin;
}

TEST(PluginRegistryTest, SingleCorePlugin)
{
    PluginRegistry registry;
    auto core = make_plugin("core");
    registry.register_plugin(core);

    auto sorted = registry.get_plugin_names_sorted_by_dependencies();

    ASSERT_EQ(sorted.size(), 1);
    EXPECT_EQ(sorted[0], "core");
}

TEST(PluginRegistryTest, PluginDependsOnCore)
{
    PluginRegistry registry;
    auto core = make_plugin("core");
    auto a = make_plugin("A"); // automatically has dependency on core

    mindnet::essential::g_configuration.allowed_plugins.insert("A");
    registry.register_plugin(core);
    registry.register_plugin(a);

    auto sorted = registry.get_plugin_names_sorted_by_dependencies();

    ASSERT_EQ(sorted.size(), 2);
    // core must be before A
    auto core_pos = std::find(sorted.begin(), sorted.end(), "core");
    auto a_pos = std::find(sorted.begin(), sorted.end(), "A");
    ASSERT_NE(core_pos, sorted.end());
    ASSERT_NE(a_pos, sorted.end());
    EXPECT_LT(core_pos, a_pos);
}

TEST(PluginRegistryTest, MultipleIndependentPlugins)
{
    PluginRegistry registry;
    auto core = make_plugin("core");
    auto a = make_plugin("A");
    auto b = make_plugin("B");

    mindnet::essential::g_configuration.allowed_plugins.insert("A");
    mindnet::essential::g_configuration.allowed_plugins.insert("B");

    registry.register_plugin(core);
    registry.register_plugin(a);
    registry.register_plugin(b);

    auto sorted = registry.get_plugin_names_sorted_by_dependencies();

    ASSERT_EQ(sorted.size(), 3);
    // core must be before A and B
    auto core_pos = std::find(sorted.begin(), sorted.end(), "core");
    auto a_pos = std::find(sorted.begin(), sorted.end(), "A");
    auto b_pos = std::find(sorted.begin(), sorted.end(), "B");

    EXPECT_LT(core_pos, a_pos);
    EXPECT_LT(core_pos, b_pos);
}

TEST(PluginRegistryTest, MissingDependencyThrows)
{
    PluginRegistry registry;
    auto core = make_plugin("core");
    // plugin C has explicit dependency on X (which doesn't exist)
    auto c = make_plugin("C", {}, {"X"});
    mindnet::essential::g_configuration.allowed_plugins.insert("C");

    registry.register_plugin(core);
    registry.register_plugin(c);

    EXPECT_THROW({
                     registry.get_plugin_names_sorted_by_dependencies();

                 }, MissingDependencyException);
}

TEST(PluginRegistryTest, CyclicDependencyThrows)
{
    PluginRegistry registry;
    auto core = make_plugin("core");
    auto a = make_plugin("A", {},{"B"}); // A depends on B (+ core)
    auto b = make_plugin("B", {},{"A"}); // B depends on A (+ core)
    mindnet::essential::g_configuration.allowed_plugins.insert("A");
    mindnet::essential::g_configuration.allowed_plugins.insert("B");

    registry.register_plugin(core);
    registry.register_plugin(a);
    registry.register_plugin(b);

    EXPECT_THROW({
                     registry.get_plugin_names_sorted_by_dependencies();

                 }, CyclicDependencyException);
}
