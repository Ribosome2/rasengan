#pragma once
#include "vector"
#include "VulkanCore/VulkanVertex.h"
#include "memory"
#include "Mesh.h"
#include "Material.h"
#include "EngineCore/Geometry/Quad.h"

class MeshRenderer {
public:
    MeshRenderer();
    void Update();
    void Render();
public:
    Transform * transform;
    std::shared_ptr<Mesh> mesh;

    //Returns the first instantiated Material assigned to the renderer.
    //Modifying material will change the material for this object only.
    std::shared_ptr<Material> material;
};
