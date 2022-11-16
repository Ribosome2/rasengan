#pragma once

#include <stdexcept>
#include "VulkanRenderer.h"
#include "VulkanContext.h"
#include "VulkanDebugUtil.h"
#include "imgui.h"
#include "VulkanInitializer.h"
#include "VulkanBufferHelper.h"
#include "Rendering/RenderingDataDefine.h"

#define GLM_FORCE_RADIANS

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <chrono>
#include "EngineCore/Time.h"
#include "string"

bool VulkanRenderer::useWireFramePipeline = false;

void VulkanRenderer::RecordCommandBuffer(VkCommandBuffer &commandBuffer, uint32_t imageIndex) {

    auto vkContext = VulkanContext::Get();


    auto &material = RenderContext.material;
    auto &pipeline = material->pipeline;
    if (useWireFramePipeline) {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->wireFramePipeline);
    } else {
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline->graphicsPipeline);
    }
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, material->pipeline->pipelineLayout, 0, 1,
                            &material->descriptorSet, 0, nullptr);
    RenderContext.meshRenderer->Render();

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

    std::array<VkClearValue, 2> clearValues{};
    clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
    clearValues[1].depthStencil = {1.0f, 0};

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();

    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


    auto &swapChainExtent = vkContext->SwapChain->swapChainExtent;
    VkViewport viewport = VulkanInitializer::GetViewPort((float) swapChainExtent.width, (float) swapChainExtent.height);
    vkCmdSetViewport(commandBuffer, 0, 1, &viewport);

    VkRect2D scissor{};
    scissor.offset = {0, 0};
    scissor.extent = swapChainExtent;
    vkCmdSetScissor(commandBuffer, 0, 1, &scissor);
}


void VulkanRenderer::DrawFrame(std::vector<std::shared_ptr<GameObject>> gameObjects) {
    auto vkContext = VulkanContext::Get();
    auto &commandBuffer = vkContext->CommandBuffer.GetCurCommandBuffer();

    for (auto &go: gameObjects) {
        RenderContext.meshRenderer = go->meshRenderer.get();
        RenderContext.material = go->meshRenderer->material.get();
        RecordCommandBuffer(commandBuffer, imageIndex);
    }
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

    //acquireImage
    VkResult result = vkAcquireNextImageKHR(device, swapChain, UINT64_MAX, imageAvailableSemaphore, VK_NULL_HANDLE,
                                            &imageIndex);
    // Although many drivers and platforms trigger VK_ERROR_OUT_OF_DATE_KHR automatically after a window resize, it is not guaranteed to happen.
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR ) {
        vkContext->SwapChain->RecreateSwapChain();
        return;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("failed to acquire swap chain image!");
    }
    vkResetFences(device, 1, &inFlightFence);

    auto &commandBuffer = vkContext->CommandBuffer.GetCurCommandBuffer();
    vkResetCommandBuffer(commandBuffer, 0);
    BeginRenderPass(imageIndex);
    updateCameraMatrix();
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
    auto result =vkQueuePresentKHR(presentQueue, &presentInfo);
    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        vkContext->SwapChain->RecreateSwapChain();
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("failed to present swap chain image!");
    }
}

void VulkanRenderer::Init() {


    createDescriptorPool();
}


VulkanRenderer::~VulkanRenderer() {
    std::cout << "VulkanRenderer Deconstruction " << std::endl;
    auto device = VulkanContext::Get()->VulkanDevice->device;

    vkDestroyRenderPass(device, VulkanContext::Get()->renderPass, nullptr);
    vkDestroyDescriptorPool(device, descriptorPool, nullptr);

}

VulkanRenderer::VulkanRenderer() {
    std::cout << "VulkanRenderer constructor " << std::endl;
}

void VulkanRenderer::createDescriptorPool() {
    auto maxDescriptorSetCount = 10;

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = maxDescriptorSetCount;
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = maxDescriptorSetCount;

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = poolSizes.size();
    poolInfo.pPoolSizes = poolSizes.data();

    poolInfo.maxSets = maxDescriptorSetCount;

    auto device = VulkanContext::Get()->VulkanDevice->device;
    if (vkCreateDescriptorPool(device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor pool!");
    }
}

void VulkanRenderer::Update() {
    auto vkContext = VulkanContext::Get();
    auto swapchainExtent = vkContext->SwapChain->swapChainExtent;
    camera.aspect = swapchainExtent.width / (float) swapchainExtent.height;
    ImGui::Begin("RenderSetting");
    float fps = 1 / Time::deltaTime;
    ImGui::Text("FPS: %.f", fps);
    ImGui::Checkbox("useWireFramePipeline", &useWireFramePipeline);
    camera.OnGUI();
    ImGui::End();
    cameraController.Update();
}

void VulkanRenderer::updateCameraMatrix() {
    camera.UpdateDataMatrix();
    RenderContext.projectionMatrix = camera.projectionMatrix;
    RenderContext.viewMatrix = camera.worldToCameraMatrix;
}



