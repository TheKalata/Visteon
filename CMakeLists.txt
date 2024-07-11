#CMakeLists.txt

cmake_minimum_required(VERSION 3.12)
project(GLTFLOADER VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(GLFW_BUILD_X11 OFF)

add_executable(GLTFLoader
    src/main.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/src/glad.c
)

set(GLFW_BUILD_WAYLAND ON)

add_subdirectory(thirdparty/glfw/)

if(true)

    target_include_directories(GLTFLoader PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/glad/include
    )
endif()

target_include_directories(GLTFLoader PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/tinyGLTF
)

target_compile_definitions(GLTFLoader PRIVATE
    -DTINYGLTF_IMPLEMENTATION
    -DSTB_IMAGE_IMPLEMENTATION
    -DSTB_IMAGE_WRITE_IMPLEMENTATION
)

target_link_libraries(GLTFLoader PRIVATE
    glfw
)

if(WIN32)
    target_link_libraries(GLTFLoader PRIVATE
        opengl32
    )
elseif(UNIX)
    target_link_libraries(GLTFLoader PRIVATE
        GL
    )
    endif()