cmake_minimum_required(VERSION 3.0.0)
project(dungeon-crawler VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories("./include/")
link_directories("./libs/")

link_libraries(glfw3 opengl32 gdi32)

add_executable(dungeon-crawler ./source/main.cpp ./source/glad.c)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
