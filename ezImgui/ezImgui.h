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

    inline std::unordered_map<std::string, ImFont*> fonts;
    inline std::string currentFontName = "default";
    inline ImVec4 tbxBackgroundColor = ImVec4(0.102f, 0.103f, 0.103f, 1.00f);
    inline ImVec4 winBackgroundColor = ImVec4(0.069f, 0.069f, 0.069f, 1.00f);
    inline ImVec4 tbxBorderColor = ImVec4(0.275f, 0.275f, 0.275f, 1.00f);

    inline WidgetMode g_WidgetMode = WidgetMode::FancyWidgets;

    inline float minTabboxHeight = 100.0f;
    inline float maxTabboxHeight = 400.0f;
    inline float elementHeight = 26.0f;

    void LoadFont(const std::string& name, const char* path, float size);
    void LoadFontFromMemory(const std::string& name, void* data, int size_bytes, float size_pixels);
    void SetFont(const std::string& name);
    void RenderFullWidthSeparator(float thickness = 1.0f, ImU32 color = 0xFF444444);

    enum class TabboxSide {
        Left,
        Right
    };

    enum class ElementType {
        Toggle,
        Slider,
        ColorPicker,
        Label,
        ComboBox
    };

    struct UIElement {
        ElementType type;
        CheckboxStyle styleValue;
        ComboBoxStyle comboStyle;
        std::string label;
        std::vector<std::string> comboItemsStorage;
        union {
            bool* boolValue;
            struct { float* value; float min; float max; };
            ImVec4* colorValue;
            struct { int* currentItem; const char* const* items; int itemsCount; int heightInItems; } comboData;
        };

        UIElement(const std::string& lbl, bool* val)
            : type(ElementType::Toggle), label(lbl), boolValue(val) {
        }
        UIElement(const std::string& lbl, bool* val, CheckboxStyle style = CheckboxStyle::ImGuiDefault)
            : type(ElementType::Toggle), label(lbl), boolValue(val), styleValue(style) {
        }
        UIElement(const std::string& lbl, int* curval, std::initializer_list<const char*> itemsInitList, int height)
            : type(ElementType::ComboBox), label(lbl) {
            comboItemsStorage.assign(itemsInitList.begin(), itemsInitList.end());
            comboData.currentItem = curval;
            comboData.itemsCount = static_cast<int>(comboItemsStorage.size());
            comboData.heightInItems = height;

            static std::vector<const char*> tempPointers;
            tempPointers.clear();
            for (const auto& s : comboItemsStorage)
                tempPointers.push_back(s.c_str());
            comboData.items = tempPointers.data();
        }
        UIElement(const std::string& lbl, int* curval, std::initializer_list<const char*> itemsInitList, int height, ComboBoxStyle style)
            : type(ElementType::ComboBox), label(lbl), comboStyle(style) {
            comboItemsStorage.assign(itemsInitList.begin(), itemsInitList.end());
            comboData.currentItem = curval;
            comboData.itemsCount = static_cast<int>(comboItemsStorage.size());
            comboData.heightInItems = height;

            static std::vector<const char*> tempPointers;
            tempPointers.clear();
            for (const auto& s : comboItemsStorage)
                tempPointers.push_back(s.c_str());
            comboData.items = tempPointers.data();
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
        void AddCheckbox(const char* label, bool* value, CheckboxStyle style);
        void AddSlider(const char* label, float* value, float min, float max);
        void AddColorPicker(const char* label, ImVec4* color);
        void AddLabel(const char* label);
        void AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items = -1);
        void AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items = -1, ComboBoxStyle style = ComboBoxStyle::ImGuiDefault);
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
        void AddCheckbox(const char* label, bool* value, CheckboxStyle style);
        void AddSlider(const char* label, float* value, float min, float max);
        void AddColorPicker(const char* label, ImVec4* color);
        void AddLabel(const char* label);
        void AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items = -1);
        void AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items = -1, ComboBoxStyle style = ComboBoxStyle::ImGuiDefault);
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

