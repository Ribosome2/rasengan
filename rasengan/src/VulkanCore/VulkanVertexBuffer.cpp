#include "VulkanVertexBuffer.h"
#include <vulkan/vulkan.h>
#include "VulkanContext.h"
VulkanVertexBuffer::VulkanVertexBuffer(void* vertices, uint32_t size) {
    this->vertices = vertices;
	this->size= size;
	VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = size;
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    auto  vulkanDevice = VulkanContext::Get()->VulkanDevice;
    auto device = vulkanDevice->device;
    if (vkCreateBuffer(device, &bufferInfo, nullptr, &vertexBuffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    //Memory requirements
    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, vertexBuffer, &memRequirements);

    //Memory allocation
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex=    vulkanDevice->FindMemoryType(memRequirements.memoryTypeBits,VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }
    vkBindBufferMemory(device,vertexBuffer,vertexBufferMemory,0);

	UpdateMemory();
}

VulkanVertexBuffer::~VulkanVertexBuffer() {
    auto device = VulkanContext::Get()->VulkanDevice->device;
    vkDestroyBuffer(device, vertexBuffer, nullptr);

    vkDestroyBuffer(device, vertexBuffer, nullptr);
    vkFreeMemory(device, vertexBufferMemory, nullptr);

}

void VulkanVertexBuffer::UpdateMemory() {
	auto  vulkanDevice = VulkanContext::Get()->VulkanDevice;
	auto device = vulkanDevice->device;
	void* data;
	vkMapMemory(device,vertexBufferMemory,0,this->size,0,&data);
	memcpy(data,vertices,(size_t)this->size);
	vkUnmapMemory(device,vertexBufferMemory);
}

