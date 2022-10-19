#include "HierarchyWindow.h"
#include "imgui.h"
#include "ImGuiEx.h"
#include "Selection.h"

void HierarchyWindow::OnGUI(Scene *pScene) {
	assert(pScene != nullptr);
	ImGui::Begin("Hierarchy");
	auto &gameObjects = pScene->gameObjects;
	for (auto go: gameObjects) {

		ImGui::PushID(go->instanceId);
		ImGui::Button(go->name.c_str()) ;
		if (ImGui::IsItemClicked()) {
			Selection::activeGameObject = go.get();
		}
		ImGui::PopID();
	}
	ImGui::End();
}
