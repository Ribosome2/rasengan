#include "InspectorWindow.h"
#include "imgui.h"
#include "ImGuiEx.h"
#include "Selection.h"
#include "EngineCore/GameObject.h"
#include "imgui_internal.h"
#include "EngineCore/MeshRenderer.h"
template<typename T, typename UIFunction>
static void DrawComponent(const std::string& name, GameObject * pGameObject,  Component *  component,UIFunction uiFunction)
{
	const ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
//	if (entity.HasComponent<T>())
	{
		// NOTE(Peter):
		//	This fixes an issue where the first "+" button would display the "Remove" buttons for ALL components on an Entity.
		//	This is due to ImGui::TreeNodeEx only pushing the id for it's children if it's actually open
		ImGui::PushID((void*)typeid(T).hash_code());
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{ 4, 4 });
		ImGui::Separator();
		bool open = ImGui::TreeNodeEx("##dummy_id", treeNodeFlags, name.c_str());
		ImGui::PopStyleVar();


		if (open)
		{
			uiFunction(component);
			ImGui::TreePop();
		}


		ImGui::PopID();
	}
}
void InspectorWindow::OnInspectorGUI() {
	ImGui::Begin("Inspector");
	auto targetGo = Selection::activeGameObject;
	if (targetGo != nullptr) {

		//name
		ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
		char buffer[256];
		memset(buffer, 0, 256);
		memcpy(buffer, targetGo->name.c_str(), targetGo->name.length());
		ImGui::PushItemWidth(contentRegionAvailable.x * 0.5f);
		if (ImGui::InputText("##name", buffer, 256))
		{
			targetGo->name = std::string(buffer);
		}
		ImGui::PopItemWidth();

		// ID
		ImGui::SameLine();
		ImGui::TextDisabled("%u", targetGo->instanceId);


		//Transform
		auto &transformPt = targetGo->transform;
		DrawComponent<Transform>("Transform", targetGo,&transformPt, [](Component* component)
		{
			auto transformComp =(Transform*)component;
			ImGuiEx::DrawVec3Control("position:", transformComp->position);
			ImGuiEx::DrawVec3Control("eulerAngles:", transformComp->eulerAngles);
			ImGuiEx::DrawVec3Control("scale:", transformComp->scale);
		});

		DrawComponent<MeshRenderer>("MeshRenderer", targetGo,(Component*)targetGo->meshRenderer.get(), [](Component* component)
		{
			auto meshRenderer =(MeshRenderer*)component;
            ImGui::Text("mesh: %s",meshRenderer->mesh->name.c_str());
            if(meshRenderer->material!= nullptr){
                ImGui::Text("Material: %s ",meshRenderer->material->name.c_str());
            }
		});
	}
	ImGui::End();
}

