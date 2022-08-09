#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include "VulkanValidation.h"
class VulkanApp{
    VkInstance instance;
public:
    void CreateInstance();
    void Cleanup();


private:
    VulkanValidation mValidation;
    std::vector<const char*> getRequiredExtensions();
};
