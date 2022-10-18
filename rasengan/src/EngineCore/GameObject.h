#pragma once
#include "EngineCore/Transform.h"
#include "memory"
#include "Component.h"
class MeshRenderer;
class GameObject {
public:
    GameObject();
    ~GameObject();

	template<typename componentClass>
	inline std::shared_ptr<componentClass> AddComponent();
    void Update();
public:
	Transform transform;
    std::shared_ptr<MeshRenderer>   meshRenderer;
    uint32_t instanceId;
    bool  AutoRotate = false;
private:
    static uint32_t s_autoInstanceId;
};
