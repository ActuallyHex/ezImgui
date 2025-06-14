#pragma once
#include "imgui.h"
#include <map>
#include <imgui_internal.h>
#define IMGUI_DEFINE_MATH_OPERATORS

//extern ImFont* bold;
#define ToVec4(r, g, b, a) ImColor(r / 255.f, g / 255.f, b / 255.f, a)

using namespace ImGui;

namespace ezWidgets
{
    bool ToggleSwitch(const char* label, bool* v);
    bool CheckboxAnim(const char* label, bool* v, const char* hint = "", ImFont* font = ImGui::GetDefaultFont());
}
