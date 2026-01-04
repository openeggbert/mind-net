# CMake Review & Modernization TODO

This document summarizes the current state of the **mind-net** CMake build system and provides recommendations for modernization, optimization, and maintainability.

---

## ✅ Strengths

1. **Project modularity**

   * Each component (`mind-net-api`, `mind-net-model`, `mind-net-http`, plugins, etc.) has its own `CMakeLists.txt`.
   * Provides a good basis for scalability and selective builds.

2. **Modern CMake version**

   * Using `cmake_minimum_required(VERSION 3.25.1)`, which supports target-based features.

3. **Unified C++ standard**

   * `set(CMAKE_CXX_STANDARD 23)` and `CMAKE_CXX_STANDARD_REQUIRED ON` are set globally.
   * Most submodules inherit this setting consistently.

4. **Release optimization flags**

   * `-O3`, `-DNDEBUG`, `-ffunction-sections`, `-fdata-sections`, `-Wl,--gc-sections` reduce binary size and improve performance.

5. **Testing support**

   * `googletest` is included and tests are placed in a dedicated directory.

6. **Bundled third-party dependencies**

   * Dependencies (`SQLiteCpp`, `nlohmann_json`, `crow`, `asio`, `googletest`) are tracked in `thirdparty/`, ensuring reproducible builds.

---

## ⚠️ Weaknesses

1. **Global compiler flags**

   * `CMAKE_CXX_FLAGS_RELEASE` is set globally.
   * Modern CMake prefers `target_compile_options()` for better encapsulation.

2. **Outdated include management**

   * Many modules use `include_directories()` instead of `target_include_directories(... PUBLIC/PRIVATE/INTERFACE)`.
   * This leads to uncontrolled global include paths.

3. **Dependency visibility is unclear**

   * `target_link_libraries()` often omits `PRIVATE` vs `PUBLIC`.
   * This can leak unnecessary dependencies to other targets.

4. **Copy-paste boilerplate**

   * Each submodule repeats similar logic (`add_library(...)`, `include_directories(...)`).
   * Increases maintenance overhead.

5. **Non-optional third-party libraries**

   * `add_subdirectory(thirdparty/...)` is unconditional.
   * Even unused libraries are always built, slowing down compilation.

6. **Static “plugin” libraries**

   * Plugins (`chat`, `mail`, `dictionary`, etc.) are built as `STATIC` and linked into the main app.
   * This limits extensibility compared to dynamic modules.

7. **Tests are always built**

   * `googletest` is included even when tests are disabled.
   * Proper conditionals are missing.

8. **No precompiled headers (PCH)**

   * Compilation may be slower due to heavy includes.
   * CMake has built-in `target_precompile_headers()` support.

9. **No install or export rules**

   * No `install(TARGETS ...)` or `export()` configuration.
   * Project cannot be installed system-wide or used as a dependency elsewhere.

---

## 🚀 Recommendations

1. **Switch to target-based CMake**

   * Replace `include_directories()` with `target_include_directories()`.
   * Replace global `CMAKE_CXX_FLAGS_RELEASE` with `target_compile_options()`.

2. **Introduce helper macros/functions**

   * Define a central function in root `CMakeLists.txt`, e.g.:

     ```cmake
     function(add_mindnet_module name)
         add_library(${name} STATIC ${ARGN})
         target_include_directories(${name} PUBLIC ${CMAKE_SOURCE_DIR}/src)
         target_compile_features(${name} PUBLIC cxx_std_23)
     endfunction()
     ```
   * This reduces duplication across submodules.

3. **Make third-party dependencies optional**

   * Example:

     ```cmake
     option(USE_SQLITECPP "Enable SQLiteCpp" ON)
     if(USE_SQLITECPP)
         add_subdirectory(thirdparty/SQLiteCpp)
     endif()
     ```

4. **Consider dynamic plugins**

   * Use `add_library(mind_net_plugin_chat MODULE ...)` or `SHARED`.
   * This allows runtime plugin loading (`dlopen`) and better extensibility.

5. **Conditional test builds**

   * Wrap tests in:

     ```cmake
     if(ENABLE_TESTS)
         add_subdirectory(tests)
     endif()
     ```

6. **Improve build speed**

   * Use precompiled headers:

     ```cmake
     target_precompile_headers(mind_net_essential PRIVATE <vector> <string> ...)
     ```
   * Enable compiler cache (`CMAKE_CXX_COMPILER_LAUNCHER=ccache`).

7. **Add install and export targets**

   * Example:

     ```cmake
     install(TARGETS mind_net_app mind_net_api ...)
     export(EXPORT MindNetTargets)
     ```

8. **Better default build type**

   * Default to `Debug` for developers:

     ```cmake
     if(NOT CMAKE_BUILD_TYPE)
         set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
     endif()
     ```

---

## 📊 Summary

* **Strengths:** modular design, modern standard, optimized Release build, integrated tests.
* **Weaknesses:** global flags, outdated include style, heavy boilerplate, non-optional dependencies, static plugins.
* **Next steps:** refactor to target-based CMake, modularize plugin handling, introduce PCH, conditionally build tests, and add install/export rules.

This modernization will improve **maintainability, build speed, portability, and extensibility** of the mind-net project.

