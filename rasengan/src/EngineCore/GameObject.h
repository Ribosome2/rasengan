#pragma once
#include "EngineCore/Transform.h"
#include "memory"
class MeshRenderer;
class GameObject {
public:
    GameObject();
    ~GameObject();
    void Update();
public:
	Transform transform;
    std::shared_ptr<MeshRenderer>   meshRenderer;
};
