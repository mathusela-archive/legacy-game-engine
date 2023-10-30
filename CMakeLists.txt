cmake_minimum_required(VERSION 3.0.0)
project(game-engine VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories("./include/")
link_directories("./libs/")

link_libraries(glfw3 assimp-vc140-mt opengl32 gdi32 BulletCollision_Debug Bullet3Common BulletDynamics_Debug LinearMath_Debug)

add_executable(game-engine ./source/main.cpp ./source/standard-functions.cpp ./source/glad.c ./source/model.cpp ./source/camera.cpp ./source/screen-quad.cpp ./source/physics-functions.cpp ./source/object.cpp ./source/scripts/character-controller.cpp ./source/terrain.cpp)

add_custom_command(TARGET game-engine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${PROJECT_SOURCE_DIR}/libs/assimp-vc140-mt.dll"
        $<TARGET_FILE_DIR:game-engine>)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
