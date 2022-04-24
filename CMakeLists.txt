cmake_minimum_required(VERSION 3.10)

project(LearningOpenGL)

add_executable(HelloWorld src/main.cpp)

target_include_directories(HelloWorld PUBLIC includes/)
#target_link_libraries(HelloWorld glfw3 dl m GL X11 pthread)

set(CMAKE_GENERATOR "Unix Makefiles")
