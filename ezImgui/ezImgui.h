#pragma once
#include <imgui.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>
#include <variant>
#include <functional>

//#define USE_EZ_WIDGETS

namespace ez {

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

    struct UIElement {
        ElementType type;
        CheckboxStyle styleValue;
        ComboBoxStyle comboStyle;
        SliderStyle sliderStyle;
        ButtonStyle buttonStyle;
        std::string label;
        std::vector<std::string> comboItemStorage;                  // string copies
        std::vector<const char*> comboItemPointers;                 // char* pointers. Changed because combo box data was being overwritten when multiple combo boxes existed in the same tabbox.
        std::shared_ptr<std::unordered_map<int, bool>> multiComboData;
        std::vector<const char*> multiComboRawItems;
        std::function<void()> buttonCallback;
        struct ButtonTag {};

        union {
            bool* boolValue;
            struct { float* valueFloat; float minFloat; float maxFloat; };
            struct { int* valueInt; int minInt; int maxInt; };
            ImVec4* colorValue;
            struct { bool* boolVal; ImVec4* clrVal; };
            struct { int* currentItem; const char* const* items; int itemsCount; int heightInItems; } comboData;
        };

        #ifdef USE_EZ_WIDGETS
            ImU32 text_color;
            ImU32 bg_color_1;
            ImU32 bg_color_2;

            UIElement(const std::string& lbl, ButtonTag, ImU32 txtClr, ImU32 bgClr1, ImU32 bgClr2, std::function<void()> callback = nullptr)
                : type(ElementType::GradientButton), label(lbl), text_color(txtClr), bg_color_1(bgClr1), bg_color_2(bgClr2), buttonCallback(callback) {
            }

            UIElement(const std::string& lbl, bool* val, CheckboxStyle style = CheckboxStyle::ImGuiDefault)
                : type(ElementType::Toggle), label(lbl), boolValue(val), styleValue(style) {
            }
            UIElement(const std::string& lbl, bool* val, ImVec4* clr, CheckboxStyle style = CheckboxStyle::ImGuiDefault)
                : type(ElementType::ToggleColorPicker), label(lbl), boolVal(val), clrVal(clr), styleValue(style) {
            }
           

            UIElement(const std::string& lbl, float* val, float mi, float ma, SliderStyle style = SliderStyle::ImGuiDefault)
                : type(ElementType::SliderFloat), label(lbl), sliderStyle(style)
            {
                valueFloat = val;
                minFloat = mi;
                maxFloat = ma;
            }

            UIElement(const std::string& lbl, int* val, int mi, int ma, SliderStyle style = SliderStyle::ImGuiDefault)
                : type(ElementType::SliderInt), label(lbl), sliderStyle(style)
            {
                valueInt = val;
                minInt = mi;
                maxInt = ma;
            }

        #else
            UIElement(const std::string& lbl, bool* val)
                : type(ElementType::Toggle), label(lbl), boolValue(val) {
            }

            UIElement(const std::string& lbl, bool* val, ImVec4* clr)
                : type(ElementType::ToggleColorPicker), label(lbl), boolVal(val), clrVal(clr) {
            }

            UIElement(const std::string& lbl, float* val, float mi, float ma)
                : type(ElementType::SliderFloat), label(lbl) {
                valueFloat = val; minFloat = mi; maxFloat = ma;
            }
            UIElement(const std::string& lbl, int* val, int mi, int ma)
                : type(ElementType::SliderInt), label(lbl) {
                valueInt = val; minInt = mi; maxInt = ma;
            }

        #endif

