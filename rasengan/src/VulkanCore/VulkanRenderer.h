#pragma once


#include <vulkan/vulkan.h>
#include "VulkanVertexBuffer.h"
class VulkanRenderer {
public:
    void BeginRenderPass(uint32_t imageIndex);
    void EndRenderPass();
    void BeginFrame();
    void DrawFrame(VulkanVertexBuffer & vulkanVertexBuffer);
    void EndFrame();
    void RecordCommandBuffer(VkCommandBuffer &  commandBuffer, uint32_t imageIndex);
    uint32_t imageIndex;
private:
};
