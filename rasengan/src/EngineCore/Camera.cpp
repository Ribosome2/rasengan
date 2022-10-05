#include "Camera.h"
#include "imgui.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

void Camera::OnGUI() {
    ImGui::SliderFloat3("eyePos ", (float *) &eyePos, -10, 10);
    static auto lookAtCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    ImGui::SliderFloat3("lookAtCenter ", (float *) &lookAtCenter, -10, 10);
    ImGui::SliderFloat("pitch ", (float *) &pitch, -89, 90);
    ImGui::SliderFloat("yaw ", (float *) &yaw, -89, 89);
}

void Camera::UpdateDataMatrix() {

    UpdateFrontDirection();
    worldToCameraMatrix = glm::lookAt(eyePos, eyePos + cameraFront, cameraUp);
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspect, nearClipPlane, farClipPlane);
    projectionMatrix[1][1] *= -1;
}

void Camera::UpdateFrontDirection() {
    glm::vec3 direction;
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);
}

Camera::Camera() {
    UpdateFrontDirection();
}
