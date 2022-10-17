#pragma once


#include <vector>
#include <memory>
#include <string>
#include "VulkanCore/VulkanVertex.h"
#include "VulkanCore/VulkanVertexBuffer.h"
#include "VulkanCore/VulkanIndexBuffer.h"

class Mesh {
public:
    Mesh();
    Mesh(std::string path );
    void Update();
    void Bind();
    uint32_t GetIndicesCount() const{
        return indexBuffer->GetCount();
    }

protected:
	void InitBuffer();

public:
    std::vector<VulkanVertex> vertices;

    std::vector<uint16_t> indices ;
    std::shared_ptr<VulkanVertexBuffer> vertexBuffer;
    std::shared_ptr<VulkanIndexBuffer> indexBuffer;

};
