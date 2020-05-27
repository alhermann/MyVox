cmake_minimum_required(VERSION 2.8)
project(MyVox)

include_directories(libraries/libkdtree++)
include_directories(src/helpers)
include_directories(/gpfs/home/hermann/local/include/eigen3)

set(SOURCES src/main.cpp)
set(CMAKE_CXX_FLAGS "-std=c++11 -O3 -fopenmp")

add_executable(${PROJECT_NAME} ${SOURCES})