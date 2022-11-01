
CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 17)
PROJECT(Rasengan)
FILE(GLOB_RECURSE IMGUI_SRC_LIST "ThirdParty/imgui/*cpp" "ThirdParty/imgui/*.h")
FILE(GLOB_RECURSE VULKAN_CORE_LIST "src/VulkanCore/*cpp" "src/VulkanCore/*.h")
FILE(GLOB_RECURSE Engine_CORE_LIST "src/EngineCore/*cpp" "src/EngineCore/*.h")
FILE(GLOB_RECURSE EDITOR_CORE_LIST "src/EditorCore/*cpp" "src/EditorCore/*.h")

ADD_EXECUTABLE(${PROJECT_NAME} "src/main.cpp"
        ${VULKAN_CORE_LIST}
        ${IMGUI_SRC_LIST}
        ${Engine_CORE_LIST}
        ${EDITOR_CORE_LIST}
        ThirdParty/stb/stb_image.h
        ThirdParty/tinyobjloader/tiny_obj_loader.h
        src/IMGUI/VulkanImguiLayer.cpp
        src/IMGUI/VulkanImguiLayer.hpp
        src/IMGUI/imgui_impl_glfw.cpp
        src/IMGUI/imgui_impl_vulkan.cpp
        src/IMGUI/ImGuiEx.cpp
        src/IMGUI/ImGuiEx.h
        src/Input/Input.cpp
        src/Input/Input.h
        ThirdParty/spirv-reflect/spirv_reflect.h
        ThirdParty/spirv-reflect/spirv_reflect.cpp
        src/VulkanCore/Spirv/VulkanSpirvCommon.h
        src/VulkanCore/Spirv/VulkanSpirvCommon.cpp
        src/VulkanCore/Spirv/VulkanSpirvOutputStreamUtil.h
        src/VulkanCore/Spirv/VulkanSpirvOutputStreamUtil.cpp
        src/VulkanCore/Spirv/VulkanSpirvReflection.h
        src/VulkanCore/Spirv/VulkanSpirvReflection.cpp
        src/Rendering/RenderingDataDefine.h
        src/DemoScene.h src/EngineCore/ModelLoader.cpp src/EngineCore/ModelLoader.h)


FIND_PACKAGE(Vulkan REQUIRED)
INCLUDE_DIRECTORIES(
        "src"
        "ThirdParty/glfw/include"
        $ENV{VK_SDK_PATH}/include
        ThirdParty/imgui
        src/IMGUI
        ThirdParty/glm
        ThirdParty/stb
        ThirdParty/spirv-reflect
        ThirdParty/tinyobjloader
)
# 拷贝 glfw3.lib 到 输出目录 中
#第一次编译需要把 glfw3.lib复制到exe目录 不然跑不起来
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/glfw3.lib" "${PROJECT_BINARY_DIR}")


TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw3)