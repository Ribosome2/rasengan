#pragma once
#include "vector"
#include "VulkanCore/VulkanVertex.h"
#include "memory"
#include "Mesh.h"
#include "Material.h"

class MeshRenderer {
public:
    MeshRenderer();
    void Update();
    void Render();
public:
    Transform * transform;
    Mesh mesh;
    Material material;
};
