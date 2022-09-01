#include <stdexcept>
#include "VulkanCommandBuffer.h"
#include "VulkanContext.h"
#include "VulkanDevice.h"
void VulkanCommandBuffer::createCommandPool() {
    auto vkContext = VulkanContext::Get();
    QueueFamilyIndices queueFamilyIndices = vkContext->VulkanDevice->FindQueueFamilies(vkContext->VulkanDevice->physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

    if (vkCreateCommandPool(VulkanContext::Get()->VulkanDevice->device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create command pool!");
    }
}

void VulkanCommandBuffer::createCommandBuffer() {
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.commandPool = commandPool;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandBufferCount = 1;

    if (vkAllocateCommandBuffers(VulkanContext::Get()->VulkanDevice->device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate command buffers!");
    }
}

void VulkanCommandBuffer::Init() {
    createCommandPool();
    createCommandBuffer();
}

VulkanCommandBuffer::~VulkanCommandBuffer() {
}

void VulkanCommandBuffer::Dispose() {
    vkDestroyCommandPool(VulkanContext::Get()->VulkanDevice->device, commandPool, nullptr);
}
