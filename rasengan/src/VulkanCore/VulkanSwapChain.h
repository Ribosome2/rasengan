#pragma once
#include "vulkan/vulkan.h"
#include "GLFW/glfw3.h"

class VulkanSwapChain {
public:
    VkSurfaceKHR surface;
    void InitSurface(GLFWwindow* windowHandle);
};
