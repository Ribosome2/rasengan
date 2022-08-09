#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <GLFW/glfw3.h>
class VulkanApp{
    VkInstance instance;
public:
    void CreateInstance();

};
