### EzImgui 

EzImgui is an extension that utilizes the [ImGui graphical interface library](https://github.com/ocornut/imgui) and makes the process of creating your own user interfaces much easier for those of us who dont want to mess with scaling/sizing/tab controls/etc. As it stands right now EzImgui is still very much a work in progress and has only been tested in win32_dx11. 

### EzWidgets

EzWidgets is a system that works with EzImgui to allow for customization and use of different element themes and styles.

*EzWidgets is NOT required in order to use EzImgui.*

## Code Snippets

Setting up a window is very similar to how you would regularly set it up. Each time you create a window, it needs to be rendered, forgetting to call the window to render it will cause the window to not appear. (This section is still very messy, will be changed later)

```cpp
#include "../ezImgui/ezImgui.h"

// Window Start
auto myWindow = ez::CreateEzWindow("My Brand New Window", ImVec2(size1, size2), WINDOW_FLAGS, true);

// Window End
myWindow->Render();

```

Adding tabs, tab boxes
```cpp
auto tab1 = myWindow->AddTab("First");
auto tab2 = myWindow->AddTab("Second");

auto tbbx1 = tab1->AddTabbox("Tabbox 1", ez::TabboxSide::Left);

auto tbbx2 = tab1->AddTabbox("Tabbox 2", ez::TabboxSide::Right);

```

Adding UI Elements
```cpp
// When adding elements such as checkboxes
// they can be added to tabboxes or tabbox tabs

// Tabboxes
auto tab1 = myWindow->AddTab("First");
auto tbbx1 = tab1->AddTabbox("Tabbox 1", ez::TabboxSide::Left);
tbbx1->AddLabel("Avacados");
tbbx1->AddLabel("Bananas");
tbbx1->AddCheckbox("Checkmark", &myToggle);

// Tabbox Tabs
auto tbbx2 = tab1->AddTabbox("Tabbox 2", ez::TabboxSide::Right);
auto tb2Tab1 = tbbx2->AddTab("1");
tb2Tab1->AddLabel("Tomatoes");

auto tb2Tab2 = tbbx2->AddTab("2");
tb2Tab2->AddLabel("Cucumbers");

```




Adding Fonts
```cpp
ez::LoadFont("default", "C:/Windows/Fonts/segoeui.ttf", 17.0f);
ez::LoadFont("mono", "C:/Windows/Fonts/consola.ttf", 13.0f);
ez::LoadFontFromMemory("pixel", pixelFONT, sizeof(pixelFONT), 15.0f);

```
