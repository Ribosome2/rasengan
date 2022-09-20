#pragma once

#include <stdexcept>
#include "VulkanRenderer.h"
#include "VulkanContext.h"
#include "VulkanDebugUtil.h"
#include "imgui.h"
#include "VulkanInitializer.h"
void VulkanRenderer::RecordCommandBuffer(VkCommandBuffer &commandBuffer, uint32_t imageIndex) {

    auto vkContext = VulkanContext::Get();
	static  bool  useWireFramePipeline = false;
	ImGui::Checkbox("useWireFramePipeline",&useWireFramePipeline);
	if(useWireFramePipeline)
	{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *vkContext->wireframePipeline);
	}else{
		vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, *vkContext->graphicsPipeline);
	}
	auto indicesCount = RenderContext.indexBuffer->GetCount();
	vkCmdDrawIndexed(commandBuffer, indicesCount, 1, 0, 0, 0);
//    vkCmdDraw(commandBuffer, 3, 1, 0, 0);

}

void VulkanRenderer::BeginRenderPass(uint32_t imageIndex) {
    auto vkContext = VulkanContext::Get();
    VkCommandBufferBeginInfo beginInfo =VulkanInitializer::GetCommandBufferBeginInfo();
    auto &commandBuffer = vkContext->CommandBuffer.GetCurCommandBuffer();


    if (vkBeginCommandBuffer(commandBuffer, &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("failed to begin recording command buffer!");
    }


	auto swapchain = vkContext->SwapChain;
	VkRenderPassBeginInfo renderPassInfo =VulkanInitializer::GetRenderPassBeginInfo(vkContext->renderPass,swapchain->GetCurrentFrameBuffer(imageIndex));
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = swapchain->swapChainExtent;

    VkClearValue clearColor = {{{0.0f, 0.0f, 0.0f, 1.0f}}};
    renderPassInfo.clearValueCount = 1;
    renderPassInfo.pClearValues = &clearColor;


    vkCmdBeginRenderPass(commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);


    auto &swapChainExtent = vkContext->SwapChain->swapChainExtent;
	VkViewport viewport =VulkanInitializer::GetViewPort((float)swapChainExtent.width,(float)swapChainExtent.height);
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


