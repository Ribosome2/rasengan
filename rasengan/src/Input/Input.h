#pragma once

#include "glm/glm.hpp"
#include "GLFW/glfw3.h"

class Input {
public:
    static void Init(GLFWwindow* window);
    static void Update();
    static void OnGUI();

    //button values are 0 for left button, 1 for right button, 2 for the middle button.
    static bool GetMouseButton(int button);

    //Returns true during the frame the user pressed the given mouse button.
    static bool GetMouseButtonDown(int button);

    // Returns true during the frame the user releases the given mouse button.
    static bool GetMouseButtonUp(int button);

    // Returns true while the user holds down the key identified by KeyCode
    // see all keycodes in glfw3.h
    static bool GetKey(int keyCode);

    // Returns true during the frame the user starts pressing down the key identified by KeyCode.
    static bool GetKeyDown(int keyCode);

    // Returns true during the frame the user releases the key identified by name.
    // You need to call this function from the Update function, since the state gets reset each frame. It will not return true until the user has pressed the key and released it again.
    static bool GetKeyUp(int keyCode);
private:
    static void cursorPositionCallback(GLFWwindow* window, double xPos, double yPos);
    static void scrollCallback(GLFWwindow* window, double xOffset, double yOffset);


public:
    static glm::vec3 mousePosition;
    static glm::vec2 mouseScrollDelta;
    static GLFWwindow *window;

private:
    static  glm::vec2 s_lastMousePos;
    static  glm::vec2 s_mouseDeltaPosition;
    static  float  s_lastMouseScroll;
    static  bool  s_isFirstPosCallCall;
    static int s_prevMouseButtonStates[3];
    static int s_mouseButtonStates[3];

    static int s_prevKeyButtonStates[];
    static int s_keyButtonStates[];

};
