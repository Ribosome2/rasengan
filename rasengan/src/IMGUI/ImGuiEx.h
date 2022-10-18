#pragma once
#include "imgui.h"

class ImGuiEx {
public:
	static bool SliderFloat3(const char* label, float v[3], float v_min, float v_max, const char* format= "%.3f", ImGuiSliderFlags flags=0);
};
