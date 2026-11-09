cmake_minimum_required(VERSION 3.14)
project(TetrisESP32_Tests)

set(CMAKE_CXX_STANDARD 14)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(run_tests 
    tests/test_tetris.cpp 
    TetrisESP32/Board.cpp
)

target_include_directories(run_tests PRIVATE TetrisESP32)

target_link_libraries(run_tests gtest_main)

include(GoogleTest)
gtest_discover_tests(run_tests)