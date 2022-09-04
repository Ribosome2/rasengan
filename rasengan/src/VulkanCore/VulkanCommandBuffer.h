#pragma once


#include <vulkan/vulkan_core.h>

class VulkanCommandBuffer {
public:
    void Dispose();
    void Init();
     ~VulkanCommandBuffer();


private:
    void createCommandPool();
    void createCommandBuffer();

    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
};
