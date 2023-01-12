cmake_minimum_required(VERSION 3.24)
project(Game_Engine)

set(CMAKE_CXX_STANDARD 23)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

find_package(Vulkan REQUIRED)

add_subdirectory("libs/glfw-3.3.8")
add_subdirectory("libs/glm-0.9.9.8")

# add_library(TracyClient STATIC libs/tracy/public/TracyClient.cpp)
# target_include_directories(TracyClient PUBLIC libs/tracy/public/tracy)
# target_compile_definitions(TracyClient PUBLIC TRACY_ENABLE=1)

if(MSVC)
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup")
endif()

add_executable(Game_Engine src/main.cpp
                           src/utils/window/window.hpp
                           src/utils/window/window.cpp
                           src/utils/window/window.cpp
                           src/application.cpp
                           src/application.hpp
                           src/utils/pipeline/pipeline.cpp
                           src/utils/pipeline/pipeline.hpp
                           src/utils/device/device.cpp
                           src/utils/device/device.hpp
                           src/utils/swapchain/swapchain.cpp
                           src/utils/swapchain/swapchain.hpp
                           src/utils/model/model.cpp
                           src/utils/model/model.hpp
                           src/utils/gameobject/gameobject.cpp
                           src/utils/gameobject/gameobject.hpp
                           src/utils/renderer/renderer.cpp
                           src/utils/renderer/renderer.hpp
                           src/rendersystems/simple/simplerendersystem.cpp
                           src/rendersystems/simple/simplerendersystem.hpp
                           src/utils/camera/camera.cpp
                           src/utils/camera/camera.hpp
                           src/utils/input/keyboard_movement_controller/keyboardmovementcontroller.cpp
                           src/utils/input/keyboard_movement_controller/keyboardmovementcontroller.hpp
                           libs/tinyobjloader/tiny_obj_loader.h src/utils/utils.hpp src/utils/buffer/buffer.cpp src/utils/buffer/buffer.hpp src/utils/descriptors/descriptors.hpp src/utils/descriptors/descriptors.cpp src/utils/frameinfo/frameinfo.hpp)
target_link_libraries(Game_Engine glfw glm Vulkan::Vulkan )#TracyClient)