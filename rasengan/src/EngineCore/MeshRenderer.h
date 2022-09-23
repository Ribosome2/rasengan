#pragma once
#include "vector"
#include "VulkanCore/VulkanVertex.h"
#include "VulkanCore/VulkanVertexBuffer.h"
#include "VulkanCore/VulkanIndexBuffer.h"
#include "memory"
class MeshRenderer {
public:
    MeshRenderer();
    void Update();
    void BindRenderData();
    void Render();
public:
    std::vector<VulkanVertex> vertices = {
            {{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
            {{0.5f,  -0.5f}, {0.0f, 1.0f, 0.0f}},
            {{0.5f,  0.5f},  {0.0f, 0.0f, 1.0f}},
            {{-0.5f, 0.5f},  {1.0f, 1.0f, 1.0f}}
    };
    std::vector<uint16_t> indices = {
            0, 1, 2, 2, 3, 0
    };
    std::shared_ptr<VulkanVertexBuffer> vertexBuffer;
    std::shared_ptr<VulkanIndexBuffer> indexBuffer;
};
