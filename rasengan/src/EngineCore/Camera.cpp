#include "Camera.h"
#include "imgui.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/matrix_clip_space.hpp"

void Camera::OnGUI() {
    ImGui::SliderFloat3("eyePos ", (float *) &eyePos, -10, 10);
    static auto lookAtCenter = glm::vec3(0.0f, 0.0f, 0.0f);
    ImGui::SliderFloat3("lookAtCenter ", (float *) &lookAtCenter, -10, 10);
}

void Camera::UpdateDataMatrix() {

    worldToCameraMatrix = glm::lookAt(eyePos, eyePos - cameraFront, cameraUp);
    projectionMatrix = glm::perspective(glm::radians(45.0f), aspect, nearClipPlane, farClipPlane);
    projectionMatrix[1][1] *= -1;
}
