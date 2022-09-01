#pragma once


#include <vulkan/vulkan_core.h>

class VulkanCommandBuffer {
public:
    void Dispose();
    void Init();
    virtual ~VulkanCommandBuffer();

private:
    void createCommandPool();
    void createCommandBuffer();

    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
};
