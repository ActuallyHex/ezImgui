#pragma once
#include <imgui.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>

namespace ez {

    enum class WidgetMode {
        ImGuiDefault,
        FancyWidgets
    };

    enum class CheckboxMode {
        ImGuiDefault,
        ToggleSwitch,
        Anim1,
        Anim2
    };

    inline std::unordered_map<std::string, ImFont*> fonts;
    inline std::string currentFontName = "default";

    void LoadFont(const std::string& name, const char* path, float size);
    void LoadFontFromMemory(const std::string& name, void* data, int size_bytes, float size_pixels);
    void SetFont(const std::string& name);

    enum class TabboxSide {
        Left,
        Right
    };

    enum class ElementType {
        Toggle,
        Slider,
        ColorPicker,
        Label
    };

    struct UIElement {
        ElementType type;
        std::string label;
        union {
            bool* boolValue;
            struct { float* value; float min; float max; };
            ImVec4* colorValue;
        };

        UIElement(const std::string& lbl, bool* val)
            : type(ElementType::Toggle), label(lbl), boolValue(val) {
        }
        UIElement(const std::string& lbl, float* val, float mi, float ma)
            : type(ElementType::Slider), label(lbl) {
            value = val; min = mi; max = ma;
        }
        UIElement(const std::string& lbl, ImVec4* col)
            : type(ElementType::ColorPicker), label(lbl), colorValue(col) {
        }
        UIElement(const std::string& lbl)
            : type(ElementType::Label), label(lbl) {
        }
    };

    struct TabboxTab {
        std::string name;
        std::vector<UIElement> elements;

        void AddCheckbox(const char* label, bool* value);
        void AddSlider(const char* label, float* value, float min, float max);
        void AddColorPicker(const char* label, ImVec4* color);
        void AddLabel(const char* label);
        void Render();
    };

    struct Tabbox {
        std::string name;
        std::vector<std::shared_ptr<TabboxTab>> tabs;
        std::vector<UIElement> elements;  // extras if tabs are empty
        int currentTabIndex = 0;
        TabboxSide side = TabboxSide::Left;

        std::shared_ptr<TabboxTab> AddTab(const char* name);
        void AddCheckbox(const char* label, bool* value);
        void AddSlider(const char* label, float* value, float min, float max);
        void AddColorPicker(const char* label, ImVec4* color);
        void AddLabel(const char* label);
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

        Window(const char* title_, ImVec2 size_, ImGuiWindowFlags flags_, bool autoshow_);
        std::shared_ptr<Tab> AddTab(const char* name);
        void Render();
    };

    std::shared_ptr<Window> CreateEzWindow(const char* title, ImVec2 size, ImGuiWindowFlags flags = ImGuiWindowFlags_None, bool autoshow = true);

    extern WidgetMode g_WidgetMode;
}

