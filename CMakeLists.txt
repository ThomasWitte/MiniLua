# set minimum version of CMake
# (this is compatible with all version of CMake but versions before 3.12 ignore
# the range and only use the first version)
cmake_minimum_required(VERSION 3.11...3.18)
# fallback to provide the same functionallity as in CMake 3.12+
if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# project definition
project(MiniLua VERSION 1.0
                DESCRIPTION "Value tracing lua interpreter"
                LANGUAGES CXX C)

# global settings
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
add_compile_options(-Wall -Werror=return-type -fno-omit-frame-pointer)

# generates compile_commands.json that can be used by clang tooling
# (e.g. language server)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# documentation
find_package(Doxygen)
find_package(Plantuml)

# dependencies
find_package(TreeSitterWrapper)
find_package(TreeSitterLua)

# include(CMakePrintHelpers)
# cmake_print_properties(TARGETS TreeSitterWrapper::TreeSitter TreeSitter TreeSitterWrapper PROPERTIES INTERFACE_INCLUDE_DIRECTORIES)

# library
add_subdirectory(src)

# tests
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
    add_subdirectory(extern/catch2)
    # support "*san" build types
    find_package(Sanitizers)

    # support "coverage" build type
    include(CodeCoverage)
    # remove tests and dependencies from coverage report
    set(LCOV_REMOVE_EXTRA "'${CMAKE_SOURCE_DIR}/tests/*'" "'${CMAKE_BINARY_DIR}/*'")

    string(TOUPPER "${CMAKE_BUILD_TYPE}" CMAKE_BUILD_TYPE_UPPERCASE)
    if(CMAKE_BUILD_TYPE_UPPERCASE STREQUAL COVERAGE)
        set(COVERAGE ON)
    endif()

    add_subdirectory(bin)
    add_subdirectory(tests)
    add_subdirectory(bench)

    # example applications
    # (get compiled in ci but not run like tests)
    add_subdirectory(examples)
endif()
