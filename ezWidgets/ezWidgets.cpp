#include "ezWidgets.h"
#include <unordered_map>

static float CalcMaxPopupHeightFromItemCount(int items_count)
{
    ImGuiContext& g = *GImGui;
    if (items_count <= 0)
        return FLT_MAX;
    return (g.FontSize + g.Style.ItemSpacing.y) * items_count - g.Style.ItemSpacing.y + (g.Style.WindowPadding.y * 2);
}

static const char* Items_ArrayGetter(void* data, int idx)
{
    const char* const* items = (const char* const*)data;
    return items[idx];
}

// [CHECKBOXES]

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

bool ezWidgets::CheckboxAnim1(const char* label, bool* v, const char* hint, ImFont* font)
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

struct checkbox_state
{
    ImVec4 gradient_color0, gradient_color1;

    ImVec4 text_color, checkmark_color, background_color;

    float rg;
};

bool ezWidgets::CheckboxAnim2(const char* label, bool* v)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    const ImVec2 label_size = CalcTextSize(label, NULL, true);

    const float square_sz = GetFrameHeight() + 1;
    const ImVec2 pos = window->DC.CursorPos;

    ImVec2 total_size = ImVec2(
        square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f),
        ImMax(square_sz, label_size.y + style.FramePadding.y * 2.0f)
    );
    ImRect total_bb(pos, ImVec2(pos.x + total_size.x, pos.y + total_size.y));

    ItemSize(total_bb, style.FramePadding.y);
    if (!ItemAdd(total_bb, id))
    {
        IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
        return false;
    }

    bool hovered, held;
    bool pressed = ButtonBehavior(total_bb, id, &hovered, &held);
    if (pressed)
    {
        *v = !(*v);
        MarkItemEdited(id);
    }

    static std::map<ImGuiID, checkbox_state> anim;
    auto& a = anim[id];

    // Color interpolation setup
    ImVec4 on_bg = ImGui::GetStyleColorVec4(ImGuiCol_FrameBgActive);
    ImVec4 off_bg = ImVec4(0, 0, 0, 0);
    ImVec4 checkmark_color = ImGui::GetStyleColorVec4(ImGuiCol_CheckMark);
    ImVec4 text_on = ImVec4(1, 1, 1, 1);
    ImVec4 text_off_hover = ImVec4(76 / 255.0f, 75 / 255.0f, 80 / 255.0f, 1);
    ImVec4 text_off = ImVec4(66 / 255.0f, 65 / 255.0f, 70 / 255.0f, 1);

    ImVec4 target_color = *v ? on_bg : (hovered ? off_bg : off_bg);
    a.gradient_color0 = ImLerp(a.gradient_color0, target_color, g.IO.DeltaTime * 6.f);
    a.gradient_color1 = ImLerp(a.gradient_color1, target_color, g.IO.DeltaTime * 6.f);
    a.checkmark_color = ImLerp(a.checkmark_color, *v ? checkmark_color : off_bg, g.IO.DeltaTime * 9.f);
    a.text_color = ImLerp(a.text_color, *v ? text_on : (hovered ? text_off_hover : text_off), g.IO.DeltaTime * 6.f);
    a.rg = ImLerp(a.rg, *v ? 0.f : 10.f, g.IO.DeltaTime * 5.f);

    ImRect check_bb(pos, ImVec2(pos.x + square_sz, pos.y + square_sz));
    RenderNavHighlight(total_bb, id);
    RenderFrame(check_bb.Min, check_bb.Max, GetColorU32(ImGuiCol_FrameBg), true, style.FrameRounding);

    const int vtx_start = window->DrawList->VtxBuffer.Size;

    ImVec2 min_rect = ImVec2(check_bb.Min.x + a.rg, check_bb.Min.y + a.rg);
    ImVec2 max_rect = ImVec2(check_bb.Max.x - a.rg, check_bb.Max.y - a.rg);

    window->DrawList->AddRectFilled(min_rect, max_rect, GetColorU32(a.gradient_color0), style.FrameRounding);
    ImGui::ShadeVertsLinearColorGradientKeepAlpha(window->DrawList, vtx_start, window->DrawList->VtxBuffer.Size,
        check_bb.Min, check_bb.Max, GetColorU32(a.gradient_color1), GetColorU32(a.gradient_color0));

    const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
    if (*v)
    {
        ImVec2 check_pos = ImVec2(check_bb.Min.x + pad + 3.0f, check_bb.Min.y + pad + 3.0f + a.rg);
        RenderCheckMark(window->DrawList, check_pos, GetColorU32(a.checkmark_color), square_sz - pad * 4.0f);
    }

    ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
    window->DrawList->AddText(label_pos, GetColorU32(a.text_color), label);

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags | ImGuiItemStatusFlags_Checkable | (*v ? ImGuiItemStatusFlags_Checked : 0));
    return pressed;
}

