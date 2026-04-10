### EzImgui & EzWidgets

**EzImgui is still an early W.I.P project and some of the code is ugly.**

EzImgui is an "extension" that utilizes the [ImGui graphical interface library](https://github.com/ocornut/imgui) and tries to add a few QoL features to the libary. The goal is to make a drop in solution for ImGui projects that enables easier customization and makes the process of creating your own GUIs much easier for those of us who dont want to mess with scaling/sizing/tab controls/etc which can be frustrating.

EzWidgets is a system that works with EzImgui to allow for customization and use of different element themes and styles without needing to modify the ImGui source.

*EzWidgets is NOT required in order to use EzImgui.*

## Highlights
- Drop-in solution (Only need to include the EzImgui.h/.cpp files)
- Performance oriented customizable outlined text labels (Utilizing glyph quads)
- Automatic tab box scaling (Sized based on number of element in one tab box)
- Easily and highly customizable source with theming options
- Straightforward and easy to use

## Using EzImgui
```cpp

auto myWindow = ez::CreateEzWindow("EzImGui Demo", ImVec2(550, 500), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, true);
myWindow->tabMode = ez::TabMode::ButtonTabs;
myWindow->tabButtonOrientation = ez::TabButtonOrientation::HorizontalTop;
myWindow->style.ScrollbarSize = 1.3f;
auto tab1 = myWindow->AddTab("First Tab");
auto tab2 = myWindow->AddTab("Second");
auto tab3 = myWindow->AddTab("Third");
auto tab4 = myWindow->AddTab("Fourth");
auto settingsTab = myWindow->AddTab("Settings");

auto tabbox1 = tab1->AddTabbox("Toggles");
tabbox1->AddCheckbox("Toggle 1", &myToggle);
tabbox1->AddCheckboxColorPicker("Toggle 2", &myToggle2, &myColor);

auto tabbox2 = tab1->AddTabbox("Sliders", ez::TabboxSide::Right);
tabbox2->AddSlider("Slider Int", &myIntSlider, 0, 100);
tabbox2->AddSlider("Slider Float", &mySlider, 0.f, 100.f);

auto tabbox3 = tab1->AddTabbox("Drop Downs", ez::TabboxSide::Left);
tabbox3->AddComboBox("Combo Box", &myChoice, { "one", "two", "three", "four"}, -1);
tabbox3->AddMultiComboBox("Multi Combo", { "Read", "Write", "Execute", "Delete" }, perms);

auto tabbox4 = tab1->AddTabbox("Buttons", ez::TabboxSide::Right);
tabbox4->AddButton("Button");
tabbox4->AddButton("Function Button", [] {
     int randNum = getRandomInRange(-100, 100);
     ez::PushNotification("[SYSTEM] Notification Number " + std::to_string(randNum) + " ALERT!");
});
auto tabbox5 = tab1->AddTabbox("Labels", ez::TabboxSide::Left);
tabbox5->AddLabel("Hello, World!");
tabbox5->AddTextWithOutline("Hello,",
     IM_COL32(255, 255, 255, 255),  // fill
     IM_COL32(255, 0, 0, 255),      // outline
     1.0f                           // thickness
);
tabbox5->AddTextWithOutline("Outlined",
     IM_COL32(255, 255, 255, 255),
     IM_COL32(0, 255, 0, 255),
     1.0f
);
tabbox5->AddTextWithOutline("World!",
     IM_COL32(255, 255, 255, 255),
     IM_COL32(0, 0, 255, 255),
     1.0f
);

ez::RenderNotifications();
myWindow->Render(); // Just like imgui if you create a window you will need to call myWindow->Render(); before ImGui::Render();

```
![Screenshot 2025-07-05 163536](https://github.com/user-attachments/assets/a49a1da4-c034-46d3-b7ce-00e93c2246e0)