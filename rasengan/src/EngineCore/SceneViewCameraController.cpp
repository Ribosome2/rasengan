#include "SceneViewCameraController.h"
#include "Input/Input.h"
#include "Time.h"

SceneViewCameraController::SceneViewCameraController(Camera *target) : target(target) {

}

void SceneViewCameraController::Update() {
    float moveSpeed = 1.0f;
    float horizontalMove = 0;
    float verticalMove = 0;
    if (Input::GetKey(GLFW_KEY_A)) {
        horizontalMove = -1;
    }

    if (Input::GetKey(GLFW_KEY_D)) {
        horizontalMove = 1;
    }

    if (Input::GetKey(GLFW_KEY_W)) {
        verticalMove = 1;
    }

    if (Input::GetKey(GLFW_KEY_S)) {
        verticalMove = -1;
    }
    rotateByMouse();
    moveSpeed = moveSpeed * Time::deltaTime;
    target->eyePos -= target->cameraRight * horizontalMove * moveSpeed;
    target->eyePos += target->cameraFront * verticalMove * moveSpeed;
    // target->eyePos += glm::vec3(
    //         horizontalMove * moveSpeed,
    //         verticalMove * moveSpeed,
    //         0.0);
    panWithMouse();
}


void SceneViewCameraController::rotateByMouse() {
    if (Input::GetMouseButton(1)) {
        auto xOffset = Input::GetMouseDeltaPosition().x;
        auto yOffset = Input::GetMouseDeltaPosition().y;
        float sensitivity = 1.0f;
        xOffset *= sensitivity;
        yOffset *= sensitivity;
        target->yaw += xOffset;
        target->pitch += yOffset;

        if (target->pitch > 89.0f)
            target->pitch = 89.0f;
        if (target->pitch < -89.0f)
            target->pitch = -89.0f;

        target->UpdateFrontDirection();
    }


}

void SceneViewCameraController::panWithMouse() {
    if (Input::GetMouseButton(2)) {
        auto xOffset = Input::GetMouseDeltaPosition().x;
        auto yOffset = Input::GetMouseDeltaPosition().y;
        float sensitivity = 1.f * Time::deltaTime;
        xOffset *= sensitivity;
        yOffset *= sensitivity;
        auto cameraRight = glm::cross(target->cameraFront, target->cameraUp);
        target->eyePos -= cameraRight * xOffset;
        target->eyePos += target->cameraUp * yOffset;
    }
}
