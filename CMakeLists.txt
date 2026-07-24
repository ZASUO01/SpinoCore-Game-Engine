cmake_minimum_required(VERSION 3.24)
project("SpinoCoreEngine" VERSION 1.0.0 LANGUAGES CXX C)

# ======================================
# C++ config
# ======================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ======================================
# Compile options
# ======================================
option(BUILD_TESTS "Enable building unit tests with GoogleTest" OFF)

# ======================================
# Final build
# ======================================
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# ======================================
# Engine source files
# ======================================
set(ENGINE_SOURCES
    "include/SpinoCore/Engine.h"

    # Config
    "include/SpinoCore/Config/ConfigEntry.h"
    "include/SpinoCore/Config/EngineConfig.h"

    # Logs
    "include/SpinoCore/Logs/EngineLogger.h"

    # Utils
    "include/SpinoCore/Utils/DeepPimpl.h"

    #Config
    "src/Config/ConfigHelper.h"
    "src/Config/ConfigHelper.cpp"
    "src/Config/Constants.h"
    "src/Config/EngineConfig.cpp"

    # Logs
    "src/Logs/EngineLogger.cpp"

    # FileSystem
    "src/FileSystem/Core/PathResolver.cpp"
    "src/FileSystem/Core/PathResolver.h"
    "src/FileSystem/Core/VirtualFileSystem.cpp"
    "src/FileSystem/Core/VirtualFileSystem.h"
    "src/FileSystem/IO/FileReader.cpp"
    "src/FileSystem/IO/FileReader.h"
    "src/FileSystem/IO/FileWriter.cpp"
    "src/FileSystem/IO/FileWriter.h"
    "src/FileSystem/Mount/MountPoint.h"
    "src/FileSystem/Mount/FolderMount.cpp"
    "src/FileSystem/Mount/FolderMount.h"

    #Utils
    "src/Utils/JSONUtils.cpp"
    "src/Utils/JSONUtils.h"

    "src/Engine.cpp"
)

set(EXTERNAL_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann/json.hpp"
        include/SpinoCore/Config/EngineConfig.h
        include/SpinoCore/Config/ConfigEntry.h
)

add_library(${PROJECT_NAME} STATIC ${ENGINE_SOURCES} ${EXTERNAL_SOURCES})
add_library("SpinoCore::Runtime" ALIAS ${PROJECT_NAME})

# ======================================
# Dependencies access control
# ======================================
target_include_directories(${PROJECT_NAME}
        PUBLIC
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/src"
        "${CMAKE_CURRENT_SOURCE_DIR}/external"
)

# ======================================
# Get 3rd party libraries
# ======================================
include(FetchContent)
set(FETCH_CONTENT_BASE_DIR "${CMAKE_BINARY_DIR}/third_party")

set(SDL_SHARED OFF CACHE BOOL "Disable SDL shared library" FORCE)
set(SDL_STATIC ON CACHE BOOL "Enable SDL static library" FORCE)
set(SDL_TESTS OFF CACHE BOOL "Disable SDL tests" FORCE)
set(SDL_EXAMPLES OFF CACHE BOOL "Disable SDL examples" FORCE)
set(SDL_DOCS OFF CACHE BOOL "Disable SDL docs" FORCE)

FetchContent_Declare(
        "SDL3"
        GIT_REPOSITORY "https://github.com/libsdl-org/SDL.git"
        GIT_TAG        "release-3.2.0"
)
FetchContent_MakeAvailable("SDL3")

# ======================================
# Link libraries
# ======================================
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3)

# ======================================
# Include tests configuration if enabled
# ======================================
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()

# ==============================================================================
# Dynamic files config
# ==============================================================================
include("${CMAKE_CURRENT_LIST_DIR}/cmake/SetupWorkspace.cmake")

setup_workspace(${PROJECT_NAME} "@config" "This folder is where you can put a config.json file to configure the engine. Please do not remove it.")
setup_workspace(${PROJECT_NAME} "@assets" "This folder is where you can put all of your project assets. Please do not remove it.")