cmake_minimum_required(VERSION 3.10)

project(GraphicsEngine)

file(GLOB engine_SRC
     "src/*.cpp"
)


add_executable(test_model lib/glad/glad.c ${engine_SRC} tests/test1.cpp )
add_executable(test_light lib/glad/glad.c ${engine_SRC} tests/test2.cpp )
add_executable(test_scene lib/glad/glad.c ${engine_SRC} tests/test3.cpp )

add_executable(demo_chess lib/glad/glad.c ${engine_SRC} demo/chess.cpp )

target_include_directories(test_model PUBLIC includes/)
target_link_libraries(test_model glfw3 dl m GL X11 pthread assimp)

target_include_directories(test_light PUBLIC includes/)
target_link_libraries(test_light glfw3 dl m GL X11 pthread assimp)

target_include_directories(test_scene PUBLIC includes/)
target_link_libraries(test_scene glfw3 dl m GL X11 pthread assimp)

target_include_directories(demo_chess PUBLIC includes/)
target_link_libraries(demo_chess glfw3 dl m GL X11 pthread assimp)

set(CMAKE_GENERATOR "Unix Makefiles")
