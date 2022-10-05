#pragma once


#include "Camera.h"

class SceneViewCameraController {
public:
    SceneViewCameraController(Camera *target);

    void Update();

private:
    Camera * target;

};
