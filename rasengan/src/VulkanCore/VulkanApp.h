#pragma once
#include <vulkan/vulkan.h>
#include <stdexcept>
#include <GLFW/glfw3.h>
#include "VulkanValidation.h"
#include "VulkanDevice.h"
#include "VulkanWindow.h"
#include "VulkanRenderer.h"

class VulkanApp{
    VkInstance instance;
public:
    void CreateInstance();
    void Cleanup();
    void DrawFrame();

private:
    VulkanValidation mValidation;
    VulkanDevice mVulkanDevice;
    VulkanRenderer mVulkanRenderer;
    std::vector<const char*> getRequiredExtensions();

};
