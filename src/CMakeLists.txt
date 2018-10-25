cmake_minimum_required(VERSION 3.2)
project(cpython_test)

#add_subdirectory(division)
set(SOURCE_FILES main.cpp)

add_library(cpython_test_lib SHARED ${SOURCE_FILES})
target_link_libraries(cpython_test_lib python3.6m boost_python3 boost_numpy3)


add_executable(cpython_test ${SOURCE_FILES})
target_link_libraries(cpython_test python3.6m boost_python3 boost_numpy3)