// [COMBO BOXES]

struct selectable_element
{
    float opacity;
};

bool ezWidgets::selectableStyle1(const char* label, bool selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    ImGuiWindow* window = GetCurrentWindow();
    if (window->SkipItems)
        return false;

    ImGuiContext& g = *GImGui;
    const ImGuiStyle& style = g.Style;

    // Submit label or explicit size to ItemSize(), whereas ItemAdd() will submit a larger/spanning rectangle.
    ImGuiID id = window->GetID(label);
    ImVec2 label_size = CalcTextSize(label, NULL, true);
    ImVec2 size(size_arg.x != 0.0f ? size_arg.x : label_size.x, size_arg.y != 0.0f ? size_arg.y : label_size.y);
    ImVec2 pos = window->DC.CursorPos;
    pos.y += window->DC.CurrLineTextBaseOffset;
    ItemSize(size, 0.0f);

    // Fill horizontal space
    // We don't support (size < 0.0f) in Selectable() because the ItemSpacing extension would make explicitly right-aligned sizes not visibly match other widgets.
    const bool span_all_columns = (flags & ImGuiSelectableFlags_SpanAllColumns) != 0;
    const float min_x = span_all_columns ? window->ParentWorkRect.Min.x : pos.x;
    const float max_x = span_all_columns ? window->ParentWorkRect.Max.x : window->WorkRect.Max.x;
    if (size_arg.x == 0.0f || (flags & ImGuiSelectableFlags_SpanAvailWidth))
        size.x = ImMax(label_size.x, max_x - min_x);

    // Text stays at the submission position, but bounding box may be extended on both sides
    const ImVec2 text_min = pos;
    const ImVec2 text_max(min_x + size.x, pos.y + size.y);

    // Selectables are meant to be tightly packed together with no click-gap, so we extend their box to cover spacing between selectable.
    ImRect bb(min_x, pos.y, text_max.x, text_max.y);
    if ((flags & ImGuiSelectableFlags_NoPadWithHalfSpacing) == 0)
    {
        const float spacing_x = span_all_columns ? 0.0f : style.ItemSpacing.x;
        const float spacing_y = style.ItemSpacing.y;
        const float spacing_L = IM_FLOOR(spacing_x * 0.50f);
        const float spacing_U = IM_FLOOR(spacing_y * 0.50f);
        bb.Min.x -= spacing_L;
        bb.Min.y -= spacing_U;
        bb.Max.x += (spacing_x - spacing_L);
        bb.Max.y += (spacing_y - spacing_U);
    }
    //if (g.IO.KeyCtrl) { GetForegroundDrawList()->AddRect(bb.Min, bb.Max, IM_COL32(0, 255, 0, 255)); }

    // Modify ClipRect for the ItemAdd(), faster than doing a PushColumnsBackground/PushTableBackground for every Selectable..
    const float backup_clip_rect_min_x = window->ClipRect.Min.x;
    const float backup_clip_rect_max_x = window->ClipRect.Max.x;
    if (span_all_columns)
    {
        window->ClipRect.Min.x = window->ParentWorkRect.Min.x;
        window->ClipRect.Max.x = window->ParentWorkRect.Max.x;
    }

    const bool disabled_item = (flags & ImGuiSelectableFlags_Disabled) != 0;
    const bool item_add = ItemAdd(bb, id, NULL, disabled_item ? ImGuiItemFlags_Disabled : ImGuiItemFlags_None);
    if (span_all_columns)
    {
        window->ClipRect.Min.x = backup_clip_rect_min_x;
        window->ClipRect.Max.x = backup_clip_rect_max_x;
    }

    if (!item_add)
        return false;

    const bool disabled_global = (g.CurrentItemFlags & ImGuiItemFlags_Disabled) != 0;
    if (disabled_item && !disabled_global) // Only testing this as an optimization
        BeginDisabled();

    // FIXME: We can standardize the behavior of those two, we could also keep the fast path of override ClipRect + full push on render only,
    // which would be advantageous since most selectable are not selected.
    if (span_all_columns && window->DC.CurrentColumns)
        PushColumnsBackground();
    else if (span_all_columns && g.CurrentTable)
        TablePushBackgroundChannel();

    // We use NoHoldingActiveID on menus so user can click and _hold_ on a menu then drag to browse child entries
    ImGuiButtonFlags button_flags = 0;
    if (flags & ImGuiSelectableFlags_NoHoldingActiveID)
    {
        button_flags |= ImGuiButtonFlags_NoHoldingActiveId;
    }
    if (flags & ImGuiSelectableFlags_SelectOnClick)
    {
        button_flags |= ImGuiButtonFlags_PressedOnClick;
    }
    if (flags & ImGuiSelectableFlags_SelectOnRelease)
    {
        button_flags |= ImGuiButtonFlags_PressedOnRelease;
    }
    if (flags & ImGuiSelectableFlags_AllowDoubleClick)
    {
        button_flags |= ImGuiButtonFlags_PressedOnClickRelease | ImGuiButtonFlags_PressedOnDoubleClick;
    }
    if (flags & ImGuiSelectableFlags_AllowOverlap)
    {
        button_flags |= ImGuiButtonFlags_AllowOverlap;
    }

    const bool was_selected = selected;
    bool hovered, held;
    bool pressed = ButtonBehavior(bb, id, &hovered, &held, button_flags);

    // Auto-select when moved into
    // - This will be more fully fleshed in the range-select branch
    // - This is not exposed as it won't nicely work with some user side handling of shift/control
    // - We cannot do 'if (g.NavJustMovedToId != id) { selected = false; pressed = was_selected; }' for two reasons
    //   - (1) it would require focus scope to be set, need exposing PushFocusScope() or equivalent (e.g. BeginSelection() calling PushFocusScope())
    //   - (2) usage will fail with clipped items
    //   The multi-select API aim to fix those issues, e.g. may be replaced with a BeginSelection() API.
    if ((flags & ImGuiSelectableFlags_SelectOnNav) && g.NavJustMovedToId != 0 && g.NavJustMovedToFocusScopeId == window->DC.NavLayerCurrent)
        if (g.NavJustMovedToId == id)
            selected = pressed = true;

    // Update NavId when clicking or when Hovering (this doesn't happen on most widgets), so navigation can be resumed with gamepad/keyboard
    if (pressed || (hovered && (flags & ImGuiSelectableFlags_SetNavIdOnHover)))
    {
        if (g.NavWindow == window && g.NavLayer == window->DC.NavLayerCurrent)
        {
            SetNavID(id, window->DC.NavLayerCurrent, window->DC.NavLayersActiveMask, WindowRectAbsToRel(window, bb));
            PushItemFlag(ImGuiItemFlags_NoNavDisableMouseHover, true);
            PopItemFlag();
        }
    }
    if (pressed)
        MarkItemEdited(id);

    if (flags & ImGuiSelectableFlags_AllowOverlap)
        ImGui::SetItemAllowOverlap();

    // In this branch, Selectable() cannot toggle the selection so this will never trigger.
    if (selected != was_selected) //-V547
        g.LastItemData.StatusFlags |= ImGuiItemStatusFlags_ToggledSelection;

    // Render
    if (held && (flags & ImGuiSelectableFlags_SetNavIdOnHover))
        hovered = true;

    RenderNavHighlight(bb, id, ImGuiNavHighlightFlags_NoRounding);

    static std::map<ImGuiID, selectable_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end())
    {
        anim.insert({ id, { 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.opacity = ImLerp(it_anim->second.opacity, (selected ? 1.0f : hovered ? 0.5f :
        0.2f),
        0.09f * (1.0f - ImGui::GetIO().DeltaTime));

    if (span_all_columns && window->DC.CurrentColumns)
        PopColumnsBackground();
    else if (span_all_columns && g.CurrentTable)
        TablePopBackgroundChannel();

    PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, it_anim->second.opacity));
    {
        RenderTextClipped(text_min, text_max, label, NULL, &label_size, style.SelectableTextAlign, &bb);
    }
    PopStyleColor();

    // Automatically close popups
    if (pressed && (window->Flags & ImGuiWindowFlags_Popup) && !(flags & ImGuiSelectableFlags_DontClosePopups) && !(g.LastItemData.StatusFlags))
        CloseCurrentPopup();

    if (disabled_item && !disabled_global)
        EndDisabled();

    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    return pressed; //-V1020
}

bool ezWidgets::selectableStyle1(const char* label, bool* p_selected, ImGuiSelectableFlags flags, const ImVec2& size_arg)
{
    if (selectableStyle1(label, *p_selected, flags, size_arg))
    {
        *p_selected = !*p_selected;
        return true;
    }
    return false;
}

struct combo_element
{
    float open_anim, arrow_anim;
};

bool ezWidgets::beginComboStyle1(const char* label, const char* preview_value, ImGuiComboFlags flags)
{
    ImGuiContext& g = *GImGui;
    ImGuiWindow* window = GetCurrentWindow();

    ImGuiNextWindowDataFlags backup_next_window_data_flags = g.NextWindowData.WindowFlags;
    g.NextWindowData.ClearFlags(); // We behave like Begin() and need to consume those values
    if (window->SkipItems)
        return false;

    const ImGuiStyle& style = g.Style;
    const ImGuiID id = window->GetID(label);
    IM_ASSERT((flags & (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)) != (ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_NoPreview)); // Can't use both flags together
    const float size = GetWindowWidth() - 30.0f;

    const ImVec2 cursor_pos = window->DC.CursorPos;
    const ImRect rect(cursor_pos, ImVec2(cursor_pos.x + size, cursor_pos.y + 53.0f));
    const ImRect clickable(ImVec2(cursor_pos.x, cursor_pos.y + 23.0f), ImVec2(cursor_pos.x + size, cursor_pos.y + 53.0f));

    const ImVec2 arrow_size = ImGui::CalcTextSize("A"); // 9.0f, FLT_MAX, 0.0f, "a"

    ItemSize(rect, style.FramePadding.y);
    if (!ItemAdd(clickable, id, &rect))
        return false;

    // Open on click
    bool hovered, held;
    bool pressed = ButtonBehavior(clickable, id, &hovered, &held);
    const ImGuiID popup_id = ImHashStr("##ComboPopup", 0, id);
    bool popup_open = IsPopupOpen(popup_id, ImGuiPopupFlags_None);
    if (pressed && !popup_open)
    {
        OpenPopupEx(popup_id, ImGuiPopupFlags_None);
        popup_open = true;
    }

    static std::map<ImGuiID, combo_element> anim;
    auto it_anim = anim.find(id);
    if (it_anim == anim.end())
    {
        anim.insert({ id, { 0.0f, 0.0f } });
        it_anim = anim.find(id);
    }

    it_anim->second.open_anim = ImLerp(it_anim->second.open_anim, popup_open ? 1.0f : 0.0f, 0.04f * (1.0f - ImGui::GetIO().DeltaTime));
    it_anim->second.arrow_anim = ImLerp(it_anim->second.arrow_anim, popup_open ? 0.3f : 0.0f, 0.05f * (1.0f - ImGui::GetIO().DeltaTime));

    window->DrawList->AddRectFilled(clickable.Min, clickable.Max, GetColorU32(ImGuiCol_FrameBg), 3.0f);
    window->DrawList->AddRect(clickable.Min, clickable.Max, ImColor(1.0f, 1.0f, 1.0f, 0.03f), 3.0f);

    window->DrawList->AddText(rect.Min, ImColor(255, 255, 255), label);

    RenderTextClipped(
        ImVec2(clickable.Min.x + 14.0f, clickable.Min.y + 7.0f),
        ImVec2(clickable.Max.x - 24.0f, clickable.Max.y + 47.0f),  // note: subtracting -47 = adding 47
        preview_value,
        NULL,
        NULL,
        ImVec2(0.0f, 0.0f)
    );

    //PushFont(combo_arrow); {
    window->DrawList->AddText(ImVec2(clickable.Max.x - 20, (clickable.Min.y + clickable.Max.y) / 2 - arrow_size.y / 2), ImColor(1.0f, 1.0f, 1.0f, 0.2f + it_anim->second.arrow_anim), "A");
    //} PopFont();

    if (!popup_open)
        return false;

    g.NextWindowData.WindowFlags = backup_next_window_data_flags;
    if (!IsPopupOpen(popup_id, ImGuiPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();
        return false;
    }

    // Set popup size
    float w = clickable.GetWidth();
    if (g.NextWindowData.WindowFlags & ImGuiNextWindowDataFlags_HasSizeConstraint)
    {
        g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
    }
    else
    {
        if ((flags & ImGuiComboFlags_HeightMask_) == 0)
            flags |= ImGuiComboFlags_HeightRegular;
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_)); // Only one
        int popup_max_height_in_items = -1;
        if (flags & ImGuiComboFlags_HeightRegular)
            popup_max_height_in_items = 8;
        else if (flags & ImGuiComboFlags_HeightSmall)
            popup_max_height_in_items = 4;
        else if (flags & ImGuiComboFlags_HeightLarge)
            popup_max_height_in_items = 20;
        SetNextWindowSizeConstraints(ImVec2(w, 0.0f), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items) * it_anim->second.open_anim));
    }

    // This is essentially a specialized version of BeginPopupEx()
    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

    // Set position given a custom constraint (peak into expected window size so we can position it)
    // FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
    // FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling FindBestWindowPosForPopupEx()?
    if (ImGuiWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            // Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
            ImVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
            popup_window->AutoPosLastDirection = (flags & ImGuiComboFlags_PopupAlignLeft) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
            ImRect r_outer = GetPopupAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(clickable.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, clickable, ImGuiPopupPositionPolicy_ComboBox);
            pos.y += 3.f;
            SetNextWindowPos(pos);
        }

    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 5)); // Horizontally align ourselves with the framed text
    PushStyleVar(ImGuiStyleVar_PopupRounding, 3.0f);
    PushStyleVar(ImGuiStyleVar_PopupBorderSize, 1.0f);
    PushStyleColor(ImGuiCol_Border, ImVec4(1.0f, 1.0f, 1.0f, 0.03f));
    PushStyleColor(ImGuiCol_PopupBg, ImVec4(ImColor(20, 20, 20, 255)));
    PushStyleColor(ImGuiCol_Text, ImVec4(ImColor(255, 255, 255)));
    bool ret = Begin(name, NULL, window_flags | ImGuiWindowFlags_NoScrollbar);
    PopStyleVar(3);
    PopStyleColor(3);
    if (!ret)
    {
        EndPopup();
        IM_ASSERT(0); // This should never happen as we tested for IsPopupOpen() above
        return false;
    }
    return true;
}

