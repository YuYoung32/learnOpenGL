cmake_minimum_required(VERSION 3.28)
project(learnOpenGL)

set(CMAKE_CXX_STANDARD 17)

# 引入第三方库
add_subdirectory(thirdparty/glfw-3.4)
add_subdirectory(thirdparty/glad)

# 包含各类头文件的目录
include_directories(src)

# 查找OpenGL库
find_package(OpenGL REQUIRED)

# 收集源文件
file(GLOB_RECURSE SOURCE_FILES
        src/*.cpp
        src/*.h)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES})

# 链接GLFW库和OpenGL库
target_link_libraries(${PROJECT_NAME} glfw glad OpenGL::GL)
