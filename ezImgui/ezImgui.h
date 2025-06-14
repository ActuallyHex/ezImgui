// ez.h
#pragma once
#include <imgui.h>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace ez {

    inline std::unordered_map<std::string, ImFont*> fonts;
    inline std::string currentFontName = "default";

    void LoadFont(const std::string& name, const char* path, float size);
    void SetFont(const std::string& name);

    enum class TabboxSide {
        Left,
        Right
    };

    struct TabboxTab;
    struct Tabbox;
    struct Tab;
    struct Window;

    struct TabboxTab {
        std::string name;
        std::vector<std::pair<std::string, bool*>> checkboxes;
        std::vector<std::pair<std::string, bool*>> toggles;
        std::vector<std::tuple<std::string, float*, float, float>> sliders;
        std::vector<std::pair<std::string, ImVec4*>> colorPickers;
        std::vector<std::string> labels;

        void AddToggle(const char* label, bool* value);
        void AddLabel(const char* label);
        void AddSlider(const char* label, float* value, float min, float max);
        void AddColorPicker(const char* label, ImVec4* color);
        void Render();
    };

    struct Tabbox {
        std::string name;
        std::vector<std::shared_ptr<TabboxTab>> tabs;
        int currentTabIndex = 0;
        TabboxSide side = TabboxSide::Left;

        // Optional tabbox-level elements
        std::vector<std::pair<std::string, bool*>> checkboxes;
        std::vector<std::pair<std::string, bool*>> toggles;
        std::vector<std::tuple<std::string, float*, float, float>> sliders;
        std::vector<std::pair<std::string, ImVec4*>> colorPickers;
        std::vector<std::string> labels;

        std::shared_ptr<TabboxTab> AddTab(const char* name);
        void AddToggle(const char* label, bool* value);
        void AddLabel(const char* label);
        void AddSlider(const char* label, float* value, float min, float max);
        void AddColorPicker(const char* label, ImVec4* color);
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

}
