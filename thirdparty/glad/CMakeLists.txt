cmake_minimum_required(VERSION 3.10)

# glad 库的头文件路径
set(GLAD_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)

# glad 源文件
set(GLAD_SOURCES ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)

# 添加 glad 库
add_library(glad STATIC ${GLAD_SOURCES})

# 设置 glad 库的包含路径
target_include_directories(glad PUBLIC ${GLAD_INCLUDE_DIR})
