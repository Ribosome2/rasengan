#pragma once
#include "iostream"
#include "vulkan/vulkan.h"
#include "unordered_map"
#include "spirv_reflect.h"

class VulkanShader {
public:
    VulkanShader(std::string vertexPath,std::string fragPath);
    void AddDescriptorSetLayoutBinding(VkDescriptorType descriptorType,VkShaderStageFlags stageFlags);
    void CreateDescriptorSetLayout();
    VkVertexInputBindingDescription  GetVertexInputBindingDescription(){ return m_vertexInputBindingDescription; };
    std::vector<VkVertexInputAttributeDescription>  GetVertexInputInputAttributeDescription(){ return m_vertexAttributeDescriptions; };
    ~VulkanShader();
private:
    void addDescriptorLayoutBindingsByShaderCode(std::vector<char>&   p_shaderCode);
    void initVertexInputBindingDescription(SpvReflectShaderModule & reflectVertShaderModule);
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
    VkVertexInputBindingDescription m_vertexInputBindingDescription;
    std::vector<VkVertexInputAttributeDescription> m_vertexAttributeDescriptions;
private:
    static std::unordered_map<std::string ,std::shared_ptr<VulkanShader> > s_shadersMap;
};
