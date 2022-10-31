#include "VulkanShader.h"
#include "ShaderUtil.h"
#include "VulkanContext.h"
#include "spirv_reflect.h"
#include "VulkanCore/Spirv/VulkanSpirvReflection.h"

std::unordered_map <std::string, std::shared_ptr<VulkanShader>> VulkanShader::s_shadersMap;


VulkanShader::VulkanShader(std::string vertexPath, std::string fragPath) {
    auto vertShaderCode = ShaderUtil::ReadFile(vertexPath);
    addDescriptorLayoutBindingsByShaderCode(vertShaderCode);
    auto fragShaderCode = ShaderUtil::ReadFile(fragPath);
    addDescriptorLayoutBindingsByShaderCode(fragShaderCode);

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
    layoutInfo.bindingCount = (uint32_t)m_descriptorSetLayoutBindings.size();
    layoutInfo.pBindings = &m_descriptorSetLayoutBindings[0];
    auto &device = VulkanContext::Get()->VulkanDevice->device;
    if (vkCreateDescriptorSetLayout(device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }
}

std::shared_ptr <VulkanShader> VulkanShader::Find(string &shaderPath) {
    auto result = s_shadersMap.find(shaderPath);
    if (result != s_shadersMap.end()) {
        return result->second;
    }
    return nullptr;
}

void VulkanShader::AddDescriptorSetLayoutBinding(VkDescriptorType descriptorType, VkShaderStageFlags stageFlags) {
    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding =(uint32_t) m_descriptorSetLayoutBindings.size();
    layoutBinding.descriptorType = descriptorType;
    layoutBinding.descriptorCount = 1;
    layoutBinding.stageFlags = stageFlags;
    layoutBinding.pImmutableSamplers = nullptr; // Optional

    m_descriptorSetLayoutBindings.push_back(layoutBinding);
}

void VulkanShader::addDescriptorLayoutBindingsByShaderCode(std::vector<char> &p_shaderCode) {
    SpvReflectShaderModule reflectShaderModule;
    SpvReflectResult result = spvReflectCreateShaderModule(p_shaderCode.size(), p_shaderCode.data(),
                                                           &reflectShaderModule);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);


    uint32_t count = 0;
    result = spvReflectEnumerateDescriptorSets(&reflectShaderModule, &count, NULL);
    assert(result == SPV_REFLECT_RESULT_SUCCESS);
    std::vector < SpvReflectDescriptorSet * > sets(count);
    result = spvReflectEnumerateDescriptorSets(&reflectShaderModule, &count, sets.data());
    assert(result == SPV_REFLECT_RESULT_SUCCESS);

    for (size_t i_set = 0; i_set < sets.size(); ++i_set) {
        const SpvReflectDescriptorSet &refl_set = *(sets[i_set]);
        for (uint32_t i_binding = 0; i_binding < refl_set.binding_count; ++i_binding) {
            const SpvReflectDescriptorBinding &refl_binding = *(refl_set.bindings[i_binding]);
            VkDescriptorSetLayoutBinding layout_binding{};
            layout_binding.binding = (uint32_t)m_descriptorSetLayoutBindings.size();
            layout_binding.descriptorType = static_cast<VkDescriptorType>(refl_binding.descriptor_type);
            layout_binding.descriptorCount = 1;
            for (uint32_t i_dim = 0; i_dim < refl_binding.array.dims_count; ++i_dim) {
                layout_binding.descriptorCount *= refl_binding.array.dims[i_dim];
            }
            layout_binding.stageFlags = static_cast<VkShaderStageFlagBits>(reflectShaderModule.shader_stage);
            std::cout << " add Descriptor " << layout_binding.descriptorType << " stage flag " << layout_binding.stageFlags << std::endl;
            m_descriptorSetLayoutBindings.push_back(layout_binding);
        }

    }
}

