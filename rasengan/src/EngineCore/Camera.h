#pragma once
#include "glm/glm.hpp"

class Camera {
public:
	void OnGUI();
	void UpdateDataMatrix();
public:

	glm::vec3 eyePos = glm::vec3(0.0f, 0.0f, 2.0f);
	glm::vec3 lookAtCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	float nearClipPlane=0.1f;
	float farClipPlane=10.0f;
	float aspect=1;

//	Matrix that transforms from world to camera space.
//	This matrix is often referred to as "view matrix" in graphics literature.
	glm::mat4 worldToCameraMatrix;
	glm::mat4 projectionMatrix;
};
