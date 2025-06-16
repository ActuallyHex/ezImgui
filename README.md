### EzImgui

EzImgui is an extension that utilizes and leverages the ImGui graphical interface library and makes the experience of developing your own user interface much easier.

## Code Snippets

Setting up a window is very similar to how you would regularly set it up. Each time you create a window, it needs to be rendered, forgetting to call the window to render it will cause the window to not appear.

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

Adding Fonts
```cpp
ez::LoadFont("default", "C:/Windows/Fonts/segoeui.ttf", 17.0f);
ez::LoadFont("mono", "C:/Windows/Fonts/consola.ttf", 13.0f);
ez::LoadFontFromMemory("pixel", pixelFONT, sizeof(pixelFONT), 15.0f);

```
