#include "GameObject.h"
#include "MeshRenderer.h"
uint32_t  GameObject::s_autoInstanceId =0;
void GameObject::Update() {
  meshRenderer->Update();
}

GameObject::GameObject() {
    s_autoInstanceId++;
    this->instanceId=s_autoInstanceId;
    std::cout<<"GameObject Created "<<std::endl;
}

GameObject::~GameObject() {
    std::cout<<"GameObject Destroy "<<std::endl;

}
