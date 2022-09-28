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
    uint32_t instanceId;
private:
    static uint32_t s_autoInstanceId;
};
