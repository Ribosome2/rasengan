#pragma once
#include "vector"
#include "VulkanCore/VulkanVertex.h"
#include "memory"
#include "Mesh.h"

class MeshRenderer {
public:
    MeshRenderer();
    void Update();
    void Render();
public:
    Mesh mesh;
};
