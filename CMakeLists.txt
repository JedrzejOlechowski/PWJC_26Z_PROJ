cmake_minimum_required(VERSION 3.16)

project(PWJC_26Z_PROJ LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.7.1
)
FetchContent_MakeAvailable(Catch2)

add_executable(main main.cpp)

add_executable(test_data_parser
    tests/test_data_parser.cpp
    DataLoader.cpp
)

target_link_libraries(test_data_parser PRIVATE Catch2::Catch2WithMain)

include(CTest)
add_test(NAME data_parser COMMAND test_data_parser)
