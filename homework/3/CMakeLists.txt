include(FetchContent)

cmake_minimum_required(VERSION 3.28)
project(config_lang)

set(CMAKE_CXX_STANDARD 17)

FetchContent_Declare(
        yaml-cpp
        GIT_REPOSITORY https://github.com/jbeder/yaml-cpp.git
)
FetchContent_MakeAvailable(yaml-cpp)

enable_testing()
include_directories(translator)
add_library(trans_lib
        translator/Translator.cpp
)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

add_executable(tests tests/tests.cpp)

target_link_libraries(tests trans_lib)
target_link_libraries(tests GTest::GTest GTest::Main)

add_executable(config_lang src/main.cpp)
target_link_libraries(config_lang trans_lib)
TARGET_LINK_LIBRARIES(trans_lib yaml-cpp::yaml-cpp)
