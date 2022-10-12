#pragma once
#include "iostream"
#include "vulkan/vulkan.h"
#include "unordered_map"
class VulkanShader {
public:
    VulkanShader(std::string vertexPath,std::string fragPath);

    void AddDescriptorSetLayoutBinding(VkDescriptorType descriptorType,VkShaderStageFlags stageFlags);
    void CreateDescriptorSetLayout();


    ~VulkanShader();
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
    std::vector<VkDescriptorSetLayoutBinding> m_descriptorSetLayoutBindings ;

private:
    static std::unordered_map<std::string ,std::shared_ptr<VulkanShader> > s_shadersMap;
};
