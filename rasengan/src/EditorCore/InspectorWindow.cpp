#include "InspectorWindow.h"
#include "imgui.h"
#include "ImGuiEx.h"
#include "Selection.h"
#include "EngineCore/GameObject.h"
#include "imgui_internal.h"
#include "EngineCore/MeshRenderer.h"
#include "imgui_impl_vulkan.h"

template<typename T, typename UIFunction>
static void
DrawComponent(const std::string &name, GameObject *pGameObject, Component *component, UIFunction uiFunction) {
    const ImGuiTreeNodeFlags treeNodeFlags =
            ImGuiTreeNodeFlags_DefaultOpen | ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth |
            ImGuiTreeNodeFlags_AllowItemOverlap | ImGuiTreeNodeFlags_FramePadding;
//	if (entity.HasComponent<T>())
    {
        // NOTE(Peter):
        //	This fixes an issue where the first "+" button would display the "Remove" buttons for ALL components on an Entity.
        //	This is due to ImGui::TreeNodeEx only pushing the id for it's children if it's actually open
        ImGui::PushID((void *) typeid(T).hash_code());

        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2{4, 4});
        ImGui::Separator();
        bool open = ImGui::TreeNodeEx("##dummy_id", treeNodeFlags, name.c_str());
        ImGui::PopStyleVar();


        if (open) {
            uiFunction(component);
            ImGui::TreePop();
        }


        ImGui::PopID();
    }
}

void DrawImage(const std::string &label, ImTextureID user_texture_id,  ImVec2& size, float columnWidth=100) {
	if(size.x>columnWidth)
	{
		size.x=columnWidth;
	}
	if(size.y>columnWidth)
	{
		size.y=columnWidth;
	}

	ImGui::PushID(label.c_str());

	ImGui::Columns(2);
	ImGui::SetColumnWidth(0, columnWidth);
	ImGui::Text(label.c_str());
	ImGui::NextColumn();

	ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0, 0 });
	ImGui::SameLine();
	ImGui::Image(user_texture_id,size);
	ImGui::PopStyleVar();
	ImGui::PopID();
}


void DrawMaterial(std::shared_ptr<Material> material)
{
    if (material != nullptr) {
        if (ImGui::TreeNodeEx("Material")) {
            ImGuiEx::DrawText("Shader:", material->shader->name);
            auto & texture = material->mainTexture;
            if(texture!= nullptr)
            {
				if(texture->descriptorSet==VK_NULL_HANDLE)
				{
					texture->descriptorSet = ImGui_ImplVulkan_AddTexture(texture->textureSampler, texture->textureImageView, material->GetDescriptor().imageLayout);
				}
                 const auto textureID = texture->descriptorSet;
				ImGui::Text("mainTexture:");
				ImGui::SameLine();
				int gridSize =100;
				ImGui::Image((ImTextureID)textureID,ImVec2(gridSize, gridSize));
				ImGui::SameLine();
				if(ImGui::Button("Reload"))
				{
					VulkanTexture::ReloadTexture("Assets/Textures/texture.jpg",material->mainTexture);
				}
            }
            ImGui::TreePop();
        }
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
        if (ImGui::InputText("##name", buffer, 256)) {
            targetGo->name = std::string(buffer);
        }
        ImGui::PopItemWidth();

        // ID
        ImGui::SameLine();
        ImGui::TextDisabled("%u", targetGo->instanceId);


        //Transform
        auto &transformPt = targetGo->transform;
        DrawComponent<Transform>("Transform", targetGo, &transformPt, [](Component *component) {
            auto transformComp = (Transform *) component;
            ImGuiEx::DrawVec3Control("position:", transformComp->position);
            ImGuiEx::DrawVec3Control("eulerAngles:", transformComp->eulerAngles);
            ImGuiEx::DrawVec3Control("scale:", transformComp->scale);
        });

        DrawComponent<MeshRenderer>("MeshRenderer", targetGo, (Component *) targetGo->meshRenderer.get(),
                                    [](Component *component) {
                                        auto meshRenderer = (MeshRenderer *) component;
                                        ImGui::Text("mesh %s", meshRenderer->mesh->name.c_str());
                                        auto material = meshRenderer->material;
                                        DrawMaterial(material);
                                    });
    }
    ImGui::End();
}

