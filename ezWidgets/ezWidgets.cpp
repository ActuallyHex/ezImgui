#include "ezWidgets.h"
#include <unordered_map>

struct checkbox_animation
{
    float animation;
};

bool ezWidgets::ToggleSwitch(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float w = GetWindowWidth() - 30;
    const float square_sz = 17;
    const ImVec2 pos = window->DC.CursorPos;

    ImVec2 frame_min = ImVec2(pos.x + w - square_sz - 13.0f, pos.y);
    ImVec2 frame_max = ImVec2(window->DC.CursorPos.x + w, window->DC.CursorPos.y + square_sz - 1.0f);

    const ImRect frame_bb(frame_min, frame_max);
    float width = square_sz;
    if (label_size.x > 0.0f)
        width += style.ItemInnerSpacing.x + label_size.x;

    float height = label_size.y + style.FramePadding.y * 2.0f;

    ImVec2 total_max = ImVec2(pos.x + width, pos.y + height);
    const ImRect total_bb(pos, total_max);

    //const ImRect frame_bb(pos + ImVec2(w - square_sz - 13, 0), window->DC.CursorPos + ImVec2(w, square_sz - 1));
    //const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(frame_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    static std::map<ImGuiID, checkbox_animation> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end())
    {
        anim.insert({ id, { 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.animation = ImLerp(it_anim->second.animation, *v ? 1.0f : 0.0f, 0.12f * (1.0f - ImGui::GetIO().DeltaTime));

    RenderNavHighlight(total_bb, id);

    //RenderFrame(frame_bb.Min, frame_bb.Max, ImColor(M::menu.frameBackground), false, 9.0f);
    //RenderFrame(frame_bb.Min, frame_bb.Max, ImColor(M::menu.frameBackground), false, 9.0f);

    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, 9.0f);
    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBgActive, it_anim->second.animation), true, 9.0f);

    // D::AddDrawListShadowRect(ImGui::GetBackgroundDrawList(), pos, pos + size, (Color_t)M::menu.menuAccent, 64.f * flDpiScale, style.WindowRounding, ImDrawFlags_ShadowCutOutShapeBackground);

    if (*v)
    {
        //std::cout << "True" << std::endl;
        window->DrawList->AddCircleFilled(ImVec2(frame_bb.Min.x + 8 + 14 * it_anim->second.animation, frame_bb.Min.y + 8), 5.0f, GetColorU32(ImGuiCol_CheckMark), 30);
    }
    else
    {
        //std::cout << "False" << std::endl;
        window->DrawList->AddCircleFilled(ImVec2(frame_bb.Min.x + 8 + 14 * it_anim->second.animation, frame_bb.Min.y + 8), 5.0f, GetColorU32(ImGuiCol_FrameBgActive, 0.5f), 30);
    }

    //window->DrawList->AddCircleFilled(ImVec2(frame_bb.Min.x + 8 + 14 * it_anim->second.animation, frame_bb.Min.y + 8), 5.0f, M::menu.frameBackground, 30);

    if (label_size.x > 0.0f)
        RenderText(total_bb.Min, label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}


bool ezWidgets::CheckboxAnim(const char* label, bool* v, const char* hint, ImFont* font)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);
    const ImVec2 hint_size = CalcTextSize(hint, NULL, true);

    const float square_sz = 16.f;
    const float ROUNDING = 3.5f;
    const ImVec2 pos = window->DC.CursorPos;

    ImVec2 frame_max = ImVec2(pos.x + square_sz, pos.y + square_sz);
    const ImRect frame_bb(pos, frame_max);

    // Compute width based on hint vs label size
    float total_width = 0.0f;
    if (hint_size.x > label_size.x)
        total_width = square_sz + hint_size.x + style.ItemInnerSpacing.x + 2.0f;
    else
        total_width = square_sz + label_size.x + style.ItemInnerSpacing.x + 2.0f;

    // Compute height based on hint presence
    float total_height = (hint_size.x > 0) ? (square_sz + 1.0f + hint_size.y) : square_sz;

    ImVec2 total_max = ImVec2(pos.x + total_width, pos.y + total_height);
    const ImRect total_bb(pos, total_max);

    //const ImRect frame_bb(pos, pos + ImVec2(square_sz, square_sz));
    //const ImRect total_bb(pos, pos + ImVec2(hint_size.x > label_size.x ? square_sz + hint_size.x + style.ItemInnerSpacing.x + 2 : square_sz + label_size.x + style.ItemInnerSpacing.x + 2, hint_size.x > 0 ? square_sz + 1 + hint_size.y : square_sz));

    ItemAdd(total_bb, id, &frame_bb);
    ItemSize(total_bb, style.FramePadding.y);

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    static std::unordered_map<ImGuiID, float> values;
    auto value = values.find(id);

    if (value == values.end())
    {
        values.insert({ id, 0.f });
        value = values.find(id);
    }
    //ImColor bruh = GetColorU32(ImVec4(38.f / 255.f, 38.f / 255.f, 44.f / 255.f, style.Alpha));
    value->second = ImLerp(value->second, (*v ? 1.f : 0.f), 0.09f);
    //38,38,44 to_vec4(38, 38, 44, style.Alpha)
    RenderFrame(frame_bb.Min, frame_bb.Max, ImColor(38, 38, 44), 0, ROUNDING); // (255 * style.Alpha)
    window->DrawList->AddRect(frame_bb.Min, frame_bb.Max, ImColor(38, 38, 44), ROUNDING);

    RenderFrame(frame_bb.Min, frame_bb.Max, GetColorU32(ImGuiCol_FrameBg), 0, ROUNDING); // custom.get_accent_color(value->second * style.Alpha)

    ImColor bruh = GetColorU32(ImGuiCol_CheckMark);

    //window->DrawList->PushClipRect(
    //    frame_bb.GetCenter() - ImVec2(8.0f / 2.0f, 8.0f / 2.0f),
    //    frame_bb.GetCenter() + ImVec2(6.0f * value->second, 8.0f)
    //);

    //RenderCheckMark(
    //    window->DrawList,
    //    frame_bb.GetCenter() - ImVec2(8.0f / 2.0f, 8.0f / 2.0f),
    //    ImColor(ImVec4(bruh.Value.x, bruh.Value.y, bruh.Value.z, value->second * style.Alpha)),
    //    8.0f
    //);

    //window->DrawList->PopClipRect();
    ImVec2 center = frame_bb.GetCenter();
    ImVec2 half_check_size(4.0f, 4.0f);  // 8.0f / 2.0f

    ImVec2 clip_min = ImVec2(center.x - half_check_size.x, center.y - half_check_size.y);
    ImVec2 clip_max = ImVec2(center.x + 6.0f * static_cast<float>(value->second), center.y + 8.0f);

    window->DrawList->PushClipRect(clip_min, clip_max);

    ImColor check_color = ImColor(ImVec4(
        bruh.Value.x,
        bruh.Value.y,
        bruh.Value.z,
        static_cast<float>(value->second) * style.Alpha
    ));

    window->DrawList->PopClipRect();

    ImVec2 checkMarkPos = ImVec2(center.x - half_check_size.x, center.y - half_check_size.y);
    RenderCheckMark(window->DrawList, checkMarkPos, check_color, 8.0f);

    ImVec2 label_pos = ImVec2(frame_bb.Max.x + style.ItemInnerSpacing.x + 2, frame_bb.GetCenter().y - label_size.y / 2);
    if (label_size.x > 0.f)
        window->DrawList->AddText(font, font->FontSize, label_pos, ToVec4(255.f, 255.f, 255.f, 255.f), label, FindRenderedTextEnd(label));

    if (hint_size.x > 0.f)
        window->DrawList->AddText(GetIO().Fonts->Fonts[0], GetIO().Fonts->Fonts[0]->FontSize - 1.f, ImVec2(label_pos.x, label_pos.y + label_size.y + 1), GetColorU32(ImGuiCol_Text, 0.7f), hint, FindRenderedTextEnd(hint));

    return pressed;
}
