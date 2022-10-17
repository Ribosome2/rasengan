#pragma once
#include "vector"
#include "VulkanCore/VulkanVertex.h"
#include "memory"
#include "Mesh.h"
#include "Material.h"
#include "EngineCore/Geometry/Quad.h"
#include "Component.h"
class MeshRenderer: public Component{
public:
    MeshRenderer();
    void Update();
    void Render();
public:
    std::shared_ptr<Mesh> mesh;

    //Returns the first instantiated Material assigned to the renderer.
    //Modifying material will change the material for this object only.
    std::shared_ptr<Material> material;
};
