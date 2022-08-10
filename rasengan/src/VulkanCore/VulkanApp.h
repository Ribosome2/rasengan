#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include "VulkanValidation.h"
#include "VulkanDevice.h"
class VulkanApp{
    VkInstance instance;
public:
    void CreateInstance();
    void Cleanup();


private:
    VulkanValidation mValidation;
    VulkanDevice mVulkanDevice;
    std::vector<const char*> getRequiredExtensions();
};
