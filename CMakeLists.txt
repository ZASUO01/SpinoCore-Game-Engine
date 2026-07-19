cmake_minimum_required(VERSION 3.24)
project("SpinoCoreEngine" VERSION 1.0.0 LANGUAGES CXX C)

# ======================================
# C++ config
# ======================================
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ======================================
# Final build
# ======================================
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")

# ======================================
# Engine source files
# ======================================
set(ENGINE_SOURCES
    "include/SpinoCore/Engine.h"
    "src/Engine.cpp"
)

set(EXTERNAL_SOURCES
        #"${CMAKE_CURRENT_SOURCE_DIR}/external/nlohmann/json.hpp"
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
        #"${CMAKE_CURRENT_SOURCE_DIR}/external"
)

