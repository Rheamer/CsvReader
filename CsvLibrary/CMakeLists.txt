cmake_minimum_required(VERSION 3.0.0)

project(csv-library LANGUAGES CXX)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(Sources
  
  
  CsvNokia.h
  CsvNokia.cpp
)

add_library(csv-library ${Sources})
target_include_directories(csv-library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})