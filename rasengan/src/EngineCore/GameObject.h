#pragma once

#include <string>
#include "EngineCore/Transform.h"
#include "memory"
#include "Component.h"
class MeshRenderer;
class GameObject {
public:
    GameObject();
    GameObject(std::string _name);
    ~GameObject();
	void AddComponent(Component* pComponent);
    void Update();
public:
	Transform transform;
    std::shared_ptr<MeshRenderer>   meshRenderer= nullptr;
    uint32_t instanceId;
	std::string name;
    bool  AutoRotate = false;
private:
    static uint32_t s_autoInstanceId;
};
