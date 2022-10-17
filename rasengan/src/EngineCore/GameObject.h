#pragma once
#include "EngineCore/Transform.h"
#include "memory"
#include "Component.h"
class MeshRenderer;
class GameObject {
public:
    GameObject();
    ~GameObject();
	void AddComponent(Component* pComponent);
    void Update();
public:
	Transform transform;
    std::shared_ptr<MeshRenderer>   meshRenderer;
    uint32_t instanceId;
    bool  AutoRotate = false;
private:
    static uint32_t s_autoInstanceId;
};
