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

VkCommandBuffer VulkanCommandBuffer::BeginSingleTimeCommands() {
    auto vkContext = VulkanContext::Get();
    VkCommandBufferAllocateInfo allocateInfo{};
    allocateInfo.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocateInfo.level=VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocateInfo.commandPool=vkContext->CommandBuffer.commandPool;
    allocateInfo.commandBufferCount=1;

    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(vkContext->VulkanDevice->device,&allocateInfo,&commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType=VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags=VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

    vkBeginCommandBuffer(commandBuffer,&beginInfo);

    return commandBuffer;
}

void VulkanCommandBuffer::EndSingleTimeCommands(VkCommandBuffer commandBuffer) {
    vkEndCommandBuffer(commandBuffer);
    auto  context  = VulkanContext::Get();
    VkSubmitInfo submitInfo{};
    submitInfo.sType=VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount=1;
    submitInfo.pCommandBuffers=&commandBuffer;

    vkQueueSubmit(context->VulkanDevice->graphicsQueue,1,&submitInfo,VK_NULL_HANDLE);
    vkQueueWaitIdle(context->VulkanDevice->graphicsQueue);

    vkFreeCommandBuffers(context->VulkanDevice->device,context->CommandBuffer.commandPool,1,&commandBuffer);
}
