#pragma once

#include <vector>
#include "memory"
#include "GameObject.h"

class Scene {
public:
    std::vector<std::shared_ptr<GameObject>> gameObjects;
};
