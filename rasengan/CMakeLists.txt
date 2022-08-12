
CMAKE_MINIMUM_REQUIRED(VERSION 3.22)
SET(CMAKE_CXX_STANDARD 17)
PROJECT(Rasengan)

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
        src/VulkanCore/VulkanSwapChain.h src/VulkanCore/VulkanShader.cpp src/VulkanCore/VulkanShader.h src/VulkanCore/ShaderUtil.h)

FIND_PACKAGE(Vulkan REQUIRED)

INCLUDE_DIRECTORIES(
        "ThirdParty/glfw/include"
        $ENV{VK_SDK_PATH}/include
)
# 拷贝 glfw3.lib 到 输出目录 中
#第一次编译需要把 glfw3.lib复制到exe目录 不然跑不起来
EXECUTE_PROCESS(COMMAND ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_SOURCE_DIR}/glfw3.lib" "${PROJECT_BINARY_DIR}")



TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE Vulkan::Vulkan glfw3)