bool ezWidgets::beginComboPopupStyle1(ImGuiID popup_id, const ImRect& bb, ImGuiComboFlags flags)
{
    ImGuiContext& g = *GImGui;
    if (!IsPopupOpen(popup_id, ImGuiPopupFlags_None))
    {
        g.NextWindowData.ClearFlags();
        return false;
    }

    // Set popup size
    float w = bb.GetWidth();
    if (g.NextWindowData.WindowFlags & ImGuiNextWindowDataFlags_HasSizeConstraint)
    {
        g.NextWindowData.SizeConstraintRect.Min.x = ImMax(g.NextWindowData.SizeConstraintRect.Min.x, w);
    }
    else
    {
        if ((flags & ImGuiComboFlags_HeightMask_) == 0)
            flags |= ImGuiComboFlags_HeightRegular;
        IM_ASSERT(ImIsPowerOfTwo(flags & ImGuiComboFlags_HeightMask_)); // Only one
        int popup_max_height_in_items = -1;
        if (flags & ImGuiComboFlags_HeightRegular)
            popup_max_height_in_items = 8;
        else if (flags & ImGuiComboFlags_HeightSmall)
            popup_max_height_in_items = 4;
        else if (flags & ImGuiComboFlags_HeightLarge)
            popup_max_height_in_items = 20;
        ImVec2 constraint_min(0.0f, 0.0f), constraint_max(FLT_MAX, FLT_MAX);
        if ((g.NextWindowData.WindowFlags & ImGuiNextWindowDataFlags_HasSize) == 0 || g.NextWindowData.SizeVal.x <= 0.0f) // Don't apply constraints if user specified a size
            constraint_min.x = w;
        if ((g.NextWindowData.WindowFlags & ImGuiNextWindowDataFlags_HasSize) == 0 || g.NextWindowData.SizeVal.y <= 0.0f)
            constraint_max.y = CalcMaxPopupHeightFromItemCount(popup_max_height_in_items);
        SetNextWindowSizeConstraints(constraint_min, constraint_max);
    }

    // This is essentially a specialized version of BeginPopupEx()
    char name[16];
    ImFormatString(name, IM_ARRAYSIZE(name), "##Combo_%02d", g.BeginPopupStack.Size); // Recycle windows based on depth

    // Set position given a custom constraint (peak into expected window size so we can position it)
    // FIXME: This might be easier to express with an hypothetical SetNextWindowPosConstraints() function?
    // FIXME: This might be moved to Begin() or at least around the same spot where Tooltips and other Popups are calling FindBestWindowPosForPopupEx()?
    if (ImGuiWindow* popup_window = FindWindowByName(name))
        if (popup_window->WasActive)
        {
            // Always override 'AutoPosLastDirection' to not leave a chance for a past value to affect us.
            ImVec2 size_expected = CalcWindowNextAutoFitSize(popup_window);
            popup_window->AutoPosLastDirection = (flags & ImGuiComboFlags_PopupAlignLeft) ? ImGuiDir_Left : ImGuiDir_Down; // Left = "Below, Toward Left", Down = "Below, Toward Right (default)"
            ImRect r_outer = GetPopupAllowedExtentRect(popup_window);
            ImVec2 pos = FindBestWindowPosForPopupEx(bb.GetBL(), size_expected, &popup_window->AutoPosLastDirection, r_outer, bb, ImGuiPopupPositionPolicy_ComboBox);
            SetNextWindowPos(pos);
        }

    // We don't use BeginPopupEx() solely because we have a custom name string, which we could make an argument to BeginPopupEx()
    ImGuiWindowFlags window_flags = ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_Popup | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoMove;
    PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(g.Style.FramePadding.x, g.Style.WindowPadding.y)); // Horizontally align ourselves with the framed text
    bool ret = Begin(name, NULL, window_flags);
    PopStyleVar();
    if (!ret)
    {
        EndPopup();
        IM_ASSERT(0); // This should never happen as we tested for IsPopupOpen() above
        return false;
    }
    return true;
}

