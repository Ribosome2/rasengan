#pragma once


#include <vulkan/vulkan_core.h>

class VulkanCommandBuffer {
public:
    void Dispose();
    void Init();
     ~VulkanCommandBuffer();
    VkCommandBuffer & GetCurCommandBuffer(){ return commandBuffer;};

private:
    void createCommandPool();
    void createCommandBuffer();

    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
};
