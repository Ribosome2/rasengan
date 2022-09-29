#include "Material.h"
#include "Renderring/RenderringDataDefine.h"
#include "VulkanCore/VulkanBufferHelper.h"
#include "Time.h"
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>

Material::Material() {
    std::cout << "Create material " << std::endl;
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    VulkanBufferHelper::CreateBuffer(bufferSize,
                                     VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                     uniformBuffer, uniformBufferMemory);
}

void Material::UpdateUniformBuffer(Transform &transform) {
    auto vkContext = VulkanContext::Get();

    auto &swapChainExtent = vkContext->SwapChain->swapChainExtent;
    auto &device = vkContext->VulkanDevice->device;

    UniformBufferObject ubo{};
    transform.UpdateModelMatrix();
    ubo.model = transform.localToWorldMatrix;
	auto const & renderContext = vkContext->VulkanRenderer->RenderContext;
    ubo.view = renderContext.viewMatrix;
    ubo.proj = renderContext.projectionMatrix;

    void *data;
    vkMapMemory(device, uniformBufferMemory, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, uniformBufferMemory);
}

Material::~Material() {
    std::cout << "destroy Material " << this->name << std::endl;
    auto device = VulkanContext::Get()->VulkanDevice->device;
    vkDestroyBuffer(device, uniformBuffer, nullptr);
    vkFreeMemory(device, uniformBufferMemory, nullptr);
}

void Material::CreateDescriptorSets(VkDescriptorSetLayout &descriptorSetLayout) {
    std::vector<VkDescriptorSetLayout> layouts(1, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = VulkanContext::Get()->VulkanRenderer->GetDescriptorPool();
    allocInfo.descriptorSetCount = 1;//todo
    allocInfo.pSetLayouts = layouts.data();

    auto device = VulkanContext::Get()->VulkanDevice->device;

    if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }else{
        std::cout<<"CreateDescriptorSets Success "<<std::endl;
    }

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = uniformBuffer;
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;

    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;

    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr; // Optional
    descriptorWrite.pTexelBufferView = nullptr; // Optional

    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);
}
