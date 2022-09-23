#pragma once

#include "glm/glm.hpp"

class Transform {
public:
	void UpdateModelMatrix();
public:

	glm::vec3 position = {0, 0, 0};
	glm::vec3 eulerAngles{0, 0, 0};
	glm::vec3 scale{1.0, 1.0, 1.0};
	glm::mat4 localToWorldMatrix = glm::mat4(1.0f);
};
