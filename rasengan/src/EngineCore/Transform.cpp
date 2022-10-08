#include "Transform.h"
#include "imgui.h"
#include <glm/gtc/matrix_transform.hpp>
#include "GameObject.h"
void Transform::UpdateModelMatrix() {
    ImGui::PushID(this->gameObject->instanceId);
	ImGui::SliderFloat3("ModelPosition", (float *) &position, -10, 10);
	ImGui::SliderFloat3("eulerAngles", (float *) &eulerAngles, -360, 360);
	ImGui::SliderFloat3("scale", (float *) &scale, -5, 5);
    ImGui::PopID();

	localToWorldMatrix = glm::mat4{1.0};


	localToWorldMatrix = glm::translate(localToWorldMatrix, position);

    //this rotation is too naive and incorrect 
	localToWorldMatrix = glm::rotate(localToWorldMatrix, eulerAngles.x, glm::vec3{1.0, 0.0, 0.0});
	localToWorldMatrix = glm::rotate(localToWorldMatrix, eulerAngles.y, glm::vec3{0.0, 1.0, 0.0});
	localToWorldMatrix = glm::rotate(localToWorldMatrix, eulerAngles.z, glm::vec3{0.0, 0.0, 1.0});

	localToWorldMatrix = glm::scale(localToWorldMatrix, scale);


}
