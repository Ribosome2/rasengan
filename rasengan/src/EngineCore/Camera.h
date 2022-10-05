#pragma once
#include "glm/glm.hpp"
//ref: https://learnopengl.com/Getting-started/Camera
class Camera {
public:
    Camera();
	void OnGUI();
    void UpdateFrontDirection();
	void UpdateDataMatrix();
public:
    glm::vec3 eyePos = glm::vec3(0.0f, 0.0f, 2.0f);

    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraDirection = glm::normalize(eyePos - cameraTarget);
    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

    float pitch=0;
    float yaw=-90;


	glm::vec3 lookAtCenter = glm::vec3(0.0f, 0.0f, 0.0f);
	float nearClipPlane=0.1f;
	float farClipPlane=10.0f;
	float aspect=1;

//	Matrix that transforms from world to camera space.
//	This matrix is often referred to as "view matrix" in graphics literature.
	glm::mat4 worldToCameraMatrix;
	glm::mat4 projectionMatrix;
};
