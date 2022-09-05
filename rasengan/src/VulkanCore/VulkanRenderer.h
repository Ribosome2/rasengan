#pragma once


#include <vulkan/vulkan.h>

class VulkanRenderer {
public:
    void BeginRenderPass();
    void EndRenderPass();
    void DrawFrame();
    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

};
