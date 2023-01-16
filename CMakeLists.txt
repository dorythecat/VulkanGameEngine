cmake_minimum_required(VERSION 3.24)
project(Game_Engine VERSION 0.0.20.1)

set(CMAKE_CXX_STANDARD 23) # I do believe this can run with just C++17, but oh well, higher is better, right?

message(STATUS "using ${CMAKE_GENERATOR}")

# GLFW settings
set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

# Find Vulkan
find_package(Vulkan REQUIRED COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)
message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find the Vulkan library!")
else()
    message(STATUS "Using the Vulkan library, located at: ${Vulkan_LIBRARIES}")
endif()

# Add subdirectories
add_subdirectory("${PROJECT_SOURCE_DIR}/libs/glfw-3.3.8")
add_subdirectory("${PROJECT_SOURCE_DIR}/libs/glm-0.9.9.8")

# Add Tracy
# add_library(TracyClient STATIC libs/tracy/public/TracyClient.cpp)
# target_include_directories(TracyClient PUBLIC libs/tracy/public/tracy)
# target_compile_definitions(TracyClient PUBLIC TRACY_ENABLE=1)

if(MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()

#==============================================================================
# COMPILE SHADERS
#==============================================================================

set(SHADER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders) # Where the shaders are located
set(SHADER_BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/res/shaders/compiled) # Where the compiled shaders will be located

# Search for the shader files
# I don't think we need all of these, but it's better to have them than not
file(GLOB SHADERS
        ${SHADER_SOURCE_DIR}/*.vert
        ${SHADER_SOURCE_DIR}/*.frag
        # ${SHADER_SOURCE_DIR}/*.comp
        # ${SHADER_SOURCE_DIR}/*.geom
        # ${SHADER_SOURCE_DIR}/*.tesc
        # ${SHADER_SOURCE_DIR}/*.tese
        # ${SHADER_SOURCE_DIR}/*.mesh
        # ${SHADER_SOURCE_DIR}/*.task
        # ${SHADER_SOURCE_DIR}/*.rgen
        # ${SHADER_SOURCE_DIR}/*.rchit
        # ${SHADER_SOURCE_DIR}/*.rmiss)

# Create the shader binaries directory, if it doesn't already exist
add_custom_command(COMMAND
                   ${CMAKE_COMMAND} -E make_directory ${SHADER_BINARY_DIR}
                   OUTPUT ${SHADER_BINARY_DIR}
                   COMMENT "Creating ${SHADER_BINARY_DIR}")

foreach(SHADER IN LISTS SHADERS) # Loop through all the shaders
    get_filename_component(FILENAME ${SHADER} NAME) # Get the filename of the shader
    add_custom_command(OUTPUT ${SHADER_BINARY_DIR}/${FILENAME}.spv
                       COMMAND ${Vulkan_GLSLC_EXECUTABLE} ${SHADER} -o ${SHADER_BINARY_DIR}/${FILENAME}.spv
                       DEPENDS ${SHADER}
                       COMMENT "Compiling ${FILENAME}") # Compile the shader
    message(STATUS "Compiling ${SHADER_SOURCE_DIR}/${FILENAME} to ${SHADER_BINARY_DIR}/${FILENAME}.spv") # Print out the shader being compiled
    list(APPEND SPV_SHADERS ${SHADER_BINARY_DIR}/${FILENAME}.spv) # Add the compiled shader to the list
endforeach()
add_custom_target(Shaders ALL DEPENDS ${SPV_SHADERS}) # This is the target that will be called when compiling the shaders

#==============================================================================
# BUILD PROJECT
#==============================================================================

file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp) # Get all the source files
add_executable(${PROJECT_NAME} ${SOURCES} ${SPV_SHADERS}) # Add the source files and shaders to the executable
add_dependencies(${PROJECT_NAME} Shaders) # Add the shaders as a dependency to the executable
target_link_libraries(${PROJECT_NAME} glfw glm Vulkan::Vulkan)#TracyClient) # Link all of the libraries

#==============================================================================