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
    moveSpeed = moveSpeed * Time::deltaTime;
    target->eyePos += glm::vec3(
            horizontalMove * moveSpeed,
            verticalMove * moveSpeed,
            0.0);
}
