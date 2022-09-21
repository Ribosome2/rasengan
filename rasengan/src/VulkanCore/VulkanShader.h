#pragma once
#include "iostream"
#include "vulkan/vulkan.h"
#include "VulkanContext.h"

class VulkanShader {
public:
    VulkanShader(std::string vertexPath,std::string fragPath);

    ~VulkanShader();
private:
    void createDescriptorSetLayout();
public:
    VkShaderModule vertShaderModule;
    VkShaderModule fragShaderModule;
    VkPipelineShaderStageCreateInfo vertShaderStageInfo;
    VkPipelineShaderStageCreateInfo fragShaderStageInfo;
    VkDescriptorSetLayout descriptorSetLayout;
};
