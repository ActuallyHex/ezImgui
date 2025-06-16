#include "ezImgui.h"

#define USE_EZ_WIDGETS

#ifdef USE_EZ_WIDGETS
#include "../ezWidgets/ezWidgets.h"
#endif

namespace ez {

    WidgetMode g_WidgetMode = WidgetMode::FancyWidgets;
    CheckboxMode g_CheckboxMode = CheckboxMode::Anim1;

    Window::Window(const char* title_, ImVec2 size_, ImGuiWindowFlags flags_, bool autoshow_)
        : title(title_), size(size_), flags(flags_), autoshow(autoshow_) {
    }

    std::shared_ptr<Window> CreateEzWindow(const char* title, ImVec2 size, ImGuiWindowFlags flags, bool autoshow) {
        return std::make_shared<Window>(title, size, flags, autoshow);
    }

    void LoadFont(const std::string& name, const char* path, float size) {
        ImGuiIO& io = ImGui::GetIO();
        ImFont* font = io.Fonts->AddFontFromFileTTF(path, size);
        if (font)
            fonts[name] = font;
    }

    void LoadFontFromMemory(const std::string& name, void* data, int size_bytes, float size_pixels)
    {
        ImGuiIO& io = ImGui::GetIO();
        ImFontConfig font_cfg;
        font_cfg.FontDataOwnedByAtlas = false; // don't let ImGui delete your memory

        ImFont* font = io.Fonts->AddFontFromMemoryTTF(data, size_bytes, size_pixels, &font_cfg);
        if (font) {
            fonts[name] = font;
        }
    }

    // SetFont
    // Add way to pushfont tabboxes and its contents 
    void SetFont(const std::string& name) {
        if (fonts.contains(name)) {
            currentFontName = name;
            ImGui::PushFont(fonts[name]);
        }
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

    void TabboxTab::AddCheckbox(const char* label, bool* value) {
        elements.emplace_back(label, value);
    }

    void TabboxTab::AddSlider(const char* label, float* value, float min, float max) {
        elements.emplace_back(label, value, min, max);
    }

    void TabboxTab::AddColorPicker(const char* label, ImVec4* color) {
        elements.emplace_back(label, color);
    }

    void TabboxTab::AddLabel(const char* label) {
        elements.emplace_back(label);
    }

    void Tabbox::AddCheckbox(const char* label, bool* value) {
        elements.emplace_back(label, value);
    }

    void Tabbox::AddSlider(const char* label, float* value, float min, float max) {
        elements.emplace_back(label, value, min, max);
    }

    void Tabbox::AddColorPicker(const char* label, ImVec4* color) {
        elements.emplace_back(label, color);
    }

    void Tabbox::AddLabel(const char* label) {
        elements.emplace_back(label);
    }

    void TabboxTab::Render() {
        for (const auto& e : elements) {
            switch (e.type) {
            case ElementType::Toggle:
            {
                #ifdef USE_EZ_WIDGETS
                    if (ez::g_CheckboxMode == ez::CheckboxMode::ToggleSwitch)
                        ezWidgets::ToggleSwitch(e.label.c_str(), e.boolValue);
                    else if (ez::g_CheckboxMode == ez::CheckboxMode::Anim1)
                        ezWidgets::CheckboxAnim1(e.label.c_str(), e.boolValue);
                    else if (ez::g_CheckboxMode == ez::CheckboxMode::Anim2)
                        ezWidgets::CheckboxAnim2(e.label.c_str(), e.boolValue);
                    else
                        ImGui::Checkbox(e.label.c_str(), e.boolValue); // fall back to default
                #else // USE_EZ_WIDGETS
                    ImGui::Checkbox(e.label.c_str(), e.boolValue);
                #endif

                break;
            }
            case ElementType::Slider:
                ImGui::SliderFloat(e.label.c_str(), e.value, e.min, e.max);
                break;
            case ElementType::ColorPicker:
                ImGui::ColorEdit4(e.label.c_str(), (float*)e.colorValue, ImGuiColorEditFlags_NoInputs);
                break;
            case ElementType::Label:
                ImGui::Text("%s", e.label.c_str());
                break;
            }
        }
    }

    void Tabbox::RenderExtras() {
        for (const auto& e : elements) {
            switch (e.type) {
            case ElementType::Toggle:
                ImGui::Checkbox(e.label.c_str(), e.boolValue);
                break;
            case ElementType::Slider:
                ImGui::SliderFloat(e.label.c_str(), e.value, e.min, e.max);
                break;
            case ElementType::ColorPicker:
                ImGui::ColorEdit4(e.label.c_str(), (float*)e.colorValue, ImGuiColorEditFlags_NoInputs);
                break;
            case ElementType::Label:
                ImGui::Text("%s", e.label.c_str());
                break;
            }
        }
    }

    void Window::Render() {
        ImGui::SetNextWindowSize(size, ImGuiCond_FirstUseEver);
        ImGui::Begin(title.c_str(), &isOpen, flags);

        if (fonts.contains(currentFontName))
            ImGui::PushFont(fonts[currentFontName]);

        if (ImGui::BeginTabBar("##tabs")) {
            for (int i = 0; i < tabNames.size(); ++i) {
                if (ImGui::BeginTabItem(tabNames[i].c_str())) {
                    currentTab = i;
                    ImGui::EndTabItem();
                }
            }
            ImGui::EndTabBar();
        }

        ImVec2 contentSize = ImVec2(size.x - 15, size.y - 60);
        ImGui::BeginChild("##MainFrame", contentSize, true);
        {
            auto& tab = tabs[currentTab];
            ImGui::Columns(2, nullptr, false);
            static std::unordered_map<std::string, int> tabboxStates;

            for (int sideIndex = 0; sideIndex < 2; ++sideIndex) {
                TabboxSide currentSide = (sideIndex == 0) ? TabboxSide::Left : TabboxSide::Right;
                for (auto& tabbox : tab->tabboxes) {
                    if (tabbox->side == currentSide) {
                        ImGui::BeginChild(tabbox->name.c_str(), ImVec2(0, 200), true);
                        ImGui::Text("%s", tabbox->name.c_str());
                        ImGui::Separator();

                        int& currentIndex = tabboxStates[tabbox->name];
                        float buttonWidth = ImGui::GetContentRegionAvail().x / std::max(1, (int)tabbox->tabs.size());
                        for (int i = 0; i < tabbox->tabs.size(); ++i) {
                            ImGui::PushID(i);
                            bool isSelected = (currentIndex == i);
                            if (ImGui::Button((tabbox->tabs[i]->name + "##" + tabbox->name).c_str(), ImVec2(buttonWidth - 4.0f, 0))) {
                                currentIndex = i;
                            }

                            if (i < tabbox->tabs.size() - 1)
                                ImGui::SameLine(0.0f, 4.0f);

                            ImGui::PopID();
                        }

                        if (!tabbox->tabs.empty())
                            ImGui::Separator();

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

                        ImGui::EndChild();
                    }
                }
                ImGui::NextColumn();
            }
        }
        ImGui::EndChild();

        if (fonts.contains(currentFontName))
            ImGui::PopFont();

        ImGui::End();
    }

} // namespace ez