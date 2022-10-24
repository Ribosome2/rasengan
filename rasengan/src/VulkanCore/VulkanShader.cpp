#include "VulkanShader.h"
#include "ShaderUtil.h"
#include "VulkanContext.h"
#include "spirv_reflect.h"
#include "VulkanCore/Spirv/VulkanSpirvReflection.h"
std::unordered_map<std::string, std::shared_ptr<VulkanShader> > VulkanShader::s_shadersMap;



VulkanShader::VulkanShader(std::string vertexPath, std::string fragPath) {
    auto vertShaderCode = ShaderUtil::ReadFile(vertexPath);
    VulkanSpirvReflection::SpirvReflectExample(vertShaderCode.data(), vertShaderCode.size());
    auto fragShaderCode = ShaderUtil::ReadFile(fragPath);
    VulkanSpirvReflection::SpirvReflectExample(fragShaderCode.data(), fragShaderCode.size());
    auto vkContext = VulkanContext::Get();
    vertShaderModule = ShaderUtil::CreateShaderModule(vkContext->VulkanDevice->device, vertShaderCode);
    fragShaderModule = ShaderUtil::CreateShaderModule(vkContext->VulkanDevice->device, fragShaderCode);

    vertShaderStageInfo = ShaderUtil::GetShaderStageCreateInfo(vertShaderModule, VK_SHADER_STAGE_VERTEX_BIT);
    fragShaderStageInfo = ShaderUtil::GetShaderStageCreateInfo(fragShaderModule, VK_SHADER_STAGE_FRAGMENT_BIT);


}

VulkanShader::~VulkanShader() {
    std::cout << "destroy shader " << std::endl;
    auto &device = VulkanContext::Get()->VulkanDevice->device;
    vkDestroyShaderModule(device, fragShaderModule, nullptr);
    vkDestroyShaderModule(device, vertShaderModule, nullptr);
    vkDestroyDescriptorSetLayout(device, descriptorSetLayout, nullptr);
}

void VulkanShader::CreateDescriptorSetLayout() {
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = m_descriptorSetLayoutBindings.size();
    layoutInfo.pBindings = &m_descriptorSetLayoutBindings[0];
    auto &device = VulkanContext::Get()->VulkanDevice->device;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

std::shared_ptr<VulkanShader> VulkanShader::Find(string &shaderPath) {
    auto result = s_shadersMap.find(shaderPath);
    if (result != s_shadersMap.end()) {
        return result->second;
    }
    return nullptr;
}

void VulkanShader::AddDescriptorSetLayoutBinding(VkDescriptorType descriptorType, VkShaderStageFlags stageFlags) {
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = m_descriptorSetLayoutBindings.size();
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = stageFlags;
    layoutBinding.pImmutableSamplers = nullptr; // Optional

    m_descriptorSetLayoutBindings.push_back(layoutBinding);
}

