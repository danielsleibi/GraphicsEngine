cmake_minimum_required(VERSION 3.10)

project(GraphicsEngine)

file(GLOB engine_SRC
     "src/*.cpp"
)


add_executable(test_model lib/glad/glad.c tests/test1.cpp ${engine_SRC})
add_executable(test_light lib/glad/glad.c tests/test2.cpp ${engine_SRC})

target_include_directories(test_model PUBLIC includes/)
target_link_libraries(test_model glfw3 dl m GL X11 pthread assimp)


target_include_directories(test_light PUBLIC includes/)
target_link_libraries(test_light glfw3 dl m GL X11 pthread assimp)

set(CMAKE_GENERATOR "Unix Makefiles")