bool ezWidgets::comboBoxStyle1(const char* label, int* current_item, const char* (*getter)(void* user_data, int idx), void* user_data, int items_count, int popup_max_height_in_items)
{
    ImGuiContext& g = *GImGui;

    // Call the getter to obtain the preview string which is a parameter to BeginCombo()
    const char* preview_value = NULL;
    if (*current_item >= 0 && *current_item < items_count)
        preview_value = getter(user_data, *current_item);

    // The old Combo() API exposed "popup_max_height_in_items". The new more general BeginCombo() API doesn't have/need it, but we emulate it here.
    if (popup_max_height_in_items != -1 && !(g.NextWindowData.WindowFlags & ImGuiNextWindowDataFlags_HasSizeConstraint))
        SetNextWindowSizeConstraints(ImVec2(0, 0), ImVec2(FLT_MAX, CalcMaxPopupHeightFromItemCount(popup_max_height_in_items)));

    if (!beginComboStyle1(label, preview_value, ImGuiComboFlags_None))
        return false;

    // Display items
    // FIXME-OPT: Use clipper (but we need to disable it on the appearing frame to make sure our call to SetItemDefaultFocus() is processed)
    bool value_changed = false;
    for (int i = 0; i < items_count; i++)
    {
        const char* item_text = getter(user_data, i);
        if (item_text == NULL)
            item_text = "*Unknown item*";

        PushID(i);
        const bool item_selected = (i == *current_item);
        if (selectableStyle1(item_text, item_selected) && *current_item != i) // selectableStyle1(item_text, item_selected)
        {
            value_changed = true;
            *current_item = i;
        }
        if (item_selected)
            SetItemDefaultFocus();
        PopID();
    }

    EndCombo();

    if (value_changed)
        MarkItemEdited(g.LastItemData.ID);

    return value_changed;
}

bool ezWidgets::comboBoxHelper1(const char* label, int* current_item, const char* const items[], int items_count, int height_in_items)
{
    const bool value_changed = comboBoxStyle1(label, current_item, Items_ArrayGetter, (void*)items, items_count, height_in_items);
    return value_changed;
}