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
        ImGuiTreeNodeFlags flag = ImGuiTreeNodeFlags_Leaf;
        if(go.get()==Selection::activeGameObject)
        {
            flag |= ImGuiTreeNodeFlags_Selected;
        }
		if(ImGui::TreeNodeEx(go->name.c_str(),flag)){
            ImGui::TreePop();
        }
        if (ImGui::IsItemClicked()) {
            Selection::activeGameObject = go.get();
        }

		ImGui::PopID();
	}
	ImGui::End();
}
