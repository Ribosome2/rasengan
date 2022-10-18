#include "GameObject.h"
#include "MeshRenderer.h"
#include "Time.h"

uint32_t  GameObject::s_autoInstanceId =0;
void GameObject::Update() {
  meshRenderer->Update();
  if(AutoRotate)
  {
      transform.eulerAngles.y+= Time::deltaTime*80;
  }
}

GameObject::GameObject() {
    s_autoInstanceId++;
    this->instanceId=s_autoInstanceId;
    this->transform.gameObject= this;
    std::cout<<"GameObject Created "<<std::endl;
}

GameObject::~GameObject() {
    std::cout<<"GameObject Destroy "<<std::endl;

}

template<typename componentClass>
inline std::shared_ptr<componentClass> GameObject::AddComponent() {
	auto  component =std::make_shared<componentClass>();
	return component;
}

