#include "GameObject.h"
#include "MeshRenderer.h"
void GameObject::Update() {
  meshRenderer->Update();
}

GameObject::GameObject() {
    std::cout<<"GameObject Created "<<std::endl;
}

GameObject::~GameObject() {
    std::cout<<"GameObject Destroy "<<std::endl;

}
