#pragma once


#include <vulkan/vulkan.h>
#include <iostream>
#include "VulkanVertexBuffer.h"
#include "VulkanIndexBuffer.h"

class RenderContext{
public:
	VulkanVertexBuffer * vertexBuffer;
	VulkanIndexBuffer * indexBuffer;
    VkBuffer uniformBuffer;
    VkDeviceMemory uniformBufferMemory;
    ~RenderContext(){
        std::cout<<"dddddddddddddddddd "<<std::endl;
    }
};

class VulkanRenderer {
public:
    VulkanRenderer();

    ~VulkanRenderer();

    void Init();
    void UpdateUniformBuffer();
    void BeginRenderPass(uint32_t imageIndex);
    void EndRenderPass();
    void BeginFrame();
    void DrawFrame();
    void EndFrame();
    void RecordCommandBuffer(VkCommandBuffer &  commandBuffer, uint32_t imageIndex);
    uint32_t imageIndex;
	RenderContext RenderContext;
private:
};
