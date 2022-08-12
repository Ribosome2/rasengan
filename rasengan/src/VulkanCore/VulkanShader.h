#pragma once
#include "iostream"
#include "vulkan/vulkan.h"
#include "VulkanContext.h"

class VulkanShader {
public:
    VulkanShader(std::string vertexPath,std::string fragPath);

    ~VulkanShader(){
        auto & vkContext =VulkanContext::GetContext();
        vkDestroyShaderModule(vkContext.VulkanDevice->device, fragShaderModule, nullptr);
        vkDestroyShaderModule(vkContext.VulkanDevice->device, vertShaderModule, nullptr);
    }
private:
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    VkPipelineShaderStageCreateInfo vertShaderStageInfo;
    VkPipelineShaderStageCreateInfo fragShaderStageInfo;
};
