#pragma once


#include <vulkan/vulkan.h>
#include <iostream>
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"

class RenderContext{
public:
	VulkanVertexBuffer * vertexBuffer;
	VulkanIndexBuffer * indexBuffer;
	VkPipelineLayout * pipelineLayout;
    VkBuffer uniformBuffer;
    VkDeviceMemory uniformBufferMemory;
};

class VulkanRenderer {
public:
    VulkanRenderer();

    ~VulkanRenderer();

    void Init();
    void UpdateUniformBuffer();
    void BeginRenderPass(uint32_t imageIndex);
    void EndRenderPass();
    void BeginFrame();
    void DrawFrame();
    void EndFrame();
    void RecordCommandBuffer(VkCommandBuffer &  commandBuffer, uint32_t imageIndex);
    void CreateDescriptorSets(VkDescriptorSetLayout & descriptorSetLayout);
private:
    void createDescriptorPool();
public:
    uint32_t imageIndex;
	RenderContext RenderContext;
private:
    VkDescriptorPool descriptorPool;
    VkDescriptorSet descriptorSet;
};
