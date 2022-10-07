#pragma once


#include "Camera.h"

class SceneViewCameraController {
public:
    SceneViewCameraController(Camera *target);

    void Update();
private:
    void rotateByMouse();
	void panWithMouse();
private:
    Camera * target;

};
