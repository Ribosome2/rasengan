#include <iostream>
#include "Input.h"
#include "imgui.h"

#define NONE_PRESS_STATE -1
glm::vec2 Input::s_lastMousePos;
glm::vec2 Input::s_mouseDeltaPosition;
static glm::vec2 mouseScrollDelta;
float  Input::s_lastMouseScroll;
bool  Input::s_isFirstPosCallCall = false;
int  Input::s_prevMouseButtonStates[3];
int  Input::s_mouseButtonStates[3];

int  Input::s_prevKeyButtonStates[GLFW_KEY_LAST];
int  Input::s_keyButtonStates[GLFW_KEY_LAST];
GLFWwindow *Input::window;

bool Input::GetMouseButton(int button) {
    return glfwGetMouseButton(window, button) == GLFW_PRESS;
}

bool Input::GetMouseButtonDown(int button) {
    return s_mouseButtonStates[button] == GLFW_PRESS;
}

bool Input::GetMouseButtonUp(int button) {
    return s_mouseButtonStates[button] == GLFW_RELEASE;
}

bool Input::GetKey(int keyCode) {
    return (glfwGetKey(window, keyCode) == GLFW_PRESS);
}

bool Input::GetKeyDown(int keyCode) {
    return s_keyButtonStates[keyCode] == GLFW_PRESS;
}


bool Input::GetKeyUp(int keyCode) {
    return s_keyButtonStates[keyCode] == GLFW_RELEASE;
}

void Input::Init(GLFWwindow *window) {
    Input::window = window;
    glfwSetCursorPosCallback(window, Input::cursorPositionCallback);
    glfwSetScrollCallback(window, Input::scrollCallback);
}

void Input::cursorPositionCallback(GLFWwindow *window, double xPos, double yPos) {
    if (s_isFirstPosCallCall) {
        s_isFirstPosCallCall = false;
        s_mouseDeltaPosition.x = 0;
        s_mouseDeltaPosition.y = 0;
    } else {
        s_mouseDeltaPosition.x = (float) xPos - s_lastMousePos.x;
        s_mouseDeltaPosition.y = (float) yPos - s_lastMousePos.y;
    }
    s_lastMousePos.x = (float) xPos;
    s_lastMousePos.y = (float) yPos;
}

void Input::scrollCallback(GLFWwindow *window, double xOffset, double yOffset) {
    s_lastMouseScroll = (float) yOffset;
}

void Input::Update() {

    //update mouse state
    for (int button = 0; button < 3; ++button) {
        auto curMouseState = glfwGetMouseButton(window, button);
        if (curMouseState == GLFW_PRESS && s_prevMouseButtonStates[button] == GLFW_RELEASE) {
            s_mouseButtonStates[button] = GLFW_PRESS;
        } else if (curMouseState == GLFW_RELEASE && s_prevMouseButtonStates[button] == GLFW_PRESS) {
            s_mouseButtonStates[button] = GLFW_RELEASE;
        } else {
            s_mouseButtonStates[button] = NONE_PRESS_STATE;
        }
        s_prevMouseButtonStates[button] = curMouseState;
    }

    for (int button = 0; button < GLFW_KEY_LAST; ++button) {
        auto keyButtonState = glfwGetKey(window, button);
        if (keyButtonState == GLFW_PRESS && s_prevKeyButtonStates[button] == GLFW_RELEASE) {
            s_keyButtonStates[button] = GLFW_PRESS;
        } else if (keyButtonState == GLFW_RELEASE && s_prevKeyButtonStates[button] == GLFW_PRESS) {
            s_keyButtonStates[button] = GLFW_RELEASE;
        } else {
            s_keyButtonStates[button] = NONE_PRESS_STATE;
        }
        s_prevKeyButtonStates[button] = keyButtonState;
    }
}

void Input::OnGUI() {
//    if (Input::GetMouseButton(0)) {
//        ImGui::Text("Pressing Mouse left ");
//    }
//
//    if (Input::GetMouseButton(1)) {
//        ImGui::Text("Pressing Mouse Right ");
//    }
//
//    if (Input::GetMouseButton(2)) {
//        ImGui::Text("Pressing Mouse Middle ");
//    }
//
//    for (int i = 0; i < 3; ++i) {
//        if (Input::GetMouseButtonUp(i)) {
//            std::cout << " mouse " << i << " up " << std::endl;
//        }
//
//        if (Input::GetMouseButtonDown(i)) {
//            std::cout << " mouse " << i << " down " << std::endl;
//        }
//    }
//
//    if (Input::GetKey(GLFW_KEY_W)) {
//        ImGui::Text("Pressing W  ");
//    }
//    if (Input::GetKeyDown(GLFW_KEY_W)) {
//        std::cout<<"Pressing W  down  " <<std::endl;
//    }
//
//    if (Input::GetKeyUp(GLFW_KEY_W)) {
//        std::cout<<"Pressing W  up  " <<std::endl;
//    }

}
