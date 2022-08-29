#pragma once
#include "iostream"
#include "vulkan/vulkan.h"
#include "VulkanContext.h"

class VulkanShader {
public:
    VulkanShader(std::string vertexPath,std::string fragPath);

    ~VulkanShader(){
        auto & vulkanDevice = VulkanContext::Get()->VulkanDevice;
        vkDestroyShaderModule(vulkanDevice->device, fragShaderModule, nullptr);
        vkDestroyShaderModule(vulkanDevice->device, vertShaderModule, nullptr);
    }
private:
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    VkPipelineShaderStageCreateInfo vertShaderStageInfo;
    VkPipelineShaderStageCreateInfo fragShaderStageInfo;
};
