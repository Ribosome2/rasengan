#pragma once

#include <stdexcept>
#include "VulkanRenderer.h"
#include "VulkanContext.h"
#include "VulkanDebugUtil.h"
#include "imgui.h"
#include "VulkanInitializer.h"
#include "VulkanBufferHelper.h"
#include "Renderring/RenderringDataDefine.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include "EngineCore/Time.h"
#include "string"
void VulkanRenderer::RecordCommandBuffer(VkCommandBuffer &commandBuffer, uint32_t imageIndex) {

    auto vkContext = VulkanContext::Get();
    static bool useWireFramePipeline = false;
    ImGui::Checkbox("useWireFramePipeline", &useWireFramePipeline);
    if (useWireFramePipeline) {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *vkContext->wireframePipeline);
    } else {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *vkContext->graphicsPipeline);
    }
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *RenderContext.pipelineLayout, 0, 1, &descriptorSet, 0, nullptr);
    auto indicesCount = RenderContext.indexBuffer->GetCount();
    vkCmdDrawIndexed(commandBuffer, indicesCount, 1, 0, 0, 0);
//    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

}

void VulkanRenderer::BeginRenderPass(uint32_t imageIndex) {
    auto vkContext = VulkanContext::Get();
    VkCommandBufferBeginInfo beginInfo = VulkanInitializer::GetCommandBufferBeginInfo();
    auto &commandBuffer = vkContext->CommandBuffer.GetCurCommandBuffer();


    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }


    auto swapchain = vkContext->SwapChain;
    VkRenderPassBeginInfo renderPassInfo = VulkanInitializer::GetRenderPassBeginInfo(vkContext->renderPass,
                                                                                     swapchain->GetCurrentFrameBuffer(
                                                                                             imageIndex));
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchain->swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;


    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


    auto &swapChainExtent = vkContext->SwapChain->swapChainExtent;
    VkViewport viewport = VulkanInitializer::GetViewPort((float) swapChainExtent.width, (float) swapChainExtent.height);
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}

/*
Wait for the previous frame to finish
Acquire an image from the swap chain
Record a command buffer which draws the scene onto that image
Submit the recorded command buffer
Present the swap chain image
 */
void VulkanRenderer::DrawFrame() {
    auto vkContext = VulkanContext::Get();
    auto &commandBuffer = vkContext->CommandBuffer.GetCurCommandBuffer();
    VkBuffer vertexBuffers[] = {RenderContext.vertexBuffer->GetVulkanBuffer()};
    VkDeviceSize offsets[] = {0};
    VkBuffer indexBuffer = {RenderContext.indexBuffer->GetIndexBuffer()};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);

    RecordCommandBuffer(commandBuffer, imageIndex);
}

void VulkanRenderer::EndRenderPass() {
    auto vkContext = VulkanContext::Get();
    auto &commandBuffer = vkContext->CommandBuffer.GetCurCommandBuffer();
    vkCmdEndRenderPass(commandBuffer);
    VK_CHECK_RESULT(vkEndCommandBuffer(commandBuffer));
}

void VulkanRenderer::BeginFrame() {
    auto vkContext = VulkanContext::Get();
    auto &device = vkContext->VulkanDevice->device;
    auto &inFlightFence = vkContext->SwapChain->inFlightFence;
    auto &swapChain = vkContext->SwapChain->swapChain;
    auto &imageAvailableSemaphore = vkContext->SwapChain->imageAvailableSemaphore;
    auto &renderFinishSemaphore = vkContext->SwapChain->renderFinishedSemaphore;

    //wait
    vkWaitForFences(device, 1, &inFlightFence, VK_TRUE, UINT64_MAX);
    vkResetFences(device, 1, &inFlightFence);

    //acquireImage
    vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE, &imageIndex);
    auto &commandBuffer = vkContext->CommandBuffer.GetCurCommandBuffer();
    vkResetCommandBuffer(commandBuffer, 0);
    BeginRenderPass(imageIndex);
}

