#pragma once


#include <vulkan/vulkan.h>

class VulkanRenderer {
public:
    void BeginRenderPass();
    void RecordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);

};
