cmake_minimum_required(VERSION 3.0.0)

project(csvreader LANGUAGES CXX VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


if(WIN32)
    add_compile_options(/utf-8)
endif(WIN32)

add_subdirectory(CsvLibrary)

add_executable(csvreader CsvParser.cpp)
target_link_libraries(csvreader csv-library)