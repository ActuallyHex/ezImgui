#pragma once
#include "imgui.h"
#include <map>
#include <imgui_internal.h>
#define IMGUI_DEFINE_MATH_OPERATORS

#define ToVec4(r, g, b, a) ImColor(r / 255.f, g / 255.f, b / 255.f, a)

using namespace ImGui;

namespace ezWidgets
{
    // [BUTTONS]
    bool ColoredButtonV1(const char* label, const ImVec2& size, ImU32 text_color, ImU32 bg_color_1, ImU32 bg_color_2);
    bool IconButton1(const char* label, ImVec2 size_arg, bool marker);
    // [CHECK BOXES]
    bool ToggleSwitch(const char* label, bool* v);
    bool CheckboxAnim1(const char* label, bool* v, const char* hint = "", ImFont* font = ImGui::GetDefaultFont());
    bool CheckboxAnim2(const char* label, bool* v);
    // [COMBO BOXES]
    bool selectableStyle1(const char* label, bool selected = false, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
    bool selectableStyle1(const char* label, bool* p_selected, ImGuiSelectableFlags flags = 0, const ImVec2& size = ImVec2(0, 0));
    bool beginComboStyle1(const char* label, const char* preview_value, ImGuiComboFlags flags);
    bool beginComboPopupStyle1(ImGuiID popup_id, const ImRect& bb, ImGuiComboFlags flags);
    bool comboBoxStyle1(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items);
    bool comboBoxHelper1(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items);
    // [SLIDERS]
    bool SliderStyle1(const char* label, float* v, float v_min, float v_max, const char* format, ImGuiSliderFlags flags);
    bool SliderStyle1(const char* label, int* v, int v_min, int v_max, const char* format, ImGuiSliderFlags flags);
}