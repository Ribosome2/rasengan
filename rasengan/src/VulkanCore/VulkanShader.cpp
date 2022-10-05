#include "VulkanShader.h"
#include "ShaderUtil.h"
#include "VulkanContext.h"
 std::unordered_map<std::string ,std::shared_ptr<VulkanShader> > VulkanShader::s_shadersMap;
VulkanShader::VulkanShader(std::string vertexPath, std::string fragPath) {
    auto vertShaderCode = ShaderUtil::ReadFile(vertexPath);
    auto fragShaderCode = ShaderUtil::ReadFile(fragPath);

    auto vkContext = VulkanContext::Get();
    vertShaderModule = ShaderUtil::CreateShaderModule(vkContext->VulkanDevice->device, vertShaderCode);
    fragShaderModule = ShaderUtil::CreateShaderModule(vkContext->VulkanDevice->device, fragShaderCode);

    vertShaderStageInfo = ShaderUtil::GetShaderStageCreateInfo(vertShaderModule, VK_SHADER_STAGE_VERTEX_BIT);
    fragShaderStageInfo = ShaderUtil::GetShaderStageCreateInfo(fragShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);
    createDescriptorSetLayout();
}

VulkanShader::~VulkanShader() {
    std::cout<<"destroy shader "<<std::endl;
    auto & device = VulkanContext::Get()->VulkanDevice->device;
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}

void  VulkanShader::createDescriptorSetLayout() {
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;
    auto & device = VulkanContext::Get()->VulkanDevice->device;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

std::shared_ptr<VulkanShader> VulkanShader::Find(string &shaderPath) {
    auto result = s_shadersMap.find(shaderPath);
    if(result!=s_shadersMap.end())
    {
        return result->second;
    }
    return nullptr;
}

