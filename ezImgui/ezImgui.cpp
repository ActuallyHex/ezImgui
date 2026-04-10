#include "ezImgui.h"
#include <imgui_internal.h>

namespace ez {

    //=================================================================================================
    // Helpers - Helps build a UIElement with a pre-bound render delegate
    //=================================================================================================

    // Push a fully constructed UIElement onto any vector<UIElement>
    // The caller fills in type, renderFn, and heightHint then passes ownership here
    static void PushElement(std::vector<UIElement>& vec, UIElement e) {
        vec.push_back(std::move(e));
    }

    // Build a checkbox element resolving the style to a concrete renderFn right here
    static UIElement MakeCheckbox(const char* label, bool* value, CheckboxStyle style) {
        UIElement e;
        e.type = ElementType::Toggle;
        e.heightHint = 1.35f;

        std::string lbl = label; // captured by value so the lambda is self-contained
        switch (style) {
#ifdef USE_EZ_WIDGETS
        case CheckboxStyle::ToggleSwitch:
            e.renderFn = [lbl, value]() { ezWidgets::ToggleSwitch(lbl.c_str(), value); };
            break;
        case CheckboxStyle::Anim1:
            e.renderFn = [lbl, value]() { ezWidgets::CheckboxAnim1(lbl.c_str(), value); };
            break;
        case CheckboxStyle::Anim2:
            e.renderFn = [lbl, value]() { ezWidgets::CheckboxAnim2(lbl.c_str(), value); };
            break;
#endif
        case CheckboxStyle::ImGuiDefault:
        default:
            e.renderFn = [lbl, value]() { ImGui::Checkbox(lbl.c_str(), value); };
            break;
        }
        return e;
    }

