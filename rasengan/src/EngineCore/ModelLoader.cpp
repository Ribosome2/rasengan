#include "ModelLoader.h"
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>
#include <unordered_map>
#include "VulkanCore/VulkanVertex.h"
void ModelLoader::LoadModel(std::string path,std::vector<VulkanVertex> & vertices,
                            std::vector<uint16_t>&  indices) {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn, err;
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, path.c_str())) {
        throw std::runtime_error(warn + err);
    }
    std::unordered_map<VulkanVertex, uint32_t> uniqueVertices{};
    for (const auto& shape : shapes) {
        for (const auto& index : shape.mesh.indices) {
            VulkanVertex vertex{};

            vertex.pos = {
                    attrib.vertices[3 * index.vertex_index + 0],
                    attrib.vertices[3 * index.vertex_index + 1],
                    attrib.vertices[3 * index.vertex_index + 2]
            };

            // The OBJ format assumes a coordinate system where a vertical coordinate of 0 means the bottom of the image,
            // however we've uploaded our image into Vulkan in a top to bottom orientation where 0 means the top of the image.
            // Solve this by flipping the vertical component of the texture coordinates:
            vertex.texCoord = {
                    attrib.texcoords[2 * index.texcoord_index + 0],
                    1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            vertex.color = {1.0f, 1.0f, 1.0f};
            if(uniqueVertices.count(vertex)==0)
            {
                uniqueVertices[vertex]=static_cast<uint32_t>(vertices.size());
                vertices.push_back(vertex);
            }
            indices.push_back(uniqueVertices[vertex]);
        }
    }
}
