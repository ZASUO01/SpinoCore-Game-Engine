cmake_minimum_required(VERSION 3.24)

enable_testing()

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

include(FetchContent)
FetchContent_Declare(
        "googletest"
        GIT_REPOSITORY "https://github.com/google/googletest.git"
        GIT_TAG        "v1.14.0"
)
FetchContent_MakeAvailable(googletest)

set(TEST_SOURCES
        "Config/EngineConfigTest.cpp"
        "FileSystem/FileReaderTest.cpp"
        "FileSystem/FileWriterTest.cpp"
        "Utils/JSONUtilsTest.cpp"
)

add_executable("SpinoCoreTests" ${TEST_SOURCES})

target_include_directories("SpinoCoreTests"
        PRIVATE
        "${CMAKE_CURRENT_SOURCE_DIR}/../src"
        "${CMAKE_CURRENT_SOURCE_DIR}/../external"
)

target_link_libraries("SpinoCoreTests"
        PRIVATE
        SpinoCore::Runtime
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests("SpinoCoreTests")