#pragma once


#include <vulkan/vulkan.h>

class VulkanRenderer {
public:
    void BeginRenderPass(uint32_t imageIndex);
    void EndRenderPass();
    void BeginFrame();
    void DrawFrame();
    void EndFrame();
    void RecordCommandBuffer(VkCommandBuffer &  commandBuffer, uint32_t imageIndex);
    uint32_t imageIndex;
private:
};
