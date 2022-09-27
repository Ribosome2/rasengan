#include "MeshRenderer.h"
#include "imgui.h"
#include "VulkanCore/VulkanContext.h"


MeshRenderer::MeshRenderer() = default;

void MeshRenderer::Update() {
   mesh.Update();
   material.UpdateUniformBuffer(*transform);
}


void MeshRenderer::Render() {
    mesh.Bind();
    auto vkContext = VulkanContext::Get();
    auto &commandBuffer = vkContext->CommandBuffer.GetCurCommandBuffer();
    auto indicesCount = mesh.GetIndicesCount();
    vkCmdDrawIndexed(commandBuffer, indicesCount, 1, 0, 0, 0);
}
