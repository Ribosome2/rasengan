#include "VulkanIndexBuffer.h"
#include "VulkanBufferHelper.h"
VulkanIndexBuffer::VulkanIndexBuffer(void *indices,size_t indexCount, size_t bufferSize) {
	this->indices= indices;
	this->bufferSize = bufferSize;
	this->indicesCount= indexCount;
	auto device = VulkanContext::Get()->VulkanDevice->device;
	VkBuffer stagingBuffer;
	VkDeviceMemory stagingBufferMemory;
	VulkanBufferHelper::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

	void* data;
	vkMapMemory(device, stagingBufferMemory, 0, bufferSize, 0, &data);
	memcpy(data, indices, (size_t) bufferSize);
	vkUnmapMemory(device, stagingBufferMemory);

	VulkanBufferHelper::CreateBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

	VulkanBufferHelper::CopyBuffer(stagingBuffer, indexBuffer, bufferSize);

	vkDestroyBuffer(device, stagingBuffer, nullptr);
	vkFreeMemory(device, stagingBufferMemory, nullptr);
}

VulkanIndexBuffer::~VulkanIndexBuffer() {
	auto device = VulkanContext::Get()->VulkanDevice->device;
	vkDestroyBuffer(device, indexBuffer, nullptr);
	vkFreeMemory(device, indexBufferMemory, nullptr);

}
