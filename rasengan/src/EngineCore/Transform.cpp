#include "Transform.h"
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"
void Transform::UpdateModelMatrix() {
	assert(this->gameObject!= nullptr);

	localToWorldMatrix = glm::mat4{1.0};


	localToWorldMatrix = glm::translate(localToWorldMatrix, position);

    //this rotation is too naive and incorrect 
	localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(eulerAngles.x), glm::vec3{1.0, 0.0, 0.0});
	localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(eulerAngles.y), glm::vec3{0.0, 1.0, 0.0});
	localToWorldMatrix = glm::rotate(localToWorldMatrix, glm::radians(eulerAngles.z), glm::vec3{0.0, 0.0, 1.0});

	localToWorldMatrix = glm::scale(localToWorldMatrix, scale);


}
