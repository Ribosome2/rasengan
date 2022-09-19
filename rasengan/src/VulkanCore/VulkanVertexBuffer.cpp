#include "VulkanVertexBuffer.h"
#include <vulkan/vulkan.h>
#include "VulkanContext.h"
#include "VulkanBufferHelper.h"

VulkanVertexBuffer::VulkanVertexBuffer(void* vertices, uint32_t size) {
    this->vertices = vertices;
	this->size= size;

    VulkanBufferHelper::CreateBuffer(size,VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                     vertexBuffer,vertexBufferMemory);
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

