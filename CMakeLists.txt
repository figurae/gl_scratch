cmake_minimum_required(VERSION 3.30.2)
project(OpenGlScratch VERSION 0.1.0)

add_subdirectory(deps/glfw-3.4)
include_directories(src include)

set (deps
    deps/glad/gl.c)
set (sources
    ${deps}
    src/main.cxx
    src/init.cxx
    src/graphics/ShaderProgram.cxx
    src/graphics/VertexArray.cxx
    src/graphics/VertexData.cxx
    src/scenes/SceneManager.cxx
    src/scenes/TriangleScene.cxx)

add_executable(OpenGlScratch ${sources})
target_compile_features(OpenGlScratch PRIVATE cxx_std_23)
target_link_libraries(OpenGlScratch PRIVATE glfw)

set(assets
    scenes/triangle/shaders/fragment.glsl
    scenes/triangle/shaders/vertex.glsl)
foreach(asset ${assets})
    file(GENERATE OUTPUT ${asset} INPUT ${asset})
endforeach()
