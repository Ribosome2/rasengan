#include "GameObject.h"
#include "MeshRenderer.h"
#include "Time.h"

uint32_t  GameObject::s_autoInstanceId = 0;

void GameObject::Update() {
	meshRenderer->Update();
	if (AutoRotate) {
		transform.eulerAngles.y += Time::deltaTime * 80;
	}
}

GameObject::GameObject() {
	s_autoInstanceId++;
	this->instanceId = s_autoInstanceId;
	this->transform.gameObject = this;
	this->name="Undefined";
	std::cout << "GameObject Created " << std::endl;
}

GameObject::~GameObject() {
	std::cout << "GameObject Destroy " << std::endl;

}

void GameObject::AddComponent(Component *pComponent) {
	assert(pComponent != nullptr);
	pComponent->gameObject = this;
	pComponent->transform = &transform;
}

GameObject::GameObject(std::string _name):GameObject() {
	this->name = _name;
}