void VulkanRenderer::EndFrame() {
    EndRenderPass();
    auto vkContext = VulkanContext::Get();
    auto &device = vkContext->VulkanDevice->device;
    auto &inFlightFence = vkContext->SwapChain->inFlightFence;
    auto &swapChain = vkContext->SwapChain->swapChain;
    auto &imageAvailableSemaphore = vkContext->SwapChain->imageAvailableSemaphore;
    auto &renderFinishSemaphore = vkContext->SwapChain->renderFinishedSemaphore;
    auto &commandBuffer = vkContext->CommandBuffer.GetCurCommandBuffer();
    //submit
    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphore};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    VkSemaphore signalSemaphores[] = {renderFinishSemaphore};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    auto &graphicsQueue = vkContext->VulkanDevice->graphicsQueue;
    VK_CHECK_RESULT(vkQueueSubmit(graphicsQueue, 1, &submitInfo, inFlightFence));


    //present

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapChain};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;
    presentInfo.pImageIndices = &imageIndex;
    presentInfo.pResults = nullptr; // Optional
    auto &presentQueue = vkContext->VulkanDevice->presentQueue;
    VK_CHECK_RESULT(vkQueuePresentKHR(presentQueue, &presentInfo));
}

void VulkanRenderer::Init() {
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    VulkanBufferHelper::CreateBuffer(bufferSize,
                                     VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
                                     VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
                                     RenderContext.uniformBuffer, RenderContext.uniformBufferMemory);

    createDescriptorPool();
}

void VulkanRenderer::UpdateUniformBuffer() {

    auto vkContext = VulkanContext::Get();
    float fps = 1/Time::deltaTime;
    ImGui::Text("FPS: %.f",fps);
    auto &swapChainExtent = vkContext->SwapChain->swapChainExtent;
    auto &device = vkContext->VulkanDevice->device;

    UniformBufferObject ubo{};
    auto modelMatrix = glm::mat4(1.0f);
    static auto modelWorldPosition  = glm::vec3{0.0,0,0};
    ImGui::SliderFloat3("ModelPosition",(float*)&modelWorldPosition,-10,10);
    modelMatrix = glm::translate(modelMatrix,modelWorldPosition);
    modelMatrix= glm::rotate(modelMatrix, Time::realtimeSinceStartup * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.model = modelMatrix;

    static auto eyePos = glm::vec3(0.0f, 2.0f, 2.0f);
    ImGui::SliderFloat3("eyePos ",(float*)&eyePos,-10,10);
    static auto lookAtCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    ImGui::SliderFloat3("lookAtCenter ",(float*)&lookAtCenter,-10,10);
    ubo.view = glm::lookAt(eyePos, lookAtCenter, glm::vec3(0.0f, 1.0f, 0.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), swapChainExtent.width / (float) swapChainExtent.height, 0.1f,
                                10.0f);
    ubo.proj[1][1] *= -1;

    void *data;
    vkMapMemory(device, RenderContext.uniformBufferMemory, 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(device, RenderContext.uniformBufferMemory);
}

VulkanRenderer::~VulkanRenderer() {
    std::cout << "VulkanRenderer Deconstruction " << std::endl;
    auto device = VulkanContext::Get()->VulkanDevice->device;

    vkDestroyBuffer(device, RenderContext.uniformBuffer, nullptr);
    vkFreeMemory(device, RenderContext.uniformBufferMemory, nullptr);
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);

}

VulkanRenderer::VulkanRenderer() {
    std::cout << "VulkanRenderer constructor " << std::endl;
}

void VulkanRenderer::createDescriptorPool() {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = 1; //todo: for now we only have one

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    poolInfo.maxSets = poolSize.descriptorCount;

    auto device = VulkanContext::Get()->VulkanDevice->device;
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void VulkanRenderer::CreateDescriptorSets(VkDescriptorSetLayout & descriptorSetLayout ) {
    std::vector<VkDescriptorSetLayout> layouts(1, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = 1 ;//todo
    allocInfo.pSetLayouts = layouts.data();

    auto device = VulkanContext::Get()->VulkanDevice->device;

    if (vkAllocateDescriptorSets(device, &allocInfo, &descriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    VkDescriptorBufferInfo bufferInfo{};
    bufferInfo.buffer = RenderContext.uniformBuffer;
    bufferInfo.offset = 0;
    bufferInfo.range = sizeof(UniformBufferObject);

    VkWriteDescriptorSet descriptorWrite{};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = descriptorSet;
    descriptorWrite.dstBinding = 0;
    descriptorWrite.dstArrayElement = 0;

    descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    descriptorWrite.descriptorCount = 1;

    descriptorWrite.pBufferInfo = &bufferInfo;
    descriptorWrite.pImageInfo = nullptr; // Optional
    descriptorWrite.pTexelBufferView = nullptr; // Optional

    vkUpdateDescriptorSets(device, 1, &descriptorWrite, 0, nullptr);


}


