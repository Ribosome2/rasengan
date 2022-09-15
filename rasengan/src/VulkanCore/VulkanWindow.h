#pragma once
#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>
class VulkanWindow {
public:

public:
    GLFWwindow* window;
private:
    VkSurfaceKHR surface;
};
