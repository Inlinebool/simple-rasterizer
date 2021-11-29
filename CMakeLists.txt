cmake_minimum_required(VERSION 3.8)
project(simple_rasterizer)

set(CMAKE_CXX_STANDARD 11)

find_package(SDL2)

include_directories(${SDL2_INCLUDE_DIRS})
set(SOURCE_FILES main.cpp Matrix.h Utils.cpp Utils.h Scene.cpp Scene.h
        Surface.cpp Surface.h TriMesh.cpp TriMesh.h Camera.cpp Camera.h Color.h LightSource.h LightSource.cpp Renderer.cpp Renderer.h Image.h)
add_executable(simple_rasterizer ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES})