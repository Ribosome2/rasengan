#include "Material.h"
#include "Rendering/RenderingDataDefine.h"
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

    CheckUpdateTexture(device);
}

void Material::CheckUpdateTexture(VkDevice &device) {
    if(mainTexture != nullptr && mainTexture->Generation != descriptor_generation)
    {
        descriptor_generation = mainTexture->Generation;
        VkDescriptorImageInfo image_infos[1];

        int imageIndex =0;//we only have one ,for now
        // Assign view and sampler.
        image_infos[imageIndex].imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        image_infos[imageIndex].imageView = mainTexture->textureImageView;
        image_infos[imageIndex].sampler = mainTexture->textureSampler;
        int updateIndex = descriptorWrites.size() - 1;  //todo:这里我们都是把图片的VkWriteDescriptorSet放在最后的，所以写死最后一个
        descriptorWrites[updateIndex].pImageInfo = &image_infos[imageIndex];
        // 更新描述符集，这里可以注意，vkUpdateDescriptorSets可以更新整个descriptorWrites列表，但是稍有设置不对就会一堆问题，
        // 最简单的来说我们只更新我们关心的单个VkWriteDescriptorSet 就行
        vkUpdateDescriptorSets(device, 1, &descriptorWrites[updateIndex], 0, nullptr);
    }
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
    auto deviceProperties = VulkanContext::Get()->VulkanDevice->deviceProperties;
    uint32_t minAlignment = deviceProperties.limits.minUniformBufferOffsetAlignment;

    // 确保 offset 是 minAlignment 的倍数,不然更新图片的时候会报错
    auto originalOffset = 0;
    uint32_t offset = (originalOffset + minAlignment - 1) & ~(minAlignment - 1);

    bufferInfo.offset = offset;
    bufferInfo.range = sizeof(UniformBufferObject);



    VkWriteDescriptorSet descriptorWriteUniformBuffer{};
    descriptorWriteUniformBuffer.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWriteUniformBuffer.dstSet = descriptorSet;
    descriptorWriteUniformBuffer.dstBinding = descriptorWrites.size();
    descriptorWriteUniformBuffer.dstArrayElement = descriptorWrites.size();

    descriptorWriteUniformBuffer.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWriteUniformBuffer.descriptorCount = 1;
    descriptorWriteUniformBuffer.pBufferInfo = &bufferInfo;
    descriptorWriteUniformBuffer.pTexelBufferView = nullptr; // Optional
    descriptorWrites.push_back(descriptorWriteUniformBuffer);

    //add sampler descriptor if exist
    if(mainTexture!= nullptr)
    {
        VkDescriptorImageInfo imageInfo{};
        imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        imageInfo.imageView = mainTexture->textureImageView;
        imageInfo.sampler = mainTexture->textureSampler;
        this->m_DescriptorImageInfo =imageInfo;
        descriptor_generation = mainTexture->Generation;
        VkWriteDescriptorSet descriptorWriteCombinedSampler{};
        descriptorWriteCombinedSampler.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWriteCombinedSampler.dstSet = descriptorSet;
        descriptorWriteCombinedSampler.dstBinding = descriptorWrites.size();
        descriptorWriteCombinedSampler.dstArrayElement = 0;

        descriptorWriteCombinedSampler.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWriteCombinedSampler.descriptorCount = 1;
        descriptorWriteCombinedSampler.pImageInfo = &imageInfo;
        descriptorWrites.push_back(descriptorWriteCombinedSampler);
    }




    vkUpdateDescriptorSets(device, descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
}
