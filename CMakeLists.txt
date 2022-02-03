cmake_minimum_required(VERSION 3.0.0)
project(dungeon-crawler VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories("./include/")
link_directories("./libs/")

link_libraries(glfw3 assimp-vc140-mt opengl32 gdi32 BulletCollision_Debug Bullet3Common BulletDynamics_Debug LinearMath_Debug)

add_executable(dungeon-crawler ./source/main.cpp ./source/standard-functions.cpp ./source/glad.c ./source/model.cpp ./source/camera.cpp ./source/scripts/camera-controller.cpp ./source/screen-quad.cpp ./source/physics-functions.cpp ./source/object.cpp)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
