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

    //Returns the first instantiated Material assigned to the renderer.
    //Modifying material will change the material for this object only.
    std::shared_ptr<Material> material;
};
