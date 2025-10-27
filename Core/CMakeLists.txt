#
# Core
#

set(SOURCES
vendor/stb/stb_image.cpp
Source/Application.cpp
Source/Window.cpp
Source/Camera.cpp
Source/Input.cpp
Source/Renderer/Renderer.cpp
Source/Renderer/Shader.cpp
Source/Renderer/GLUtils.cpp
)

add_library(Core STATIC)

target_sources(Core PRIVATE ${SOURCES})

# Match VS filters to directory structure on disk
source_group(TREE "${CMAKE_CURRENT_SOURCE_DIR}" FILES ${SOURCES})

add_compile_definitions(
    GLFW_INCLUDE_NONE
)

target_link_libraries(Core glfw glad glm imgui)

target_include_directories(Core PUBLIC "Include" "vendor/stb")

Format(Core .)

AddCppCheck(Core)
