cmake_minimum_required(VERSION 3.16)

project(menv)

# Add source files
set(
    SOURCES
    main.cpp
    src/Camera.cpp
    src/Cube.cpp
    src/Shader.cpp
    src/Tokenizer.cpp
    src/Window.cpp
)

# Add header files
set(
    HEADERS
    include/core.h
    include/Camera.h
    include/Cube.h
    include/Shader.h
    include/Tokenizer.h
    include/Window.h
)

# Require GL
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(
    include
)

# Add library directories
link_directories(
    lib
)

# Add executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} "include/Skeleton.h" "src/Skeleton.cpp" "include/Joint.h" "src/Joint.cpp" "include/Skin.h" "src/Skin.cpp" "src/imgui.cpp" "src/imgui_demo.cpp" "src/imgui_draw.cpp" "src/imgui_impl_glfw.cpp" "src/imgui_impl_opengl3.cpp" "src/imgui_tables.cpp" "src/imgui_widgets.cpp" "include/imconfig.h" "include/imgui.h" "include/imgui_impl_opengl3.h" "include/imgui_internal.h" "include/imstb_rectpack.h" "include/imstb_textedit.h" "include/imstb_truetype.h" "include/imgui_impl_glfw.h" "include/imgui_impl_opengl3_loader.h")

# Link libraries
target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glew32s.lib glfw3)

# Move assets to .exe
add_custom_target(CopyShaders ALL
	COMMAND ${CMAKE_COMMAND} -E copy_directory
	"${PROJECT_SOURCE_DIR}/shaders"
	"${CMAKE_BINARY_DIR}/shaders"
)
add_dependencies(menv CopyShaders)