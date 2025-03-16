cmake_minimum_required(VERSION 3.10)

set(STB_IMAGE_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

set(STB_IMAGE_SOURCES src/stb_image.cpp)

add_library(stb STATIC ${STB_IMAGE_SOURCES})

target_include_directories(stb PUBLIC ${STB_IMAGE_INCLUDE_DIR})
