#pragma once

#include "VulkanCore/VulkanPipeline.h"
#include "VulkanCore/VulkanShader.h"
#include "GameObject.h"

class Material {
public:
    Material();
    ~Material();
    void CreateDescriptorSets(VkDescriptorSetLayout &descriptorSetLayout);
    void UpdateUniformBuffer(Transform &transform);
public:
    VulkanShader*  shader;
    VulkanPipeline *  pipeline;
    VkDescriptorSet descriptorSet;
    VkBuffer uniformBuffer;
    VkDeviceMemory uniformBufferMemory;
};
