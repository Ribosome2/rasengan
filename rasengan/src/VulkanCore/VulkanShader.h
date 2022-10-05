#pragma once
#include "iostream"
#include "vulkan/vulkan.h"
#include "unordered_map"
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

public:
    std::shared_ptr<VulkanShader> Find(std::string & shaderPath);
//    static InitShaders(std::string vertexPath,std::string fragPath,std::string shaderName);

private:
    static std::unordered_map<std::string ,std::shared_ptr<VulkanShader> > s_shadersMap;
};
