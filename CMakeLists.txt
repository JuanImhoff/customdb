cmake_minimum_required(VERSION 3.16)
project(customdb
    VERSION 0.1.0
    DESCRIPTION "A lightweight key-value database in modern C++"
    LANGUAGES CXX
)

# --------------------------------------------------
# Global settings
# --------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json (for clang-tidy, editors)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------
# Warnings (target-based, modern CMake)
# --------------------------------------------------
function(enable_warnings target)
    if (MSVC)
        target_compile_options(${target} PRIVATE /W4 /WX)
    else()
        target_compile_options(${target} PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Werror
        )
    endif()
endfunction()

# --------------------------------------------------
# customdb library
# --------------------------------------------------
add_library(customdb
    src/store.cpp
)

target_include_directories(customdb
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

enable_warnings(customdb)

# --------------------------------------------------
# Key-value CLI application
# --------------------------------------------------
add_executable(kvcli
    app/kvcli.cpp
)

target_link_libraries(kvcli
    PRIVATE customdb
)

enable_warnings(kvcli)

# --------------------------------------------------
# Testing
# --------------------------------------------------
include(CTest)
enable_testing()

include(FetchContent)

FetchContent_Declare( # add googletest from github repo
    googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.17.0
)

# Prevent overriding parent project's compiler/linker settings USELESS IN GCC
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

FetchContent_MakeAvailable(googletest)

add_executable(customdb_tests
    tests/test_store.cpp
)

target_link_libraries(customdb_tests
    PRIVATE
        GTest::gtest
        GTest::gtest_main
        customdb
)

enable_warnings(customdb_tests)

add_test(NAME customdb_tests COMMAND customdb_tests)

# --------------------------------------------------
# Optional: sanitizers (Debug only)
# --------------------------------------------------
if (CMAKE_BUILD_TYPE STREQUAL "Debug" AND NOT MSVC)
    message(STATUS "Enabling AddressSanitizer + UBSan")
    add_compile_options(-fsanitize=address,undefined)
    add_link_options(-fsanitize=address,undefined)
endif()