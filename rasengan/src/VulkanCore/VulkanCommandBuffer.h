#pragma once


#include <vulkan/vulkan_core.h>

class VulkanCommandBuffer {
public:
    static VkCommandBuffer BeginSingleTimeCommands();

    static void EndSingleTimeCommands(VkCommandBuffer commandBuffer);

    void Dispose();

    void Init();

    ~VulkanCommandBuffer();

    VkCommandBuffer &GetCurCommandBuffer() { return commandBuffer; };

    VkCommandPool &GetCommandPool() { return commandPool; }

private:
    void createCommandPool();

    void createCommandBuffer();

    VkCommandPool commandPool;
    VkCommandBuffer commandBuffer;
};
