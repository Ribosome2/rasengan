#pragma once


#include <vulkan/vulkan.h>
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"

struct RenderContext{
	VulkanVertexBuffer * vertexBuffer;
	VulkanIndexBuffer * indexBuffer;
};

class VulkanRenderer {
public:
    void BeginRenderPass(uint32_t imageIndex);
    void EndRenderPass();
    void BeginFrame();
    void DrawFrame();
    void EndFrame();
    void RecordCommandBuffer(VkCommandBuffer &  commandBuffer, uint32_t imageIndex);
    uint32_t imageIndex;
	RenderContext RenderContext;
private:
};
