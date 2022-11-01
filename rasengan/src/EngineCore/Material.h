#pragma once

#include "VulkanCore/VulkanPipeline.h"
#include "VulkanCore/VulkanShader.h"
#include "GameObject.h"
#include "VulkanCore/VulkanTexture.h"

class Material {
public:
	Material();

	~Material();

	void CreateDescriptorSets(VkDescriptorSetLayout &descriptorSetLayout);

	void UpdateUniformBuffer(Transform &transform);

public:
	std::string name = "default";
	VulkanShader *shader = nullptr;
	VulkanPipeline *pipeline = nullptr;
	VkDescriptorSet descriptorSet = nullptr;
	VkBuffer uniformBuffer = nullptr;
	VkDeviceMemory uniformBufferMemory = nullptr;
	VulkanTexture *mainTexture = nullptr;
};