    // Build a checkbox + color picker element
    static UIElement MakeCheckboxColorPicker(const char* label, bool* value, ImVec4* color, CheckboxStyle style) {
        UIElement e;
        e.type = ElementType::ToggleColorPicker;
        e.heightHint = 1.35f;

        std::string lbl = label;
        switch (style) {
#ifdef USE_EZ_WIDGETS
        case CheckboxStyle::ToggleSwitch:
            e.renderFn = [lbl, value, color]() {
                ezWidgets::ToggleSwitch(lbl.c_str(), value);
                float fullWidth = ImGui::GetContentRegionAvail().x;
                ImGui::SameLine(ImGui::GetCursorPosX() + fullWidth - 25.0f);
                ImGui::ColorEdit4(("##" + lbl).c_str(), (float*)color,
                    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                };
            break;
        case CheckboxStyle::Anim1:
            e.renderFn = [lbl, value, color]() {
                ezWidgets::CheckboxAnim1(lbl.c_str(), value);
                float fullWidth = ImGui::GetContentRegionAvail().x;
                ImGui::SameLine(ImGui::GetCursorPosX() + fullWidth - 25.0f);
                ImGui::ColorEdit4(("##" + lbl).c_str(), (float*)color,
                    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                };
            break;
        case CheckboxStyle::Anim2:
            e.renderFn = [lbl, value, color]() {
                ezWidgets::CheckboxAnim2(lbl.c_str(), value);
                float fullWidth = ImGui::GetContentRegionAvail().x;
                ImGui::SameLine(ImGui::GetCursorPosX() + fullWidth - 25.0f);
                ImGui::ColorEdit4(("##" + lbl).c_str(), (float*)color,
                    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                };
            break;
#endif
        case CheckboxStyle::ImGuiDefault:
        default:
            e.renderFn = [lbl, value, color]() {
                ImGui::Checkbox(lbl.c_str(), value);
                float fullWidth = ImGui::GetContentRegionAvail().x;
                ImGui::SameLine(ImGui::GetCursorPosX() + fullWidth - 25.0f);
                ImGui::ColorEdit4(("##" + lbl).c_str(), (float*)color,
                    ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
                };
            break;
        }
        return e;
    }

    // Build a float slider element
    static UIElement MakeSliderFloat(const char* label, float* value, float min, float max, SliderStyle style) {
        UIElement e;
        e.type = ElementType::SliderFloat;
        std::string lbl = label;

        switch (style) {
#ifdef USE_EZ_WIDGETS
        case SliderStyle::Style1:
            e.heightHint = 1.90f;
            e.renderFn = [lbl, value, min, max]() {
                ezWidgets::SliderStyle1(lbl.c_str(), value, min, max, "%.0f", 0);
                };
            break;
        case SliderStyle::Style2:
            e.heightHint = 1.90f;
            e.renderFn = [lbl, value, min, max]() {
                ezWidgets::SliderStyle2(lbl.c_str(), value, min, max, "%.0f", 0);
                };
            break;
#endif
        case SliderStyle::ImGuiDefault:
        default:
            e.heightHint = 1.35f;
            e.renderFn = [lbl, value, min, max]() {
                ImGui::SliderFloat(lbl.c_str(), value, min, max);
                };
            break;
        }
        return e;
    }

    // Build an int slider element
    static UIElement MakeSliderInt(const char* label, int* value, int min, int max, SliderStyle style) {
        UIElement e;
        e.type = ElementType::SliderInt;
        std::string lbl = label;

        switch (style) {
#ifdef USE_EZ_WIDGETS
        case SliderStyle::Style1:
            e.heightHint = 1.90f;
            e.renderFn = [lbl, value, min, max]() {
                ezWidgets::SliderStyle1(lbl.c_str(), value, min, max, "%d", 0);
                ImGui::Dummy(ImVec2(0.0f, 4.f));
                };
            break;
        case SliderStyle::Style2:
            e.heightHint = 1.00f;
            e.renderFn = [lbl, value, min, max]() {
                ezWidgets::SliderStyle2(lbl.c_str(), value, min, max, "%d", 0);
                ImGui::Dummy(ImVec2(0.0f, 4.f));
                };
            break;
#endif
        case SliderStyle::ImGuiDefault:
        default:
            e.heightHint = 1.35f;
            e.renderFn = [lbl, value, min, max]() {
                ImGui::SliderInt(lbl.c_str(), value, min, max);
                };
            break;
        }
        return e;
    }

    // Build a button element
    static UIElement MakeButton(const char* label, std::function<void()> onClick, ButtonStyle style) {
        UIElement e;
        e.type = ElementType::Button;
        e.heightHint = 1.35f;
        std::string lbl = label;

        switch (style) {
#ifdef USE_EZ_WIDGETS
        case ButtonStyle::Style1:
            e.renderFn = [lbl, onClick]() {
                if (ezWidgets::IconButton1(lbl.c_str(), ImVec2(-FLT_MIN, 0.0f), false) && onClick)
                    onClick();
                };
            break;
#endif
        case ButtonStyle::ImGuiDefault:
        default:
            e.renderFn = [lbl, onClick]() {
                if (ImGui::Button(lbl.c_str(), ImVec2(-FLT_MIN, 0.0f)) && onClick)
                    onClick();
                };
            break;
        }
        return e;
    }

    // Build a gradient button element (EzWidgets only; no-op otherwise)
    static UIElement MakeGradientButton(const char* label, ImU32 textClr, ImU32 bgClr1, ImU32 bgClr2, std::function<void()> onClick) {
        UIElement e;
        e.type = ElementType::GradientButton;
        e.heightHint = 1.20f;
        std::string lbl = label;

#ifdef USE_EZ_WIDGETS
        e.renderFn = [lbl, textClr, bgClr1, bgClr2, onClick]() {
            if (ezWidgets::ColoredButtonV1(lbl.c_str(), ImVec2(-FLT_MIN, 0.0f), textClr, bgClr1, bgClr2) && onClick)
                onClick();
            };
#else
        e.renderFn = []() {}; // no-op when EzWidgets is absent
#endif

        return e;
    }

    // Build a combo box element, owning its own item storage.
    static UIElement MakeComboBox(const char* label, int* currentItem,
        std::initializer_list<const char*> itemsInitList,
        int heightInItems, ComboBoxStyle style)
    {
        UIElement e;
        e.type = ElementType::ComboBox;
        e.heightHint = 1.35f;

        // String storage and pointer list are kept in shared_ptrs so they have a stable heap address regardless of how the UIElement vector reallocates
        // Capturing raw pointers into e.comboItemStorage/e.comboItemPointers would dangle after the first vector reallocation
        auto itemStorage = std::make_shared<std::vector<std::string>>();
        auto itemPointers = std::make_shared<std::vector<const char*>>();

        for (const auto& s : itemsInitList)
            itemStorage->emplace_back(s);
        for (const auto& str : *itemStorage)
            itemPointers->push_back(str.c_str());

        // Also keep copies on the element itself so callers that inspect the element directly still work
        e.comboItemStorage = *itemStorage;
        e.comboItemPointers = *itemPointers;

        std::string lbl = label;
        int         itemsCount = static_cast<int>(itemPointers->size());

        switch (style) {
#ifdef USE_EZ_WIDGETS
        case ComboBoxStyle::Style1:
            // itemStorage must be captured alongside itemPointers: itemPointers holds
            // const char* into the std::strings owned by itemStorage, so if itemStorage
            // is destroyed those pointers dangle even though itemPointers itself survives
            e.renderFn = [lbl, currentItem, itemStorage, itemPointers, itemsCount, heightInItems]() {
                ezWidgets::comboBoxHelper1(lbl.c_str(), currentItem,
                    itemPointers->data(), itemsCount, heightInItems);
                };
            break;
#endif
        case ComboBoxStyle::ImGuiDefault:
        default:
            e.renderFn = [lbl, currentItem, itemStorage, itemPointers, itemsCount]() {
                const char* preview = nullptr;
                if (currentItem && *currentItem >= 0 && *currentItem < itemsCount)
                    preview = (*itemPointers)[*currentItem];
                if (ImGui::BeginCombo(lbl.c_str(), preview, 0)) {
                    for (int n = 0; n < itemsCount; n++) {
                        const bool selected = (*currentItem == n);
                        if (ImGui::Selectable((*itemPointers)[n], selected))
                            *currentItem = n;
                        if (selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                };
            break;
        }
        return e;
    }

    // Build a multi combo box element
    static UIElement MakeMultiComboBox(const char* label,
        std::initializer_list<const char*> itemsList,
        std::shared_ptr<std::unordered_map<int, bool>> data)
    {
        UIElement e;
        e.type = ElementType::MultiComboBox;
        e.heightHint = 1.35f;
        e.multiComboData = std::move(data);

        // Same stable-address fix as MakeComboBox: keep the pointer list in a
        // shared_ptr so the lambda capture is never invalidated by a vector realloc
        auto itemStorage = std::make_shared<std::vector<std::string>>(itemsList.begin(), itemsList.end());
        auto itemPointers = std::make_shared<std::vector<const char*>>();
        itemPointers->reserve(itemStorage->size());
        for (const auto& s : *itemStorage)
            itemPointers->push_back(s.c_str());

        // Mirror onto the element for any external inspection
        e.comboItemStorage = *itemStorage;
        e.multiComboRawItems = *itemPointers;

        std::string lbl = label;
        auto* dataPtr = e.multiComboData.get();

        e.renderFn = [lbl, dataPtr, itemStorage, itemPointers]() {
            ImGuiMultiComboBox(lbl.c_str(), dataPtr, *itemPointers);
            };
        return e;
    }

    // Build a standalone color picker element
    static UIElement MakeColorPicker(const char* label, ImVec4* color) {
        UIElement e;
        e.type = ElementType::ColorPicker;
        e.heightHint = 1.42f;
        std::string lbl = label;
        e.renderFn = [lbl, color]() {
            ImGui::ColorEdit4(lbl.c_str(), (float*)color, ImGuiColorEditFlags_NoInputs);
            };
        return e;
    }

    // Build a text label element
    static UIElement MakeLabel(const char* label) {
        UIElement e;
        e.type = ElementType::Label;
        e.heightHint = 0.6f;
        std::string lbl = label;
        e.renderFn = [lbl]() { ImGui::Text("%s", lbl.c_str()); };
        return e;
    }

    // Build an outlined text element
    // The renderFn resolves the draw list at call-time (not build-time) so it always writes into whichever window is active when Render() fires
    static UIElement MakeTextWithOutline(const char* text,
        ImU32        fillColor,
        ImU32        outlineColor,
        float        outlineThickness,
        ImFont* font,
        float        fontSize,
        bool         cpuFineAA)
    {
        UIElement e;
        e.type = ElementType::Label; // same height behaviour as a plain label
        e.heightHint = 1.0f;

        std::string txt = text;

        e.renderFn = [txt, fillColor, outlineColor, outlineThickness,
            font, fontSize, cpuFineAA]()
            {
                // GetCursorScreenPos gives the correct top-left for the current layout position matching where ImGui::Text would place itself
                ImVec2 pos = ImGui::GetCursorScreenPos();

                // Resolve font/size here so any font pushed after build-time is respected (if the caller pushes a font before Render())
                ImFont* resolvedFont = font ? font : ImGui::GetFont();
                float   resolvedFontSize = fontSize > 0.0f ? fontSize : resolvedFont->FontSize;

                AddTextWithOutline(
                    ImGui::GetWindowDrawList(),
                    resolvedFont,
                    resolvedFontSize,
                    pos,
                    fillColor,
                    outlineColor,
                    txt.c_str(),
                    outlineThickness,
                    0.0f,      // wrapWidth: no wrap by default inside a tabbox
                    cpuFineAA
                );

                // Advance the layout cursor by the same amount ImGui::Text would so subsequent elements position correctly beneath this one
                ImVec2 textSize = resolvedFont->CalcTextSizeA(
                    resolvedFontSize, FLT_MAX, 0.0f, txt.c_str());
                ImGui::Dummy(textSize);
            };

        return e;
    }

    //=================================================================================================
    // TabboxTab Add* methods
    // Each method delegates to the corresponding Make* helper above and pushes the resulting element
    // *No style logic lives here
    //=================================================================================================

    void TabboxTab::AddCheckbox(const char* label, bool* value, CheckboxStyle style) {
        elements.push_back(MakeCheckbox(label, value, style));
    }

    void TabboxTab::AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color, CheckboxStyle style) {
        elements.push_back(MakeCheckboxColorPicker(label, value, color, style));
    }

    void TabboxTab::AddSlider(const char* label, float* value, float min, float max, SliderStyle style) {
        elements.push_back(MakeSliderFloat(label, value, min, max, style));
    }

    void TabboxTab::AddSlider(const char* label, int* value, int min, int max, SliderStyle style) {
        elements.push_back(MakeSliderInt(label, value, min, max, style));
    }

    void TabboxTab::AddColorPicker(const char* label, ImVec4* color) {
        elements.push_back(MakeColorPicker(label, color));
    }

    void TabboxTab::AddLabel(const char* label) {
        elements.push_back(MakeLabel(label));
    }

    void TabboxTab::AddTextWithOutline(const char* text, ImU32 fillColor, ImU32 outlineColor,
        float outlineThickness, ImFont* font, float fontSize,
        bool cpuFineAA)
    {
        elements.push_back(MakeTextWithOutline(text, fillColor, outlineColor,
            outlineThickness, font, fontSize, cpuFineAA));
    }

    void TabboxTab::AddMultiComboBox(const char* label, std::initializer_list<const char*> items,
        std::shared_ptr<std::unordered_map<int, bool>> data)
    {
        elements.push_back(MakeMultiComboBox(label, items, std::move(data)));
    }

    void TabboxTab::AddButton(const char* label, std::function<void()> onClick, ButtonStyle style) {
        elements.push_back(MakeButton(label, std::move(onClick), style));
    }

    void TabboxTab::AddGradientButton(const char* label, ImU32 textClr, ImU32 bgClr1, ImU32 bgClr2,
        std::function<void()> onClick)
    {
        elements.push_back(MakeGradientButton(label, textClr, bgClr1, bgClr2, std::move(onClick)));
    }

    void TabboxTab::AddComboBox(const char* label, int* current_item,
        std::initializer_list<const char*> items,
        int height_in_items, ComboBoxStyle style)
    {
        elements.push_back(MakeComboBox(label, current_item, items, height_in_items, style));
    }

    //=================================================================================================
    // Tabbox Add* methods
    //=================================================================================================

    void Tabbox::AddCheckbox(const char* label, bool* value, CheckboxStyle style) {
        elements.push_back(MakeCheckbox(label, value, style));
    }

    void Tabbox::AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color, CheckboxStyle style) {
        elements.push_back(MakeCheckboxColorPicker(label, value, color, style));
    }

    void Tabbox::AddSlider(const char* label, float* value, float min, float max, SliderStyle style) {
        elements.push_back(MakeSliderFloat(label, value, min, max, style));
    }

    void Tabbox::AddSlider(const char* label, int* value, int min, int max, SliderStyle style) {
        elements.push_back(MakeSliderInt(label, value, min, max, style));
    }

    void Tabbox::AddColorPicker(const char* label, ImVec4* color) {
        elements.push_back(MakeColorPicker(label, color));
    }

    void Tabbox::AddLabel(const char* label) {
        elements.push_back(MakeLabel(label));
    }

    void Tabbox::AddTextWithOutline(const char* text, ImU32 fillColor, ImU32 outlineColor,
        float outlineThickness, ImFont* font, float fontSize,
        bool cpuFineAA)
    {
        elements.push_back(MakeTextWithOutline(text, fillColor, outlineColor,
            outlineThickness, font, fontSize, cpuFineAA));
    }

    void Tabbox::AddMultiComboBox(const char* label, std::initializer_list<const char*> items,
        std::shared_ptr<std::unordered_map<int, bool>> data)
    {
        elements.push_back(MakeMultiComboBox(label, items, std::move(data)));
    }

    void Tabbox::AddButton(const char* label, std::function<void()> onClick, ButtonStyle style) {
        elements.push_back(MakeButton(label, std::move(onClick), style));
    }

    void Tabbox::AddGradientButton(const char* label, ImU32 textClr, ImU32 bgClr1, ImU32 bgClr2,
        std::function<void()> onClick)
    {
        elements.push_back(MakeGradientButton(label, textClr, bgClr1, bgClr2, std::move(onClick)));
    }

    void Tabbox::AddComboBox(const char* label, int* current_item,
        std::initializer_list<const char*> items,
        int height_in_items, ComboBoxStyle style)
    {
        elements.push_back(MakeComboBox(label, current_item, items, height_in_items, style));
    }

    //=================================================================================================
    // Notifications
    //=================================================================================================
    void PushNotification(const std::string& text, float duration) {
        g_Notifications.push_back(Notification{ text, duration, 0.0f, -200.0f, 1.0f });
    }

    void RenderNotifications() {
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const float padding = 10.0f;
        const float box_rounding = 0.0f;

        int index = 0;
        for (int i = 0; i < (int)g_Notifications.size(); ) {
            Notification& n = g_Notifications[i];
            n.age += io.DeltaTime;

            if (n.lifetime - n.age < 0.5f)
                n.alpha = ImClamp((n.lifetime - n.age) / 0.2f, 0.0f, 1.0f);

            if (n.slide < 0.0f) {
                n.slide = ImLerp(n.slide, 0.0f, ImClamp(io.DeltaTime * 10.0f, 0.0f, 1.0f));
                if (n.slide > -1.0f) n.slide = 0.0f;
            }
            if (n.slide > 0.0f) n.slide = 0.0f;

            ImVec2 text_size = ImGui::CalcTextSize(n.text.c_str());
            float box_width = text_size.x + 2 * padding;
            float box_height = text_size.y + 2 * padding;
            ImVec2 pos = ImVec2(padding + n.slide, padding + index * (box_height + 6));
            ImVec2 rect_min = pos;
            ImVec2 rect_max = ImVec2(pos.x + box_width, pos.y + box_height);

            ImVec2 rect_outline_min = ImVec2(pos.x - 1.4f, pos.y - 1.4f);
            ImVec2 rect_outline_max = ImVec2(pos.x + box_width + 1.4f, pos.y + box_height + 1.4f);

            ImU32 bg_border_col = ImGui::ColorConvertFloat4ToU32(ImVec4(0.0f, 0.0f, 0.0f, n.alpha));
            ImU32 bg_col = ImGui::ColorConvertFloat4ToU32(ImVec4(0.07f, 0.07f, 0.07f, n.alpha));
            ImU32 border_col = ImGui::ColorConvertFloat4ToU32(ImVec4(ez::accentColor.x, ez::accentColor.y, ez::accentColor.z, n.alpha));
            ImU32 text_col = ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, n.alpha));

            draw_list->AddRectFilled(rect_min, rect_max, bg_col, box_rounding);
            draw_list->AddRect(ImVec2(rect_outline_min.x - 0.4f, rect_outline_min.y - 0.4f),
                ImVec2(rect_outline_max.x + 0.4f, rect_outline_max.y + 0.4f),
                bg_border_col, box_rounding);
            draw_list->AddRect(rect_outline_min, rect_outline_max, border_col, box_rounding);
            draw_list->AddText(ImVec2(pos.x + padding, pos.y + padding), text_col, n.text.c_str());

            if (n.age >= n.lifetime)
                g_Notifications.erase(g_Notifications.begin() + i);
            else { ++i; ++index; }
        }
    }

    //=================================================================================================
    // Fonts
    //=================================================================================================
    void LoadFont(const std::string& name, const char* path, float size) {
        ImFont* font = ImGui::GetIO().Fonts->AddFontFromFileTTF(path, size);
        if (font) fonts[name] = font;
    }

    void LoadFontFromMemory(const std::string& name, void* data, int size_bytes, float size_pixels) {
        ImFontConfig cfg;
        cfg.FontDataOwnedByAtlas = false;
        ImFont* font = ImGui::GetIO().Fonts->AddFontFromMemoryTTF(data, size_bytes, size_pixels, &cfg);
        if (font) fonts[name] = font;
    }

    void SetFont(const std::string& name) {
        if (fonts.contains(name)) {
            currentFontName = name;
            ImGui::PushFont(fonts[name]);
        }
    }

    //=================================================================================================
    // Utility
    //=================================================================================================
    void RenderFullWidthSeparator(float thickness, ImVec4 color) {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        if (window->SkipItems) return;

        float startX = window->Pos.x;
        float endX = window->Pos.x + window->Size.x;
        float y = ImGui::GetCursorScreenPos().y;

        window->DrawList->AddLine(ImVec2(startX, y), ImVec2(endX, y), ImColor(color), thickness);
        ImGui::Dummy(ImVec2(0.5f, thickness));
    }

    bool ImGuiMultiComboBox(const char* label, std::unordered_map<int, bool>* data, std::vector<const char*> items) {
        std::string preview;
        for (size_t i = 0; i < items.size(); ++i)
            if ((*data)[i]) preview += std::string(preview.empty() ? "" : ", ") + items[i];

        const char* preview_value = preview.empty() ? "none" : preview.c_str();
        if (ImGui::BeginCombo(label, preview_value)) {
            for (size_t i = 0; i < items.size(); ++i) {
                bool selected = (*data)[i];
                if (ImGui::Selectable(items[i], selected, ImGuiSelectableFlags_DontClosePopups))
                    (*data)[i] = !(*data)[i];
            }
            ImGui::EndCombo();
        }
        return true;
    }

    //=================================================================================================
    // AddTextWithOutline
    //
    // Render the text into a throw-away ImDrawList so we can inspect its
    // vertex buffer without touching the real draw list yet
    // 
    // Walk the resulting quads (4 verts each, 6 indices each) For every quad compute its screen-space centroid then emit a new expanded quad
    // into the real draw list in the outline color
    // 
    // Copy the original fill quads into the real draw list on top
    //
    // This produces correct per-glyph outlines in a single merged geometry batch without needing extra font glyphs or modifying the shaders
    //
    //=================================================================================================

    namespace {

        // Unpack an ImU32 color into normalised floats.
        inline void UnpackColor(ImU32 col, float& r, float& g, float& b, float& a) {
            r = ((col >> 0) & 0xFF) / 255.0f;
            g = ((col >> 8) & 0xFF) / 255.0f;
            b = ((col >> 16) & 0xFF) / 255.0f;
            a = ((col >> 24) & 0xFF) / 255.0f;
        }

        // Repack normalised floats back into ImU32.
        inline ImU32 PackColor(float r, float g, float b, float a) {
            return IM_COL32(
                (int)(ImClamp(r, 0.f, 1.f) * 255.f + 0.5f),
                (int)(ImClamp(g, 0.f, 1.f) * 255.f + 0.5f),
                (int)(ImClamp(b, 0.f, 1.f) * 255.f + 0.5f),
                (int)(ImClamp(a, 0.f, 1.f) * 255.f + 0.5f)
            );
        }

        // Given four vertex positions forming a glyph quad compute the centroid and push each vertex outward by "amount" pixels
        // Vertices are assumed in imguis standard order:
        //   0=top-left  1=top-right  2=bottom-right  3=bottom-left
        inline void ExpandQuad(const ImVec2 src[4], ImVec2 dst[4], float amount) {
            ImVec2 c(
                (src[0].x + src[1].x + src[2].x + src[3].x) * 0.25f,
                (src[0].y + src[1].y + src[2].y + src[3].y) * 0.25f
            );
            for (int i = 0; i < 4; ++i) {
                ImVec2 dir(src[i].x - c.x, src[i].y - c.y);
                float  len = ImSqrt(dir.x * dir.x + dir.y * dir.y);
                if (len > 0.0f) { dir.x /= len; dir.y /= len; }
                dst[i] = ImVec2(src[i].x + dir.x * amount,
                    src[i].y + dir.y * amount);
            }
        }

        // Emit one quad into drawList using pre-computed positions uvs and color uses PrimReserve + direct write to avoid any hidden reallocation
        // Winding matches ImGui: triangles {0,1,2} and {0,2,3}
        inline void EmitQuad(ImDrawList* dl,
            const ImVec2  pos[4],
            const ImVec2  uv[4],
            ImU32         col)
        {
            dl->PrimReserve(6, 4);

            ImDrawVert* vtx = dl->_VtxWritePtr;
            ImDrawIdx* idx = dl->_IdxWritePtr;
            ImDrawIdx   base = (ImDrawIdx)dl->_VtxCurrentIdx;

            for (int i = 0; i < 4; ++i) {
                vtx[i].pos = pos[i];
                vtx[i].uv = uv[i];
                vtx[i].col = col;
            }

            idx[0] = base;     idx[1] = base + 1; idx[2] = base + 2;
            idx[3] = base;     idx[4] = base + 2; idx[5] = base + 3;

            dl->_VtxWritePtr += 4;
            dl->_IdxWritePtr += 6;
            dl->_VtxCurrentIdx += 4;
        }

    } // anon namespace

    void AddTextWithOutline(ImDrawList* drawList,
        ImFont* font,
        float        fontSize,
        ImVec2       pos,
        ImU32        fillColor,
        ImU32        outlineColor,
        const char* text,
        float        outlineThickness,
        float        wrapWidth,
        bool         cpuFineAA)
    {
        if (!text || text[0] == '\0') return;
        if (!drawList)                  return;

        if (!font)            font = ImGui::GetFont();
        if (fontSize <= 0.0f) fontSize = font->FontSize;

        const char* textEnd = text + strlen(text);
        // capture glyph quads via ImFont::RenderText into a
        // temporary draw list

        // ImFont::RenderText writes directly into whatever draw list you
        // pass it without touching clip rect state or texture commands so its safe to use with a bare scratch list so this avoids the clip-rect/draw-command ordering issues
        ImDrawList scratch(drawList->_Data);
        scratch._ResetForNewFrame();

        // Give the scratch list a large clip rect so nothing is culled
        // RenderText respects the clip rect in ClipRectStack then we we push our own so every glyph is guaranteed to produce geometry
        scratch.PushClipRectFullScreen();
        scratch.PushTextureID(font->ContainerAtlas->TexID);

        // We pass a maximally large clip_rect so nothing is culled here
        ImVec4 noClip(-8192.f, -8192.f, 8192.f, 8192.f);
        font->RenderText(&scratch, fontSize, pos, fillColor,
            noClip, text, textEnd, wrapWidth, false);

        // walk the scratch vertex buffer in strides of 4
        // ImGui emits exactly 4 verts per visible glyph so whitespace and newlines produce no geometry
        const int totalVerts = scratch.VtxBuffer.Size;
        if (totalVerts == 0) return;

        // Sub-pixel alpha softening for thin outlines.
        ImU32 resolvedOutlineColor = outlineColor;
        if (cpuFineAA && outlineThickness < 1.5f) {
            float r, g, b, a;
            UnpackColor(outlineColor, r, g, b, a);
            a *= ImClamp(outlineThickness, 0.0f, 1.0f);
            resolvedOutlineColor = PackColor(r, g, b, a);
        }

        // Walk strides of 4 verts one stride = one glyph quad
        for (int vi = 0; vi + 3 < totalVerts; vi += 4) {
            const ImDrawVert& v0 = scratch.VtxBuffer[vi + 0]; // top-left
            const ImDrawVert& v1 = scratch.VtxBuffer[vi + 1]; // top-right
            const ImDrawVert& v2 = scratch.VtxBuffer[vi + 2]; // bottom-right
            const ImDrawVert& v3 = scratch.VtxBuffer[vi + 3]; // bottom-left

            ImVec2 srcPos[4] = { v0.pos, v1.pos, v2.pos, v3.pos };
            ImVec2 uvs[4] = { v0.uv,  v1.uv,  v2.uv,  v3.uv };

            // Outline quad first (behind fill)
            ImVec2 outlinePos[4];
            ExpandQuad(srcPos, outlinePos, outlineThickness);
            EmitQuad(drawList, outlinePos, uvs, resolvedOutlineColor);

            // Fill quad on top.
            EmitQuad(drawList, srcPos, uvs, fillColor);
        }
    }

    //=================================================================================================
    // Render helpers
    //=================================================================================================
    void TabboxTab::Render() {
        for (auto& e : elements)
            if (e.renderFn) e.renderFn();
    }

    void Tabbox::RenderExtras() {
        for (auto& e : elements)
            if (e.renderFn) e.renderFn();
    }

    //=================================================================================================
    // Window and Tab factory methods
    //=================================================================================================
    Window::Window(const char* title_, ImVec2 size_, ImGuiWindowFlags flags_, bool autoshow_)
        : title(title_), size(size_), flags(flags_), autoshow(autoshow_) {
    }

    std::shared_ptr<Window> CreateEzWindow(const char* title, ImVec2 size, ImGuiWindowFlags flags, bool autoshow) {
        return std::make_shared<Window>(title, size, flags, autoshow);
    }

    std::shared_ptr<Tab> Window::AddTab(const char* name) {
        tabNames.emplace_back(name);
        auto tab = std::make_shared<Tab>();
        tab->name = name;
        tabs.push_back(tab);
        return tab;
    }

    std::shared_ptr<Tabbox> Tab::AddTabbox(const char* name, TabboxSide side) {
        auto tabbox = std::make_shared<Tabbox>();
        tabbox->name = name;
        tabbox->side = side;
        tabboxes.push_back(tabbox);
        return tabbox;
    }

    std::shared_ptr<TabboxTab> Tabbox::AddTab(const char* name) {
        auto tab = std::make_shared<TabboxTab>();
        tab->name = name;
        tabs.push_back(tab);
        return tab;
    }

    //=================================================================================================
    // Window::Render
    //=================================================================================================
    void Window::Render() {

        if (center) {
            ImVec2 centerPos = ImGui::GetMainViewport()->GetCenter();
            auto tempSize = ImVec2(size.x * 0.5f, size.y * 0.5f);
            centerPos.x -= tempSize.x;
            centerPos.y -= tempSize.y;
            ImGui::SetNextWindowPos(centerPos, ImGuiCond_FirstUseEver);
        }

        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ez::winBackgroundColor);
        ImGui::Begin(title.c_str(), &isOpen, flags);

        static int testBar = 0;
        ImVec2 contentSize;

        if (fonts.contains(currentFontName))
            ImGui::PushFont(fonts[currentFontName]);

        ImGui::Spacing();

        if (!tabNames.empty()) {
            if (tabMode == TabMode::ImGuiTabs) {
                contentSize = ImVec2(size.x - 15, size.y - 66);

                if (ImGui::BeginTabBar("##tabs")) {
                    for (int i = 0; i < (int)tabNames.size(); i++) {
                        if (ImGui::BeginTabItem(tabNames[i].c_str())) {
                            testBar = i;
                            ImGui::EndTabItem();
                        }
                    }
                    ImGui::EndTabBar();
                }
            }
            else if (tabMode == TabMode::ButtonTabs) {
                float buttonHeight = 30.0f;

                if (tabButtonOrientation == TabButtonOrientation::HorizontalTop) {
                    ImGui::BeginChild("##ButtonTabBar", ImVec2(0, buttonHeight), false);
                    for (size_t i = 0; i < tabNames.size(); i++) {
                        if (i > 0) ImGui::SameLine();
                        ImVec4 normalColor = ImVec4(0.2f, 0.2f, 0.2f, 1);
                        ImVec4 activeColor = ImVec4(0.35f, 0.35f, 0.4f, 1);
                        ImGui::PushStyleColor(ImGuiCol_Button, testBar == (int)i ? activeColor : normalColor);
                        if (ImGui::Button(tabNames[i].c_str()))
                            testBar = static_cast<int>(i);
                        ImGui::PopStyleColor();
                    }
                    ImGui::EndChild();
                    RenderFullWidthSeparator(1.0f, contentSeperatorColor);
                }
                else {
                    ImGui::BeginGroup();
                    for (int i = 0; i < (int)tabNames.size(); ++i) {
                        ImVec4 normalColor = ImVec4(0.2f, 0.2f, 0.2f, 1);
                        ImVec4 activeColor = ImVec4(0.35f, 0.35f, 0.4f, 1);
                        ImGui::PushStyleColor(ImGuiCol_Button, testBar == i ? activeColor : normalColor);
                        if (ImGui::Button(tabNames[i].c_str(), ImVec2(55.f, 0.0f)))
                            testBar = i;
                        ImGui::PopStyleColor();
                    }
                    ImGui::EndGroup();
                    ImGui::SameLine();
                }
            }
        }

        currentTab = testBar;
        ImGui::PushStyleColor(ImGuiCol_Border, ez::contentFrameBorderBg);
        ImGui::BeginChild("##MainFrame", contentSize, true);
        {
            auto& tab = tabs[currentTab];
            ImGui::Columns(2, nullptr, false);
            static std::unordered_map<std::string, int> tabboxStates;

            for (int sideIndex = 0; sideIndex < 2; ++sideIndex) {
                TabboxSide currentSide = (sideIndex == 0) ? TabboxSide::Left : TabboxSide::Right;

                for (auto& tabbox : tab->tabboxes) {
                    if (tabbox->side != currentSide) continue;

                    int& currentIndex = tabboxStates[tabbox->name];
                    int visibleIndex = currentIndex;

                    const std::vector<UIElement>* activeElements = nullptr;
                    if (!tabbox->tabs.empty() && visibleIndex >= 0 && visibleIndex < (int)tabbox->tabs.size())
                        activeElements = &tabbox->tabs[visibleIndex]->elements;
                    else
                        activeElements = &tabbox->elements;

                    int elementCount = static_cast<int>(activeElements->size());

                    float itemHeight = ImGui::GetFrameHeightWithSpacing();
                    float headerHeight = ImGui::GetTextLineHeightWithSpacing();
                    float separatorSpacing = 2 * ImGui::GetStyle().ItemSpacing.y;
                    float tabButtonRowHeight = 0.0f;
                    if (!tabbox->tabs.empty())
                        tabButtonRowHeight = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.y * 2.5f;

                    float tabboxHeight = headerHeight + separatorSpacing + tabButtonRowHeight;

                    for (const auto& e : *activeElements) {

                        // Height estimation uses the per-element heightHint set at add-time for multi-element tabboxes a small correction keeps items from clipping
                        float hint = e.heightHint;
                        float finalMod = 0.0f;
                        if (elementCount > 1 && hint > 1.0f) {
                            finalMod = (float)(elementCount + 1);
                            finalMod += (hint / (float)elementCount) + 1.0f;
                        }
                        tabboxHeight += (itemHeight * hint) - finalMod;
                    }

                    if (elementCount == 0)
                        tabboxHeight += itemHeight * 0.79f;

                    // Tabbox style push
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ez::tbxBackgroundColor);
                    ImGui::PushStyleColor(ImGuiCol_Border, ez::tbxBorderColor);
                    ImGui::PushStyleColor(ImGuiCol_FrameBg, ez::frameBg);
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ez::frameBgActive);
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ez::frameBgHovered);
                    ImGui::PushStyleColor(ImGuiCol_CheckMark, ez::accentColor);
                    ImGui::PushStyleColor(ImGuiCol_SliderGrab, ez::accentColor);
                    ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ez::accentColor);
                    ImGui::PushStyleColor(ImGuiCol_Button, ez::buttonColor);
                    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ez::frameRounding);
                    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, ez::tabboxRounding);

