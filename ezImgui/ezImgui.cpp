#include "ezImgui.h"
#include <imgui_internal.h>

#ifdef USE_EZ_WIDGETS
#include "../ezWidgets/ezWidgets.h"
#endif

// W.I.P List
// 1. replace shared_ptr with vector<T> or unique_ptr
//    vector<T> might improve performance?
//    as it is right now the wrapper isnt very performance or thread safe
// 2. (maybe) Add way to pushfont tabboxes and its contents
// 3. More drawlist features (animations, styles, etc.)

namespace ez {

    //=================================================================================================
    // Globals
    //=================================================================================================
    static CheckboxStyle g_CheckboxMode = CheckboxStyle::ImGuiDefault;

    //=================================================================================================
    // Function Definitions
    //=================================================================================================
    #ifdef USE_EZ_WIDGETS

    //=== TabboxTab methods ===
    void TabboxTab::AddCheckbox(const char* label, bool* value, CheckboxStyle style) {
        elements.emplace_back(label, value, style);
    }

    void TabboxTab::AddCheckbox(const char* label, bool* value) {
        elements.emplace_back(label, value, CheckboxStyle::ImGuiDefault);
    }

    void TabboxTab::AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items, ComboBoxStyle style) {
        elements.emplace_back(label, current_item, items, height_in_items, style);
    }

    void TabboxTab::AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items) {
        elements.emplace_back(label, current_item, items, height_in_items, ComboBoxStyle::ImGuiDefault);
    }

    void TabboxTab::AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color, CheckboxStyle style) {
        elements.emplace_back(label, value, color, style);
    }

    void TabboxTab::AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color) {
        elements.emplace_back(label, value, color, CheckboxStyle::ImGuiDefault);
    }

    void TabboxTab::AddGradientButton(const char* label, ImU32 textClr, ImU32 bgClr1, ImU32 bgClr2, std::function<void()> onClick) {
        elements.emplace_back(label, UIElement::ButtonTag{}, textClr, bgClr1, bgClr2, onClick);
    }

    void TabboxTab::AddSlider(const char* label, float* value, float min, float max, SliderStyle style) {
        elements.emplace_back(label, value, min, max, style);
    }

    void TabboxTab::AddSlider(const char* label, int* value, int min, int max, SliderStyle style) {
        elements.emplace_back(label, value, min, max, style);
    }

    //=== Tabbox methods ===
    void Tabbox::AddCheckbox(const char* label, bool* value) {
        elements.emplace_back(label, value, CheckboxStyle::ImGuiDefault);
    }

    void Tabbox::AddCheckbox(const char* label, bool* value, CheckboxStyle style) {
        elements.emplace_back(label, value, style);
    }

    void Tabbox::AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items, ComboBoxStyle style) {
        elements.emplace_back(label, current_item, items, height_in_items, style);
    }

    void Tabbox::AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items) {
        elements.emplace_back(label, current_item, items, height_in_items, ComboBoxStyle::ImGuiDefault);
    }

    void Tabbox::AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color, CheckboxStyle style) {
        elements.emplace_back(label, value, color, style);
    }

    void Tabbox::AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color) {
        elements.emplace_back(label, value, color, CheckboxStyle::ImGuiDefault);
    }

    void Tabbox::AddGradientButton(const char* label, ImU32 textClr, ImU32 bgClr1, ImU32 bgClr2, std::function<void()> onClick) {
        elements.emplace_back(label, UIElement::ButtonTag{}, textClr, bgClr1, bgClr2, onClick);
    }

    void Tabbox::AddSlider(const char* label, float* value, float min, float max, SliderStyle style) {
        elements.emplace_back(label, value, min, max, style);
    }

    void Tabbox::AddSlider(const char* label, int* value, int min, int max, SliderStyle style) {
        elements.emplace_back(label, value, min, max, style);
    }

    #else

    //=== TabboxTab methods ===
    void TabboxTab::AddCheckbox(const char* label, bool* value) {
        elements.emplace_back(label, value);
    }

    void TabboxTab::AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color) {
        elements.emplace_back(label, value, color);
    }

    void TabboxTab::AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items) {
        elements.emplace_back(label, current_item, items, height_in_items);
    }

    //=== Tabbox methods ===
    void Tabbox::AddCheckbox(const char* label, bool* value) {
        elements.emplace_back(label, value);
    }

    void Tabbox::AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color) {
        elements.emplace_back(label, value, color);
    }

    void Tabbox::AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items) {
        elements.emplace_back(label, current_item, items, height_in_items);
    }

    #endif // USE_EZ_WIDGETS

    //=== Shared (common to both configs) ===

    // TabboxTab shared
    void TabboxTab::AddSlider(const char* label, float* value, float min, float max) {
        elements.emplace_back(label, value, min, max);
    }

    void TabboxTab::AddSlider(const char* label, int* value, int min, int max) {
        elements.emplace_back(label, value, min, max);
    }

    void TabboxTab::AddButton(const char* label, std::function<void()> onClick, ButtonStyle style) {
        #ifdef USE_EZ_WIDGETS
            elements.emplace_back(label, UIElement::ButtonTag{}, onClick, style);
        #else
            elements.emplace_back(label, UIElement::ButtonTag{}, onClick);
        #endif
    }

    void TabboxTab::AddColorPicker(const char* label, ImVec4* color) {
        elements.emplace_back(label, color);
    }

    void TabboxTab::AddLabel(const char* label) {
        elements.emplace_back(label);
    }

    void TabboxTab::AddMultiComboBox(const char* label, std::initializer_list<const char*> items, std::shared_ptr<std::unordered_map<int, bool>> data) {
        elements.emplace_back(label, items, data);
    }

    // Tabbox shared
    void Tabbox::AddSlider(const char* label, float* value, float min, float max) {
        elements.emplace_back(label, value, min, max);
    }

    void Tabbox::AddSlider(const char* label, int* value, int min, int max) {
        elements.emplace_back(label, value, min, max);
    }

    void Tabbox::AddButton(const char* label, std::function<void()> onClick, ButtonStyle style) {
        #ifdef USE_EZ_WIDGETS
            elements.emplace_back(label, UIElement::ButtonTag{}, onClick, style);
        #else
            elements.emplace_back(label, UIElement::ButtonTag{}, onClick);
        #endif
    }

    void Tabbox::AddColorPicker(const char* label, ImVec4* color) {
        elements.emplace_back(label, color);
    }

    void Tabbox::AddLabel(const char* label) {
        elements.emplace_back(label);
    }

    void Tabbox::AddMultiComboBox(const char* label, std::initializer_list<const char*> items, std::shared_ptr<std::unordered_map<int, bool>> data) {
        elements.emplace_back(label, items, data);
    }

    //=================================================================================================
    // Notifications
    //=================================================================================================
    void PushNotification(const std::string& text, float duration) {
        g_Notifications.push_back(Notification{ text, duration, 0.0f, -200.0f, 1.0f });
    }

    void ez::RenderNotifications() {
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();
        const float padding = 10.0f;
        const float line_height = 20.0f;
        const float box_rounding = 0.0f;

        int index = 0;
        for (int i = 0; i < g_Notifications.size(); ) {
            Notification& n = g_Notifications[i];
            n.age += io.DeltaTime;

            // Fade out in the last 0.2 seconds
            if (n.lifetime - n.age < 0.5f)
                n.alpha = ImClamp((n.lifetime - n.age) / 0.2f, 0.0f, 1.0f);

            // Slide-in animation
            if (n.slide < 0.0f) {
                n.slide = ImLerp(n.slide, 0.0f, ImClamp(io.DeltaTime * 10.0f, 0.0f, 1.0f));
                if (n.slide > -1.0f) n.slide = 0.0f;  // snap to 0 when almost finished
            }
            if (n.slide > 0.0f)
                n.slide = 0.0f;

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
            draw_list->AddRect(ImVec2(rect_outline_min.x - 0.4f, rect_outline_min.y - 0.4f), ImVec2(rect_outline_max.x + 0.4f, rect_outline_max.y + 0.4f), bg_border_col, box_rounding);
            draw_list->AddRect(rect_outline_min, rect_outline_max, border_col, box_rounding);

            draw_list->AddText(ImVec2(pos.x + padding, pos.y + padding), text_col, n.text.c_str());

            if (n.age >= n.lifetime) {
                g_Notifications.erase(g_Notifications.begin() + i);
            }
            else {
                ++i;
                ++index;
            }
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
    // UI Rendering
    //=================================================================================================
    void RenderUIElements(const UIElement& e)
    {
        switch (e.type) {
        case ElementType::Toggle:
        {
            #ifdef USE_EZ_WIDGETS

            switch (e.styleValue)
            {
            case CheckboxStyle::ToggleSwitch:
                ezWidgets::ToggleSwitch(e.label.c_str(), e.boolValue);
                break;
            case CheckboxStyle::Anim1:
                ezWidgets::CheckboxAnim1(e.label.c_str(), e.boolValue);
                break;
            case CheckboxStyle::Anim2:
                ezWidgets::CheckboxAnim2(e.label.c_str(), e.boolValue);
                break;
            case CheckboxStyle::ImGuiDefault:
            default:
                ImGui::Checkbox(e.label.c_str(), e.boolValue);
                break;
            }

            #else
            ImGui::Checkbox(e.label.c_str(), e.boolValue);
            #endif

            break;
        }
        case ElementType::SliderFloat:
            #ifdef USE_EZ_WIDGETS
            switch (e.sliderStyle)
            {
            case SliderStyle::Style1:
                ezWidgets::SliderStyle1(e.label.c_str(), e.valueFloat, e.minFloat, e.maxFloat, "%.0f", 0);
                break;
            case SliderStyle::ImGuiDefault:
            default:
                ImGui::SliderFloat(e.label.c_str(), e.valueFloat, e.minFloat, e.maxFloat);
                break;
            }
            #else
            ImGui::SliderFloat(e.label.c_str(), e.valueFloat, e.minFloat, e.maxFloat);
            #endif

            break;
        case ElementType::SliderInt:
            #ifdef USE_EZ_WIDGETS 
            switch (e.sliderStyle)
            {
            case SliderStyle::Style1:
                ezWidgets::SliderStyle1(e.label.c_str(), e.valueInt, e.minInt, e.maxInt, "%d", 0);
                ImGui::Dummy(ImVec2(0.0f, 4.f));
                break;
            case SliderStyle::ImGuiDefault:
            default:
                ImGui::SliderInt(e.label.c_str(), e.valueInt, e.minInt, e.maxInt);
                break;
            }
            #else
            ImGui::SliderInt(e.label.c_str(), e.valueInt, e.minInt, e.maxInt);
            #endif

            break;
        case ElementType::ColorPicker:
            ImGui::ColorEdit4(e.label.c_str(), (float*)e.colorValue, ImGuiColorEditFlags_NoInputs);
            break;
        case ElementType::ToggleColorPicker:
        {
            #ifdef USE_EZ_WIDGETS
            switch (e.styleValue)
            {
            case CheckboxStyle::ToggleSwitch:
                ezWidgets::ToggleSwitch(e.label.c_str(), e.boolValue);
                break;
            case CheckboxStyle::Anim1:
                ezWidgets::CheckboxAnim1(e.label.c_str(), e.boolValue);
                break;
            case CheckboxStyle::Anim2:
                ezWidgets::CheckboxAnim2(e.label.c_str(), e.boolValue);
                break;
            case CheckboxStyle::ImGuiDefault:
            default:
                ImGui::Checkbox(e.label.c_str(), e.boolValue);
                break;
            }
            #else
            ImGui::Checkbox(e.label.c_str(), e.boolVal);
            #endif

            float fullWidth = ImGui::GetContentRegionAvail().x;
            float colorPickerWidth = 25.0f; // approximate width of color picker (can tweak as needed)

            ImGui::SameLine(ImGui::GetCursorPosX() + fullWidth - colorPickerWidth);
            ImGui::ColorEdit4(("##" + e.label).c_str(), (float*)e.clrVal, ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel);
            break;
        }
        case ElementType::MultiComboBox:
            ImGuiMultiComboBox(e.label.c_str(), e.multiComboData.get(), e.multiComboRawItems);
            break;
        case ElementType::Label:
            ImGui::Text("%s", e.label.c_str());
            break;
        case ElementType::Button:

            #ifdef USE_EZ_WIDGETS
            switch (e.buttonStyle)
            {
            case ButtonStyle::Style1:
                ezWidgets::IconButton1(e.label.c_str(), ImVec2(-FLT_MIN, 0.0f), false);
                break;
            case ButtonStyle::ImGuiDefault:
            default:
                if (ImGui::Button(e.label.c_str(), ImVec2(-FLT_MIN, 0.0f)) && e.buttonCallback)
                    e.buttonCallback();
                break;
            }

            #else
            if (ImGui::Button(e.label.c_str(), ImVec2(-FLT_MIN, 0.0f)) && e.buttonCallback)
                e.buttonCallback();
            #endif

            break;
        case ElementType::GradientButton:
        {
            #ifdef USE_EZ_WIDGETS
            if (ezWidgets::ColoredButtonV1(e.label.c_str(), ImVec2(-FLT_MIN, 0.0f), e.text_color, e.bg_color_1, e.bg_color_2) && e.buttonCallback)
                e.buttonCallback();
            break;

            #else
            break;
            #endif
        }
        case ElementType::ComboBox:
            #ifdef USE_EZ_WIDGETS

            switch (e.comboStyle)
            {
            case ComboBoxStyle::Style1:
                ezWidgets::comboBoxHelper1(e.label.c_str(), e.comboData.currentItem, e.comboData.items, e.comboData.itemsCount, e.comboData.heightInItems);
                break;
            case ComboBoxStyle::ImGuiDefault:
            default:
                const char* preview_value = nullptr;
                if (e.comboData.currentItem && *e.comboData.currentItem >= 0 && *e.comboData.currentItem < e.comboData.itemsCount)
                    preview_value = e.comboData.items[*e.comboData.currentItem];

                if (ImGui::BeginCombo(e.label.c_str(), preview_value, 0))
                {
                    for (int n = 0; n < e.comboData.itemsCount; n++)
                    {
                        const bool is_selected = (*e.comboData.currentItem == n);
                        if (ImGui::Selectable(e.comboData.items[n], is_selected))
                            *e.comboData.currentItem = n;

                        if (is_selected)
                            ImGui::SetItemDefaultFocus();
                    }
                    ImGui::EndCombo();
                }
                break;
            }

            #else
            const char* preview_value = nullptr;
            if (e.comboData.currentItem && *e.comboData.currentItem >= 0 && *e.comboData.currentItem < e.comboData.itemsCount)
                preview_value = e.comboData.items[*e.comboData.currentItem];

            if (ImGui::BeginCombo(e.label.c_str(), preview_value, 0))
            {
                for (int n = 0; n < e.comboData.itemsCount; n++)
                {
                    const bool is_selected = (*e.comboData.currentItem == n);
                    if (ImGui::Selectable(e.comboData.items[n], is_selected))
                        *e.comboData.currentItem = n;

                    if (is_selected)
                        ImGui::SetItemDefaultFocus();
                }
                ImGui::EndCombo();
            }
            #endif

            break;
        }
    }

    //=================================================================================================
    // Window and Tabs
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
    // TabboxTab Controls
    //=================================================================================================
    void TabboxTab::Render() {
        for (auto& e : elements)
            RenderUIElements(e);
    }

    //=================================================================================================
    // Tabbox Controls
    //=================================================================================================
    void Tabbox::RenderExtras() {
        for (auto& e : elements)
            RenderUIElements(e);
    }

    void Window::Render() {

        if (center) { // start menu centered
            ImVec2 centerPos = ImGui::GetMainViewport()->GetCenter();
            auto tempSize = ImVec2(size.x * 0.5f, size.y * 0.5f);
            centerPos.x = centerPos.x - tempSize.x;
            centerPos.y = centerPos.y - tempSize.y;
            ImGui::SetNextWindowPos(centerPos, ImGuiCond_FirstUseEver);
        }

        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ez::winBackgroundColor);
        ImGui::Begin(title.c_str(), &isOpen, flags);
        static int testBar = 0;
        ImVec2 contentSize;

        //ImGuiWindow* window = ImGui::GetCurrentWindow();
        //ImVec2 windowPos = window->Pos;
        //ImVec2 windowSize = window->Size;

        //float rectHeight = 20.0f;  // Height of the top bar
        //ImU32 color = IM_COL32(255, 0, 0, 255);  // Solid red for example

        //window->DrawList->AddRectFilled(
        //    windowPos,
        //    ImVec2(windowPos.x + windowSize.x, windowPos.y + rectHeight),
        //    color
        //);


        if (fonts.contains(currentFontName))
            ImGui::PushFont(fonts[currentFontName]);

        ImGui::Spacing();

        if (!tabNames.empty())
        {
            if (tabMode == TabMode::ImGuiTabs)
            {
                contentSize = ImVec2(size.x - 15, size.y - 66);

                if (ImGui::BeginTabBar("##tabs")) {
                    for (int i = 0; i < tabNames.size(); i++) {
                        if (ImGui::BeginTabItem(tabNames[i].c_str())) {
                            testBar = static_cast<int>(i);
                            ImGui::EndTabItem();
                        }
                    }
                    ImGui::EndTabBar();
                }
            }
            else if (tabMode == TabMode::ButtonTabs)
            {
                float buttonHeight = 30.0f;

                if (tabButtonOrientation == TabButtonOrientation::HorizontalTop)
                {
                    ImGui::BeginChild("##ButtonTabBar", ImVec2(0, buttonHeight), false);
                    for (size_t i = 0; i < tabNames.size(); i++) {
                        if (i > 0) ImGui::SameLine();

                        ImVec4 normalColor = ImVec4(0.2f, 0.2f, 0.2f, 1);
                        ImVec4 activeColor = ImVec4(0.35f, 0.35f, 0.4f, 1);
                        ImGui::PushStyleColor(ImGuiCol_Button, testBar == i ? activeColor : normalColor);

                        if (ImGui::Button(tabNames[i].c_str())) {
                            testBar = static_cast<int>(i);
                        }

                        ImGui::PopStyleColor();
                    }
                    ImGui::EndChild();

                    RenderFullWidthSeparator(1.0f, contentSeperatorColor);
                }
                else
                {
                    ImGui::BeginGroup();
                    for (int i = 0; i < tabNames.size(); ++i) 
                    {
                        ImVec4 normalColor = ImVec4(0.2f, 0.2f, 0.2f, 1);
                        ImVec4 activeColor = ImVec4(0.35f, 0.35f, 0.4f, 1);
                        ImGui::PushStyleColor(ImGuiCol_Button, testBar == i ? activeColor : normalColor);

                        if (ImGui::Button(tabNames[i].c_str(), ImVec2(55.f, 0.0f))) {
                            testBar = static_cast<int>(i);
                        }

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
                    if (tabbox->side == currentSide) {

                        int& currentIndex = tabboxStates[tabbox->name];
                        int visibleIndex = currentIndex;
                        int elementCount = 0;
                        if (!tabbox->tabs.empty() && visibleIndex >= 0 && visibleIndex < tabbox->tabs.size()) {
                            elementCount = static_cast<int>(tabbox->tabs[visibleIndex]->elements.size());
                        }
                        else 
                        {
                            elementCount = static_cast<int>(tabbox->elements.size());
                        }

                        float itemHeight = ImGui::GetFrameHeightWithSpacing();      // standard height per item
                        float headerHeight = ImGui::GetTextLineHeightWithSpacing(); // for the tabbox label

                        float separatorSpacing = 2 * ImGui::GetStyle().ItemSpacing.y; // 2 separators (top + below tabs)
                        float tabButtonRowHeight = 0.0f;

                        if (!tabbox->tabs.empty()) 
                        {
                            tabButtonRowHeight = ImGui::GetFrameHeight() + ImGui::GetStyle().ItemSpacing.y * 2.5f;
                        }

                        float tabboxHeight = headerHeight + separatorSpacing + tabButtonRowHeight;

                        for (const auto& e : (!tabbox->tabs.empty() && visibleIndex >= 0 && visibleIndex < tabbox->tabs.size())
                            ? tabbox->tabs[visibleIndex]->elements
                            : tabbox->elements)
                        {
                            switch (e.type)
                            {
                            case ElementType::MultiComboBox:
                            case ElementType::ComboBox:
                            {
                                float comboHeight = 1.35;
                                float finalMod = 0.0f;

                                if (elementCount > 1)
                                {
                                    finalMod = (elementCount + 1);
                                    finalMod += (comboHeight / elementCount) + 1;
                                }

                                tabboxHeight += (itemHeight * comboHeight) - finalMod;
                                break;
                            }
                            case ElementType::ColorPicker:
                            {
                                float clrHeight = 1.42;
                                float finalMod = 0.0f;

                                if (elementCount > 1)
                                {
                                    finalMod = (elementCount + 1);
                                    finalMod += (clrHeight / elementCount) + 1;
                                }

                                tabboxHeight += (itemHeight * clrHeight) - finalMod;
                                break;
                            }
                            case ElementType::ToggleColorPicker:
                            case ElementType::Toggle:
                            {
                                float toggleHeight = 1.35;
                                float finalMod = 0.0f;

                                if (elementCount > 1)
                                {
                                    finalMod = (elementCount + 1);
                                    finalMod += (toggleHeight / elementCount) + 1;               // Is this the best way to get total element height? No absolutely not... but it works.
                                }

                                tabboxHeight += (itemHeight * toggleHeight) - finalMod;

                                break;
                            }
                            case ElementType::Button:
                                tabboxHeight += itemHeight * 1.20f;
                                break;
                            case ElementType::SliderInt:
                            case ElementType::SliderFloat:
                            {
                                if(e.sliderStyle == ez::SliderStyle::Style1)
                                    tabboxHeight += itemHeight * 1.90f;
                                else
                                    tabboxHeight += itemHeight * 0.20f;
                            }

                            default:
                                tabboxHeight += itemHeight;
                                break;
                            }
                        }

                        if (elementCount == 0)
                            tabboxHeight += itemHeight * 0.79f;

                        // [Tabbox Content Colors]
                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ez::tbxBackgroundColor);
                        ImGui::PushStyleColor(ImGuiCol_Border, ez::tbxBorderColor);
                        ImGui::PushStyleColor(ImGuiCol_FrameBg, ez::frameBg);
                        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ez::frameBgActive);
                        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ez::frameBgHovered);
                        ImGui::PushStyleColor(ImGuiCol_CheckMark, ez::accentColor);
                        ImGui::PushStyleColor(ImGuiCol_SliderGrab, ez::accentColor);
                        ImGui::PushStyleColor(ImGuiCol_SliderGrabActive, ez::accentColor);
                        ImGui::PushStyleColor(ImGuiCol_Button, ez::buttonColor);
                        
                        // [Tabbox Content Styles]
                        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, ez::frameRounding);
                        ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, ez::tabboxRounding);
                        ImGui::BeginChild(tabbox->name.c_str(), ImVec2(0, tabboxHeight), true);
                        ImGui::Text("%s", tabbox->name.c_str());

                        RenderFullWidthSeparator(1.0f, tbxBorderColor);

                        float buttonWidth = ImGui::GetContentRegionAvail().x / std::max(1, (int)tabbox->tabs.size());
                        for (int i = 0; i < tabbox->tabs.size(); ++i) {
                            ImGui::PushID(i);

                            ImVec4 normalColor = ImVec4(0.2f, 0.2f, 0.2f, 1);
                            ImVec4 activeColor = ImVec4(0.35f, 0.35f, 0.4f, 1);
                            ImGui::PushStyleColor(ImGuiCol_Button, currentIndex == i ? activeColor : normalColor);
                           

                            bool isSelected = (currentIndex == i);
                            if (ImGui::Button((tabbox->tabs[i]->name + "##" + tabbox->name).c_str(), ImVec2(buttonWidth - 4.0f, 0))) {
                                currentIndex = i;
                            }

                            ImGui::PopStyleColor();

                            if (i < tabbox->tabs.size() - 1)
                                ImGui::SameLine(0.0f, 4.0f);

                            ImGui::PopID();
                        }

                        if (!tabbox->tabs.empty())
                            RenderFullWidthSeparator(1.0f, tbxBorderColor);

                        ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(0.f, 0.f, 0.f, 0.f));
                        ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.f, 0.f, 0.f, 0.f));
                        if (!tabbox->tabs.empty() && currentIndex >= 0 && currentIndex < tabbox->tabs.size()) {
                            ImGui::BeginChild(("##ContentFrame_" + tabbox->name).c_str(), ImVec2(-1, 0), ImGuiChildFlags_None);
                            tabbox->tabs[currentIndex]->Render();
                            ImGui::EndChild();
                        }
                        else {
                            ImGui::BeginChild(("##ContentFrame_" + tabbox->name).c_str(), ImVec2(-1, 0), ImGuiChildFlags_None);
                            tabbox->RenderExtras();
                            ImGui::EndChild();
                        }
                        ImGui::PopStyleColor(2);

                        ImGui::EndChild();
                        ImGui::PopStyleVar(2);
                        ImGui::PopStyleColor(9);
                    }
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