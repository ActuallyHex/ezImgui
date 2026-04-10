#pragma once
#include <imgui.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <functional>

//#define USE_EZ_WIDGETS

#ifdef USE_EZ_WIDGETS
#include "../ezWidgets/ezWidgets.h"
#endif

namespace ez {

    // Style enums are kept so Add* methods can accept them as parameters but they are resolved immediately at add-time and never stored on UIElement
    enum class CheckboxStyle {
        ImGuiDefault,
        ToggleSwitch,
        Anim1,
        Anim2
    };

    enum class ComboBoxStyle {
        ImGuiDefault,
        Style1,
        Style2
    };

    enum class SliderStyle {
        ImGuiDefault,
        Style1,
        Style2
    };

    enum class ButtonStyle {
        ImGuiDefault,
        Style1
    };

    enum class TabboxSide {
        Left,
        Right
    };

    // ElementType is kept exclusively for tabbox height estimation
    enum class ElementType {
        Toggle,
        SliderFloat,
        SliderInt,
        ColorPicker,
        Label,
        ComboBox,
        MultiComboBox,
        ToggleColorPicker,
        LabelColorPicker,
        Button,
        GradientButton
    };

    enum class TabMode {
        ImGuiTabs,
        ButtonTabs
    };

    enum class TabButtonOrientation {
        HorizontalTop,
        VerticalLeft
    };

    struct Notification {
        std::string text;
        float lifetime = 3.0f;
        float age = 0.0f;
        float slide = 0.0f;
        float alpha = 1.0f;
    };

    inline std::unordered_map<std::string, ImFont*> fonts;
    inline std::vector<Notification> g_Notifications;
    inline std::string currentFontName = "default";

    inline ImVec4 tbxBackgroundColor = ImVec4(0.102f, 0.103f, 0.103f, 1.00f);
    inline ImVec4 winBackgroundColor = ImVec4(0.069f, 0.069f, 0.069f, 1.00f);
    inline ImVec4 tbxBorderColor = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    inline ImVec4 frameBg = ImVec4(0.16f, 0.29f, 0.48f, 0.54f);
    inline ImVec4 frameBgHovered = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    inline ImVec4 frameBgActive = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    inline ImVec4 accentColor = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    inline ImVec4 buttonColor = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    inline ImVec4 contentSeperatorColor = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    inline ImVec4 contentFrameBorderBg = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    inline int tabboxRounding = 0;
    inline int frameRounding = 0;

    inline TabMode g_TabMode = TabMode::ImGuiTabs;

    inline float minTabboxHeight = 100.0f;
    inline float maxTabboxHeight = 400.0f;
    inline float elementHeight = 26.0f;

    void LoadFont(const std::string& name, const char* path, float size);
    void LoadFontFromMemory(const std::string& name, void* data, int size_bytes, float size_pixels);
    void SetFont(const std::string& name);
    void RenderFullWidthSeparator(float thickness, ImVec4 color);
    bool ImGuiMultiComboBox(const char* label, std::unordered_map<int, bool>* data, std::vector<const char*> items);
    void PushNotification(const std::string& text, float duration = 3.0f);
    void RenderNotifications();

    // -------------------------------------------------------------------------
    // AddTextWithOutline
    //
    // Draws text with a solid outline in a single merged pass
    // Outline geometry is expanded from each glyph quad and emitted into the draw list before the fill quads
    // No shader changes/texture lookups beyond the normal font atlas
    //
    // Parameters:
    //   drawList          - target draw list (e.g. ImGui::GetWindowDrawList())
    //   font              - font to use; pass nullptr for ImGui's current font
    //   fontSize          - size in pixels; pass 0.0f to use font's default size
    //   pos               - top-left origin of the text
    //   fillColor         - RGBA color of the text fill (ImU32)
    //   outlineColor      - RGBA color of the outline (ImU32)
    //   text              - null-terminated UTF-8 string
    //   outlineThickness  - outline width in pixels (default 1.0f, fractional ok)
    //   wrapWidth         - pixel width at which text wraps; 0.0f = no wrap
    //   cpuFineAA         - when true, blends outline alpha at sub-pixel thickness for a slightly softer edge at thickness < 1.5
    // 
    // -------------------------------------------------------------------------
    void AddTextWithOutline(ImDrawList* drawList,
        ImFont* font,
        float        fontSize,
        ImVec2       pos,
        ImU32        fillColor,
        ImU32        outlineColor,
        const char* text,
        float        outlineThickness = 1.0f,
        float        wrapWidth = 0.0f,
        bool         cpuFineAA = true);

    // -------------------------------------------------------------------------
    // UIElement
    //
    // Each element stores:
    //   renderFn   - a zero-argument callable that draws the widget
    //   type       - used only by Window::Render for tabbox height estimation
    //   heightHint - multiplier applied on top of the base frame height when estimating tabbox size 
    //                set by Add* methods so that styles can declare it needs more vertical space than ImGuiDefault without the render path needing to know
    //
    // -------------------------------------------------------------------------
    struct UIElement {
        ElementType           type;
        std::function<void()> renderFn;
        float                 heightHint = 1.0f; // multiplier for height estimation