                    ImGui::BeginChild(tabbox->name.c_str(), ImVec2(0, tabboxHeight), true);
                    ImGui::Text("%s", tabbox->name.c_str());
                    RenderFullWidthSeparator(1.0f, tbxBorderColor);

                    float buttonWidth = ImGui::GetContentRegionAvail().x / std::max(1, (int)tabbox->tabs.size());
                    for (int i = 0; i < (int)tabbox->tabs.size(); ++i) {
                        ImGui::PushID(i);
                        ImVec4 normalColor = ImVec4(0.2f, 0.2f, 0.2f, 1);
                        ImVec4 activeColor = ImVec4(0.35f, 0.35f, 0.4f, 1);
                        ImGui::PushStyleColor(ImGuiCol_Button, currentIndex == i ? activeColor : normalColor);
                        if (ImGui::Button((tabbox->tabs[i]->name + "##" + tabbox->name).c_str(),
                            ImVec2(buttonWidth - 4.0f, 0)))
                            currentIndex = i;
                        ImGui::PopStyleColor();
                        if (i < (int)tabbox->tabs.size() - 1)
                            ImGui::SameLine(0.0f, 4.0f);
                        ImGui::PopID();
                    }

                    if (!tabbox->tabs.empty())
                        RenderFullWidthSeparator(1.0f, tbxBorderColor);

                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.f, 0.f));
                    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 0.f));
                    ImGui::BeginChild(("##ContentFrame_" + tabbox->name).c_str(), ImVec2(-1, 0), ImGuiChildFlags_None);

                    if (!tabbox->tabs.empty() && currentIndex >= 0 && currentIndex < (int)tabbox->tabs.size())
                        tabbox->tabs[currentIndex]->Render();
                    else
                        tabbox->RenderExtras();

                    ImGui::EndChild();
                    ImGui::PopStyleColor(2);

                    ImGui::EndChild();
                    ImGui::PopStyleVar(2);
                    ImGui::PopStyleColor(9);
                }

                ImGui::NextColumn();
            }
        }
        ImGui::EndChild();
        ImGui::PopStyleColor(1);

        if (fonts.contains(currentFontName))
            ImGui::PopFont();

        ImGui::PopStyleColor();
        ImGui::End();
    }

}