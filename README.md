### EzImgui 

EzImgui is an extension that utilizes the [ImGui graphical interface library](https://github.com/ocornut/imgui) and makes the process of creating your own interfaces much easier for those of us who dont want to mess with scaling/sizing/tab controls/etc. 

**EzImgui is still an early W.I.P project and some of the code is ugly.**

### EzWidgets

EzWidgets is a system that works with EzImgui to allow for customization and use of different element themes and styles.

*EzWidgets is NOT required in order to use EzImgui.*

## Using EzImgui
```cpp

auto myWindow = ez::CreateEzWindow("Test Window", ImVec2(510, 400), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, true);
myWindow->tabMode = ez::TabMode::ButtonTabs; // Tab Button Mode selector (ImGuiTabs or ButtonTabs)
myWindow->style.ScrollbarSize = 1.3f;
auto tab1 = myWindow->AddTab("Tab 1");
auto tab2 = myWindow->AddTab("Tab 2");

auto tabbox1 = tab1->AddTabbox("Tab header 1");
tabbox1->AddCheckbox("Toggle 1", &myToggle);
tabbox1->AddCheckbox("Toggle 2", &myToggle2);
tabbox1->AddCheckboxColorPicker("Toggle 3", &myToggle3, &myColor);

auto tabbox2 = tab1->AddTabbox("Tab header 2");
tabbox2->AddSlider("Slider Int", &myIntSlider, 0, 100);
tabbox2->AddSlider("Slider Float", &mySlider, 0.f, 100.f);

auto tabbox3 = tab1->AddTabbox("Tab header 3", ez::TabboxSide::Right); // Optional tabbox sides
tabbox3->AddButton("Button");
tabbox3->AddButton("Function Button", [] {  // Buttons can have functions inside them
     ez::PushNotification("Pop out message!");
});

auto tabbox4 = tab1->AddTabbox("Tab header 4", ez::TabboxSide::Right);
tabbox4->AddComboBox("Combo 1", &myChoice, { "one", "two", "three" }, -1);
tabbox4->AddComboBox("Combo 22", &myChoice2, { "Choice 1", "Choice 2", "Choice 3" }, -1);
tabbox4->AddMultiComboBox("Mult Combo", { "Read", "Write", "Execute", "Delete" }, perms);

ez::RenderNotifications();
myWindow->Render(); // Just like imgui if you create a window you will need to call myWindow->Render(); before ImGui::Render();

```
![Screenshot 2025-07-05 163536](https://github.com/user-attachments/assets/a49a1da4-c034-46d3-b7ce-00e93c2246e0)
