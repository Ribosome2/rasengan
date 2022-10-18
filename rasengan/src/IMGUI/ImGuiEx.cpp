#include "ImGuiEx.h"

bool ImGuiEx::SliderFloat3(const char *label, float *v, float v_min, float v_max, const char *format,
						   ImGuiSliderFlags flags) {
	ImGui::Text(label);
	ImGui::SameLine();
	ImGui::PushID(label);
	ImGui::SliderFloat3("",v,v_min,v_max,format,flags);
	ImGui::PopID();
	return false;
}
