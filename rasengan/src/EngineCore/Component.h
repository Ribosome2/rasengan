#pragma once
class GameObject;
class Transform;
class Component {
public:
	GameObject * gameObject= nullptr;
	Transform * transform= nullptr;
};
