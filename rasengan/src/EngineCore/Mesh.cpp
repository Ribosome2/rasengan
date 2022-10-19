#include "Mesh.h"
#include "imgui.h"
#include "VulkanCore/VulkanContext.h"
#include "ModelLoader.h"

Mesh::Mesh() {

}

void Mesh::InitBuffer() {
    auto vertexBufferSize = sizeof(vertices[0]) * vertices.size();
    vertexBuffer = std::make_shared<VulkanVertexBuffer>(vertices.data(), static_cast<uint32_t>(vertexBufferSize));

    auto indexBufferSize = sizeof(indices[0]) * indices.size();
    indexBuffer = std::make_shared<VulkanIndexBuffer>(indices.data(), indices.size(),
                                                      static_cast<uint32_t>(indexBufferSize));
}

void EditVertexData(std::vector<VulkanVertex> &vertices) {
    for (int i = 0; i < vertices.size(); ++i) {
        auto &vertex = vertices[i];
        ImGui::PushID(i);
        ImGui::SliderFloat2(("Position "), (float *) &vertex.pos, -1.5, 2.5);
        ImGui::SliderFloat3("Color ", (float *) &vertex.color, -2, 2);
        ImGui::PopID();
    }
}

void Mesh::Update() {
//    EditVertexData(vertices);
//    vertexBuffer->UpdateMemory();
}

void Mesh::Bind() {
    auto vkContext = VulkanContext::Get();
    auto &commandBuffer = vkContext->CommandBuffer.GetCurCommandBuffer();
    VkBuffer vertexBuffers[] = {vertexBuffer->GetVulkanBuffer()};
    VkDeviceSize offsets[] = {0};
    VkBuffer indexBuffer = {this->indexBuffer->GetIndexBuffer()};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT16);
}

Mesh::Mesh(std::string path) {
    ModelLoader::LoadModel(path, this->vertices, this->indices);
    std::string base_filename = path.substr(path.find_last_of("/\\") + 1);
    this->name= base_filename;
    InitBuffer();
}
