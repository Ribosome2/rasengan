
CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 17)
PROJECT(Rasengan)
FILE(GLOB_RECURSE IMGUI_SRC_LIST "ThirdParty/imgui/*cpp" "ThirdParty/imgui/*.h")

ADD_EXECUTABLE(${PROJECT_NAME} "src/main.cpp"
        src/VulkanCore/VulkanApp.h
        src/VulkanCore/VulkanApp.cpp
        src/VulkanCore/VulkanValidation.h
        src/VulkanCore/VulkanDevice.cpp
        src/VulkanCore/VulkanDevice.h
        src/VulkanCore/VulkanWindow.cpp
        src/VulkanCore/VulkanWindow.h
        src/VulkanCore/VulkanContext.h
        src/VulkanCore/VulkanSwapChain.cpp
        ${IMGUI_SRC_LIST}
        src/IMGUI/VulkanImguiLayer.cpp
        src/IMGUI/VulkanImguiLayer.hpp
        src/IMGUI/imgui_impl_glfw.cpp
        src/IMGUI/imgui_impl_vulkan.cpp
        src/VulkanCore/VulkanSwapChain.h
        src/VulkanCore/VulkanShader.cpp
        src/VulkanCore/VulkanShader.h
        src/VulkanCore/ShaderUtil.h
        src/VulkanCore/VulkanPipeline.cpp
        src/VulkanCore/VulkanPipeline.h
        src/VulkanCore/VulkanCommandBuffer.cpp
        src/VulkanCore/VulkanCommandBuffer.h
        src/VulkanCore/VulkanRenderer.cpp
        src/VulkanCore/VulkanRenderer.h
        src/VulkanCore/VulkanDebugUtil.h
        src/VulkanCore/VulkanInitializer.h
        src/VulkanCore/VulkanVertex.cpp
        src/VulkanCore/VulkanVertex.h
        src/VulkanCore/VulkanVertexBuffer.cpp src/VulkanCore/VulkanVertexBuffer.h src/VulkanCore/VulkanBufferHelper.h src/VulkanCore/VulkanIndexBuffer.cpp src/VulkanCore/VulkanIndexBuffer.h src/Renderring/RenderringDataDefine.h
        src/EngineCore/Time.cpp src/EngineCore/Time.h
        src/EngineCore/GameObject.cpp
        src/EngineCore/GameObject.h
        src/EngineCore/Transform.cpp
        src/EngineCore/Transform.h
        src/EngineCore/MeshRenderer.cpp
        src/EngineCore/MeshRenderer.h
        src/EngineCore/Material.cpp
        src/EngineCore/Material.h
        src/EngineCore/Mesh.cpp
        src/EngineCore/Mesh.h
        src/EngineCore/Camera.cpp
        src/EngineCore/Camera.h
        src/EngineCore/SceneViewCameraController.cpp
        src/EngineCore/SceneViewCameraController.h src/Input/Input.cpp src/Input/Input.h)

FIND_PACKAGE(Vulkan REQUIRED)
INCLUDE_DIRECTORIES(
        "src"
        "ThirdParty/glfw/include"
        $ENV{VK_SDK_PATH}/include
        ThirdParty/imgui
        src/IMGUI
        ThirdParty/glm
)
# 拷贝 glfw3.lib 到 输出目录 中
#第一次编译需要把 glfw3.lib复制到exe目录 不然跑不起来
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/glfw3.lib" "${PROJECT_BINARY_DIR}")


TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw3)