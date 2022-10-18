#include "HierarchyWindow.h"
#include "imgui.h"
#include "ImGuiEx.h"

void HierarchyWindow::OnGUI(Scene *pScene) {
	assert(pScene!= nullptr);
	ImGui::Begin("Hierarchy");
	auto & gameObjects =pScene->gameObjects ;
	for (auto go: gameObjects) {
		auto & transformPt =go->transform;
		ImGui::PushID(go->instanceId);
		if(ImGui::TreeNode(go->name.c_str())){
			ImGui::Separator();
			ImGui::Text("Transform:");
			ImGuiEx::SliderFloat3("position:", (float *) &transformPt.position, -10, 10);
			ImGuiEx::SliderFloat3("eulerAngles:", (float *) &transformPt.eulerAngles, -360, 360);
			ImGuiEx::SliderFloat3("scale:", (float *) &transformPt.scale, -5, 5);

			if(go->meshRenderer!= nullptr)
			{
				ImGui::Separator();
				ImGui::Text("MeshRenderer:");
			}
			ImGui::TreePop();
		}

		ImGui::PopID();
	}
	ImGui::End();
}
