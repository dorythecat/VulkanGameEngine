cmake_minimum_required(VERSION 3.24)
project(Game_Engine)

set(CMAKE_CXX_STANDARD 23) # I do believe this can run with just C++17, but oh well, higher is better, right?

message(STATUS "using ${CMAKE_GENERATOR}")

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

set (NAME "Game Engine")
project(NAME VERSION 0.0.20)

find_package(Vulkan REQUIRED COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)
message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find the Vulkan library!")
else()
    message(STATUS "Using the Vulkan library located at: ${Vulkan_LIBRARIES}")
endif()

add_subdirectory("${PROJECT_SOURCE_DIR}/libs/glfw-3.3.8")
add_subdirectory("${PROJECT_SOURCE_DIR}/libs/glm-0.9.9.8")

# add_library(TracyClient STATIC libs/tracy/public/TracyClient.cpp)
# target_include_directories(TracyClient PUBLIC libs/tracy/public/tracy)
# target_compile_definitions(TracyClient PUBLIC TRACY_ENABLE=1)

if(MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()

#==============================================================================
# COMPILE SHADERS
#

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders)
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/compiled)

file(GLOB SHADERS
        ${SHADER_SOURCE_DIR}/*.vert
        ${SHADER_SOURCE_DIR}/*.frag
        ${SHADER_SOURCE_DIR}/*.comp
        ${SHADER_SOURCE_DIR}/*.geom
        ${SHADER_SOURCE_DIR}/*.tesc
        ${SHADER_SOURCE_DIR}/*.tese
        ${SHADER_SOURCE_DIR}/*.mesh
        ${SHADER_SOURCE_DIR}/*.task
        ${SHADER_SOURCE_DIR}/*.rgen
        ${SHADER_SOURCE_DIR}/*.rchit
        ${SHADER_SOURCE_DIR}/*.rmiss)

add_custom_command(COMMAND
                   ${CMAKE_COMMAND} -E make_directory ${SHADER_BINARY_DIR}
                   OUTPUT ${SHADER_BINARY_DIR}
                   COMMENT "Creating ${SHADER_BINARY_DIR}")

foreach(GLSL IN LISTS SHADERS)
    get_filename_component(FILENAME ${GLSL} NAME)
    add_custom_command(OUTPUT ${SHADER_BINARY_DIR}/${FILENAME}.spv
            COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${GLSL} -o ${SHADER_BINARY_DIR}/${FILENAME}.spv
            DEPENDS ${GLSL}
            COMMENT "Compiling ${FILENAME}")
    message(...)
    list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv)
endforeach()
add_custom_target(Shaders ALL DEPENDS ${SPV_SHADERS})

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES} ${SPV_SHADERS})
add_dependencies(${PROJECT_NAME} Shaders)
target_link_libraries(${PROJECT_NAME} glfw glm Vulkan::Vulkan)#TracyClient)