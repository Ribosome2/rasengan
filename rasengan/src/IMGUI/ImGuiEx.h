#pragma once

#include <string>
#include "imgui.h"
#include "glm/glm.hpp"
class ImGuiEx {
public:
	static bool SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format= "%.3f", ImGuiSliderFlags flags=0);
	static bool DrawVec3Control(const std::string& label, glm::vec3& values, float resetValue = 0.0f, float columnWidth = 100.0f);
	static void  DrawText(const std::string& label, const std::string &textContent,float columnWidth = 100.0f);
};
