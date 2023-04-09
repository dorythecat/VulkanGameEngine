cmake_minimum_required(VERSION 3.24)
project(Game_Engine)

set(CMAKE_CXX_STANDARD 23)

message(STATUS "using ${CMAKE_GENERATOR}")

# This isn't really necessary, but it's nice to have to avoid errors
if(NOT CMAKE_BUILD_TYPE)
    message(WARNING "No build type selected, defaulting to Debug")
    set(CMAKE_BUILD_TYPE Debug)
endif()

# For knowing what these do, and where I have gotten this information, check the awesome article at https://interrupt.memfault.com/blog/best-and-worst-gcc-clang-compiler-flags (I know it's for embedded systems, but it's still a  pretty good article)
# For more info on all of these warning flags, check out https://gcc.gnu.org/onlinedocs/gcc-3.4.0/gcc/Warning-Options.html
# For more information of all of these optimization flags, check out https://gcc.gnu.org/onlinedocs/gcc-12.2.0/gcc/Optimize-Options.html
set(CMAKE_CXX_FLAGS "-static -Wall -Wextra -Wundef -Wdouble-promotion -Wformat=2 -fno-common -Wconversion -pedantic -Wno-unused-parameter") # Bundle the necessary static libraries, as well as enable some useful warnings, and disable some nasty ones
set(CMAKE_CXX_FLAGS_DEBUG "-g3 -Og -DDEBUG") # Enable debug symbols and optimisations for debug builds
set(CMAKE_CXX_FLAGS_RELEASE "-O3") # Enable full optimisations for release builds

# GLFW settings
option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)
option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)
option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)
option(GLFW_INSTALL "Generate installation target" OFF)
option(GLFW_DOCUMENT_INTERNALS "Include internals in documentation" OFF)

# Find Vulkan
find_package(Vulkan REQUIRED COMPONENTS glslc)
find_program(glslc_executable NAMES glslc HINTS Vulkan::glslc)
message(STATUS "Found Vulkan: $ENV{VULKAN_SDK}")

if (NOT Vulkan_FOUND)
    message(FATAL_ERROR "Could not find the Vulkan library!")
else()
    message(STATUS "Using the Vulkan library, located at: ${Vulkan_LIBRARIES}")
endif()

set(IMGUI_DIR ${PROJECT_SOURCE_DIR}/libs/imgui)
set(IMGUI_SOURCES ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
                  ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
                  ${IMGUI_DIR}/imgui.cpp
                  ${IMGUI_DIR}/imgui_draw.cpp
                  ${IMGUI_DIR}/imgui_tables.cpp
                  ${IMGUI_DIR}/imgui_widgets.cpp)

# Add subdirectories
add_subdirectory("${PROJECT_SOURCE_DIR}/libs/glfw-3.3.8")
add_subdirectory("${PROJECT_SOURCE_DIR}/libs/glm-0.9.9.8")

# Include directories
include_directories("${PROJECT_SOURCE_DIR}/libs/glfw-3.3.8/include")
include_directories("${PROJECT_SOURCE_DIR}/libs/stb_image")
include_directories(${IMGUI_DIR} ${IMGUI_DIR}/backends)

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
        ${SHADER_SOURCE_DIR}/*.frag)
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
add_executable(${PROJECT_NAME} ${SOURCES} ${SPV_SHADERS} ${IMGUI_SOURCES} libs/stb_image/stb_image.h) # Add the source files and shaders to the executable
target_link_libraries(Game_Engine glfw glm Vulkan::Vulkan )#TracyClient)
target_compile_definitions(${PROJECT_NAME} PUBLIC -DImTextureID=ImU64) # Define the ImTextureID as an ImU64

#==============================================================================