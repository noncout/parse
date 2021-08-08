cmake_minimum_required(VERSION 3.17)
project(JsonParse)

set(CMAKE_CXX_STANDARD 17)

include_directories(${PROJECT_SOURCE_DIR}/Src)

aux_source_directory(${PROJECT_SOURCE_DIR}/Src SRC)

add_executable(JsonParse main.cpp ${SRC})