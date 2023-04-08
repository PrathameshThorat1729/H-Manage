cmake_minimum_required(VERSION 3.26.0)

project(H-Manage)
set(EXE hmanage)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED true)

include_directories(include)


file(GLOB_RECURSE SRC_FILES src/${PROJECT_NAME}/*.cpp)
add_executable(${EXE} ${SRC_FILES})

add_subdirectory(lib)