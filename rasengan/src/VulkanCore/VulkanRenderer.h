#pragma once


#include <vulkan/vulkan.h>
#include <iostream>
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"
#include "EngineCore/GameObject.h"
#include "EngineCore/MeshRenderer.h"
#include "EngineCore/Material.h"

class RenderContext {
public:
    MeshRenderer * meshRenderer;
    Material * material;
};

class VulkanRenderer {
public:
	VulkanRenderer();

	~VulkanRenderer();

	void Init();

	void BeginRenderPass(uint32_t imageIndex);
	void EndRenderPass();
	void BeginFrame();
	void DrawFrame();
	void EndFrame();
	void RecordCommandBuffer(VkCommandBuffer &commandBuffer, uint32_t imageIndex);
    VkDescriptorPool &  GetDescriptorPool() { return descriptorPool;};
private:
	void createDescriptorPool();
public:
	uint32_t imageIndex;
	RenderContext RenderContext;
private:
	VkDescriptorPool descriptorPool;
};