        // ComboBox items need to stay alive alongside the element so we keep owned storage here (same approach as before)
        std::vector<std::string>   comboItemStorage;
        std::vector<const char*>   comboItemPointers;
        std::shared_ptr<std::unordered_map<int, bool>> multiComboData;
        std::vector<const char*>   multiComboRawItems;
    };

    // -------------------------------------------------------------------------
    // Forward declarations
    // -------------------------------------------------------------------------
    struct TabboxTab {
        std::string name;
        std::vector<UIElement> elements;

        // Checkboxes
        void AddCheckbox(const char* label, bool* value, CheckboxStyle style = CheckboxStyle::ImGuiDefault);
        void AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color, CheckboxStyle style = CheckboxStyle::ImGuiDefault);

        // Sliders
        void AddSlider(const char* label, float* value, float min, float max, SliderStyle style = SliderStyle::ImGuiDefault);
        void AddSlider(const char* label, int* value, int   min, int   max, SliderStyle style = SliderStyle::ImGuiDefault);

        // Misc
        void AddColorPicker(const char* label, ImVec4* color);
        void AddLabel(const char* label);
        void AddMultiComboBox(const char* label, std::initializer_list<const char*> items, std::shared_ptr<std::unordered_map<int, bool>> data);
        void AddButton(const char* label, std::function<void()> onClick = nullptr, ButtonStyle style = ButtonStyle::ImGuiDefault);
        void AddGradientButton(const char* label, ImU32 textClr, ImU32 bgClr1, ImU32 bgClr2, std::function<void()> onClick = nullptr);
        void AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items = -1, ComboBoxStyle style = ComboBoxStyle::ImGuiDefault);
        void AddTextWithOutline(const char* text, ImU32 fillColor, ImU32 outlineColor, float outlineThickness = 1.0f, ImFont* font = nullptr, float fontSize = 0.0f, bool cpuFineAA = true);

        void Render();
    };

    struct Tabbox {
        std::string name;
        std::vector<std::shared_ptr<TabboxTab>> tabs;
        std::vector<UIElement> elements;   // used when tabs vector is empty
        int currentTabIndex = 0;
        TabboxSide side = TabboxSide::Left;

        std::shared_ptr<TabboxTab> AddTab(const char* name);

        // Checkboxes
        void AddCheckbox(const char* label, bool* value, CheckboxStyle style = CheckboxStyle::ImGuiDefault);
        void AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color, CheckboxStyle style = CheckboxStyle::ImGuiDefault);

        // Sliders
        void AddSlider(const char* label, float* value, float min, float max, SliderStyle style = SliderStyle::ImGuiDefault);
        void AddSlider(const char* label, int* value, int   min, int   max, SliderStyle style = SliderStyle::ImGuiDefault);

        // Misc
        void AddColorPicker(const char* label, ImVec4* color);
        void AddLabel(const char* label);
        void AddMultiComboBox(const char* label, std::initializer_list<const char*> items, std::shared_ptr<std::unordered_map<int, bool>> data);
        void AddButton(const char* label, std::function<void()> onClick = nullptr, ButtonStyle style = ButtonStyle::ImGuiDefault);
        void AddGradientButton(const char* label, ImU32 textClr, ImU32 bgClr1, ImU32 bgClr2, std::function<void()> onClick = nullptr);
        void AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items = -1, ComboBoxStyle style = ComboBoxStyle::ImGuiDefault);
        void AddTextWithOutline(const char* text, ImU32 fillColor, ImU32 outlineColor, float outlineThickness = 1.0f, ImFont* font = nullptr, float fontSize = 0.0f, bool cpuFineAA = true);

        void RenderExtras();
    };

    struct Tab {
        std::string name;
        std::vector<std::shared_ptr<Tabbox>> tabboxes;
        std::shared_ptr<Tabbox> AddTabbox(const char* name, TabboxSide side = TabboxSide::Left);
    };

    struct Window {
        std::string title;
        std::vector<std::string> tabNames;
        std::vector<std::shared_ptr<Tab>> tabs;
        int currentTab = 0;
        bool center = true;
        bool autoshow = true;
        bool isOpen = true;
        ImVec2 size;
        ImGuiWindowFlags flags;
        ImGuiStyle& style = ImGui::GetStyle();
        TabMode tabMode = TabMode::ImGuiTabs;
        TabButtonOrientation tabButtonOrientation = TabButtonOrientation::HorizontalTop;

        Window(const char* title_, ImVec2 size_, ImGuiWindowFlags flags_, bool autoshow_);
        std::shared_ptr<Tab> AddTab(const char* name);
        void Render();
    };

    std::shared_ptr<Window> CreateEzWindow(const char* title, ImVec2 size, ImGuiWindowFlags flags = ImGuiWindowFlags_None, bool autoshow = true);

}