        UIElement(const std::string& lbl, int* curval, std::initializer_list<const char*> itemsInitList, int height)
            : type(ElementType::ComboBox), label(lbl) {
            comboData.currentItem = curval;
            comboData.heightInItems = height;

            comboItemStorage.clear();
            for (const auto& s : itemsInitList)
                comboItemStorage.emplace_back(s);

            comboItemPointers.clear();
            for (const auto& str : comboItemStorage)
                comboItemPointers.push_back(str.c_str());

            comboData.items = comboItemPointers.data();
            comboData.itemsCount = static_cast<int>(comboItemPointers.size());
        }
        UIElement(const std::string& lbl, int* curval, std::initializer_list<const char*> itemsInitList, int height, ComboBoxStyle style)
            : type(ElementType::ComboBox), label(lbl), comboStyle(style) {
            comboData.currentItem = curval;
            comboData.heightInItems = height;

            comboItemStorage.clear();
            for (const auto& s : itemsInitList)
                comboItemStorage.emplace_back(s);

            comboItemPointers.clear();
            for (const auto& str : comboItemStorage)
                comboItemPointers.push_back(str.c_str());

            comboData.items = comboItemPointers.data();
            comboData.itemsCount = static_cast<int>(comboItemPointers.size());
        }
        UIElement(const std::string& lbl, ImVec4* col)
            : type(ElementType::ColorPicker), label(lbl), colorValue(col) {
        }
        UIElement(const std::string& lbl)
            : type(ElementType::Label), label(lbl) {
        }
        UIElement(const std::string& lbl, std::initializer_list<const char*> itemsList, std::shared_ptr<std::unordered_map<int, bool>> data)
            : type(ElementType::MultiComboBox), label(lbl), multiComboData(std::move(data)) {
            comboItemStorage.assign(itemsList.begin(), itemsList.end());
            multiComboRawItems.reserve(comboItemStorage.size());
            for (const auto& s : comboItemStorage)
                multiComboRawItems.push_back(s.c_str());
        }

        UIElement(const std::string& lbl, ButtonTag, std::function<void()> callback = nullptr, ButtonStyle style = ButtonStyle::ImGuiDefault)
             : type(ElementType::Button), label(lbl), buttonCallback(callback), buttonStyle(style) {
        }
    };

    struct TabboxTab {
        std::string name;
        std::vector<UIElement> elements;

        void AddCheckbox(const char* label, bool* value);
        void AddCheckbox(const char* label, bool* value, CheckboxStyle style);                               // Only used in EzWidgets
        void AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color);
        void AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color, CheckboxStyle style);     // Only used in EzWidgets
        void AddSlider(const char* label, float* value, float min, float max, SliderStyle style);
        void AddSlider(const char* label, int* value, int min, int max, SliderStyle style);
        void AddSlider(const char* label, float* value, float min, float max);
        void AddSlider(const char* label, int* value, int min, int max);
        void AddColorPicker(const char* label, ImVec4* color);
        void AddLabel(const char* label);
        void AddMultiComboBox(const char* label, std::initializer_list<const char*> items, std::shared_ptr<std::unordered_map<int, bool>> data);
        void AddButton(const char* label, std::function<void()> onClick = nullptr, ButtonStyle style = ButtonStyle::ImGuiDefault);

        void AddGradientButton(const char* label, ImU32 textClr, ImU32 bgClr1, ImU32 bgClr2, std::function<void()> onClick = nullptr);
        void AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items);
        void AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items, ComboBoxStyle style);     // Only used in EzWidgets

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
        void AddCheckbox(const char* label, bool* value, CheckboxStyle style);     // Only used in EzWidgets
        void AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color);
        void AddCheckboxColorPicker(const char* label, bool* value, ImVec4* color, CheckboxStyle style);     // Only used in EzWidgets
        void AddSlider(const char* label, float* value, float min, float max, SliderStyle style);
        void AddSlider(const char* label, int* value, int min, int max, SliderStyle style);
        void AddSlider(const char* label, float* value, float min, float max);
        void AddSlider(const char* label, int* value, int min, int max);
        void AddColorPicker(const char* label, ImVec4* color);
        void AddLabel(const char* label);
        void AddMultiComboBox(const char* label, std::initializer_list<const char*> items, std::shared_ptr<std::unordered_map<int, bool>> data);
        void AddButton(const char* label, std::function<void()> onClick = nullptr, ButtonStyle style = ButtonStyle::ImGuiDefault);
        void AddGradientButton(const char* label, ImU32 textClr, ImU32 bgClr1, ImU32 bgClr2, std::function<void()> onClick = nullptr);
        void AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items = -1);
        void AddComboBox(const char* label, int* current_item, std::initializer_list<const char*> items, int height_in_items, ComboBoxStyle style);     // Only used in EzWidgets



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

