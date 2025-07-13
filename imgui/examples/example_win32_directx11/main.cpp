// Dear ImGui: standalone example application for DirectX 11

// Learn about Dear ImGui:
// - FAQ                  https://dearimgui.com/faq
// - Getting Started      https://dearimgui.com/getting-started
// - Documentation        https://dearimgui.com/docs (same as your local docs/ folder).
// - Introduction, links and more at the top of imgui.cpp

#include "imgui.h"
#include "imgui_impl_win32.h"
#include "imgui_impl_dx11.h"
#include <d3d11.h>
#include <tchar.h>
#include "../ezImgui/ezImgui.h"
#include <random> // not needed by default
#include "../../../ezWidgets/ezWidgets.h"
//#include "../../../../../Downloads/font_array.h"

// Data
static ID3D11Device*            g_pd3dDevice = nullptr;
static ID3D11DeviceContext*     g_pd3dDeviceContext = nullptr;
static IDXGISwapChain*          g_pSwapChain = nullptr;
static bool                     g_SwapChainOccluded = false;
static UINT                     g_ResizeWidth = 0, g_ResizeHeight = 0;
static ID3D11RenderTargetView*  g_mainRenderTargetView = nullptr;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

int currentTab = 0;
std::shared_ptr<std::unordered_map<int, bool>> perms = std::make_shared<std::unordered_map<int, bool>>();
std::shared_ptr<std::unordered_map<int, bool>> numbers = std::make_shared<std::unordered_map<int, bool>>();

int getRandomInRange(int min, int max) { // just for debugging not supposed to be included
    static std::mt19937 gen(static_cast<unsigned int>(std::time(nullptr)));
    std::uniform_int_distribution<> distrib(min, max);
    return distrib(gen);
}

// Main code
int main(int, char**)
{
    // Create application window
    //ImGui_ImplWin32_EnableDpiAwareness();
    WNDCLASSEXW wc = { sizeof(wc), CS_CLASSDC, WndProc, 0L, 0L, GetModuleHandle(nullptr), nullptr, nullptr, nullptr, nullptr, L"ImGui Example", nullptr };
    ::RegisterClassExW(&wc);
    HWND hwnd = ::CreateWindowW(wc.lpszClassName, L"Dear ImGui DirectX11 Example", WS_OVERLAPPEDWINDOW, 100, 100, 1280, 800, nullptr, nullptr, wc.hInstance, nullptr);

    // Initialize Direct3D
    if (!CreateDeviceD3D(hwnd))
    {
        CleanupDeviceD3D();
        ::UnregisterClassW(wc.lpszClassName, wc.hInstance);
        return 1;
    }

    // Show the window
    ::ShowWindow(hwnd, SW_SHOWDEFAULT);
    ::UpdateWindow(hwnd);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsLight();

    // Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(hwnd);
    ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return a nullptr. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Use '#define IMGUI_ENABLE_FREETYPE' in your imconfig file to use Freetype for higher quality font rendering.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, nullptr, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != nullptr);
    ez::LoadFont("default", "C:/Windows/Fonts/segoeui.ttf", 17.0f);
    //ez::LoadFont("mono", "C:/Windows/Fonts/consola.ttf", 13.0f);
    //ez::LoadFontFromMemory("pixel", pixelFONT, sizeof(pixelFONT), 15.0f);

    // Our state
    bool show_demo_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);


    // Main loop
    bool done = false;
    while (!done)
    {
        // Poll and handle messages (inputs, window resize, etc.)
        // See the WndProc() function below for our to dispatch events to the Win32 backend.
        MSG msg;
        while (::PeekMessage(&msg, nullptr, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&msg);
            ::DispatchMessage(&msg);
            if (msg.message == WM_QUIT)
                done = true;
        }
        if (done)
            break;

        // Handle window being minimized or screen locked
        if (g_SwapChainOccluded && g_pSwapChain->Present(0, DXGI_PRESENT_TEST) == DXGI_STATUS_OCCLUDED)
        {
            ::Sleep(10);
            continue;
        }
        g_SwapChainOccluded = false;

        // Handle window resize (we don't resize directly in the WM_SIZE handler)
        if (g_ResizeWidth != 0 && g_ResizeHeight != 0)
        {
            CleanupRenderTarget();
            g_pSwapChain->ResizeBuffers(0, g_ResizeWidth, g_ResizeHeight, DXGI_FORMAT_UNKNOWN, 0);
            g_ResizeWidth = g_ResizeHeight = 0;
            CreateRenderTarget();
        }

        // Start the Dear ImGui frame
        ImGui_ImplDX11_NewFrame();
        ImGui_ImplWin32_NewFrame();
        ImGui::NewFrame();

        // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
        if (show_demo_window)
            ImGui::ShowDemoWindow(&show_demo_window);

        static auto side = ez::TabboxSide::Left;
        static bool myToggle = false;
        static bool myToggle2 = false;
        static bool myToggle3 = false;
        static bool myToggle4 = false;
        static int myChoice = 0;
        static int myChoice2 = 0;
        static float mySlider = 0.0f;
        static int myIntSlider = 0;
        static ImVec4 myColor = ImVec4(1, 0, 0, 1);

        auto myWindow = ez::CreateEzWindow("Test Window", ImVec2(510, 500), ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse, true);
        myWindow->tabMode = ez::TabMode::ButtonTabs;
        myWindow->tabButtonOrientation = ez::TabButtonOrientation::HorizontalTop;
        myWindow->style.ScrollbarSize = 1.3f;
        auto tab1 = myWindow->AddTab("First");
        auto tab2 = myWindow->AddTab("Second");
        auto tab3 = myWindow->AddTab("Third");
        auto tab4 = myWindow->AddTab("Fourth");
        auto settingsTab = myWindow->AddTab("Settings");

        auto tabbox1 = tab1->AddTabbox("Tab header 1");
        
        tabbox1->AddCheckbox("Toggle 1", &myToggle);
        tabbox1->AddCheckbox("Toggle 2", &myToggle2);
        tabbox1->AddCheckbox("Toggle 3", &myToggle3);
        tabbox1->AddCheckboxColorPicker("Toggle 4", &myToggle4, &myColor);
        tabbox1->AddSlider("Slider Int", &myIntSlider, 0, 100);
        tabbox1->AddSlider("Slider Float", &mySlider, 0.f, 100.f);

        auto tabbox2 = tab1->AddTabbox("Tab header 2", ez::TabboxSide::Right);

        tabbox2->AddButton("Notification Testing", [] {
            int randNum = getRandomInRange(-100, 100);
            ez::PushNotification("[SYSTEM] Config " + std::to_string(randNum) + " saved!");

        });

        //tabbox2->AddGradientButton("Test", IM_COL32(255, 255, 255, 255), IM_COL32(155, 0, 0, 255), IM_COL32(30, 0, 0, 255), [] {
        //    int randNum = getRandomInRange(-100, 100);
        //    ez::PushNotification("[SYSTEM] Config " + std::to_string(randNum) + " saved!");

        //    });
        //tabbox2->AddGradientButton("hi", IM_COL32(255, 255, 255, 255), IM_COL32(155, 0, 0, 255), IM_COL32(30, 0, 0, 255));
        tabbox2->AddComboBox("Combo 1", &myChoice, { "one", "two", "three" }, -1);
        tabbox2->AddComboBox("Combo 22", &myChoice2, { "Choice 1", "Choice 2", "Choice 3" }, -1);
        tabbox2->AddMultiComboBox("Mult Combo", { "Read", "Write", "Execute", "Delete" }, perms);

        auto tabbox1Tab2 = tab2->AddTabbox("Custom Assets", ez::TabboxSide::Left);
        tabbox1Tab2->AddSlider("Slider Intb", &myIntSlider, 0, 100);
        tabbox1Tab2->AddSlider("Slider Inta", &myIntSlider, 0, 100);
        
        auto settingsColorTab = settingsTab->AddTabbox("Menu Colors");
        settingsColorTab->AddColorPicker("Tabbox Border Color", &ez::tbxBorderColor);
        settingsColorTab->AddColorPicker("Content Border Color", &ez::contentFrameBorderBg);
        settingsColorTab->AddColorPicker("Content Seperator Color", &ez::contentSeperatorColor);
        settingsColorTab->AddColorPicker("Tabbox Background Color", &ez::tbxBackgroundColor);
        settingsColorTab->AddColorPicker("Window Background Color", &ez::winBackgroundColor);
        settingsColorTab->AddColorPicker("Frame Background Color", &ez::frameBg);
        settingsColorTab->AddColorPicker("Frame Background Active Color", &ez::frameBgActive);
        settingsColorTab->AddColorPicker("Frame Background Hovered Color", &ez::frameBgHovered);
        settingsColorTab->AddColorPicker("Accent Color", &ez::accentColor);
        settingsColorTab->AddColorPicker("Button Color", &ez::buttonColor);

        auto settingsStyleTab = settingsTab->AddTabbox("Menu Style Vars", ez::TabboxSide::Right);
        settingsStyleTab->AddSlider("Tabbox Rounding", &ez::tabboxRounding, 0, 10);
        settingsStyleTab->AddSlider("Frame Rounding", &ez::frameRounding, 0, 10);

        ez::RenderNotifications();
        myWindow->Render();

        // Rendering
        ImGui::Render();
        const float clear_color_with_alpha[4] = { clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w };
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, nullptr);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, clear_color_with_alpha);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

        // Present
        HRESULT hr = g_pSwapChain->Present(1, 0);   // Present with vsync
        //HRESULT hr = g_pSwapChain->Present(0, 0); // Present without vsync
        g_SwapChainOccluded = (hr == DXGI_STATUS_OCCLUDED);
    }

    // Cleanup
    ImGui_ImplDX11_Shutdown();
    ImGui_ImplWin32_Shutdown();
    ImGui::DestroyContext();

    CleanupDeviceD3D();
    ::DestroyWindow(hwnd);
    ::UnregisterClassW(wc.lpszClassName, wc.hInstance);

    return 0;
}

// Helper functions

bool CreateDeviceD3D(HWND hWnd)
{
    // Setup swap chain
    DXGI_SWAP_CHAIN_DESC sd;
    ZeroMemory(&sd, sizeof(sd));
    sd.BufferCount = 2;
    sd.BufferDesc.Width = 0;
    sd.BufferDesc.Height = 0;
    sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    sd.BufferDesc.RefreshRate.Numerator = 60;
    sd.BufferDesc.RefreshRate.Denominator = 1;
    sd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
    sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    sd.OutputWindow = hWnd;
    sd.SampleDesc.Count = 1;
    sd.SampleDesc.Quality = 0;
    sd.Windowed = TRUE;
    sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

    UINT createDeviceFlags = 0;
    //createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
    D3D_FEATURE_LEVEL featureLevel;
    const D3D_FEATURE_LEVEL featureLevelArray[2] = { D3D_FEATURE_LEVEL_11_0, D3D_FEATURE_LEVEL_10_0, };
    HRESULT res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res == DXGI_ERROR_UNSUPPORTED) // Try high-performance WARP software driver if hardware is not available.
        res = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_WARP, nullptr, createDeviceFlags, featureLevelArray, 2, D3D11_SDK_VERSION, &sd, &g_pSwapChain, &g_pd3dDevice, &featureLevel, &g_pd3dDeviceContext);
    if (res != S_OK)
        return false;

    CreateRenderTarget();
    return true;
}

void CleanupDeviceD3D()
{
    CleanupRenderTarget();
    if (g_pSwapChain) { g_pSwapChain->Release(); g_pSwapChain = nullptr; }
    if (g_pd3dDeviceContext) { g_pd3dDeviceContext->Release(); g_pd3dDeviceContext = nullptr; }
    if (g_pd3dDevice) { g_pd3dDevice->Release(); g_pd3dDevice = nullptr; }
}

void CreateRenderTarget()
{
    ID3D11Texture2D* pBackBuffer;
    g_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackBuffer));
    g_pd3dDevice->CreateRenderTargetView(pBackBuffer, nullptr, &g_mainRenderTargetView);
    pBackBuffer->Release();
}

void CleanupRenderTarget()
{
    if (g_mainRenderTargetView) { g_mainRenderTargetView->Release(); g_mainRenderTargetView = nullptr; }
}

// Forward declare message handler from imgui_impl_win32.cpp
extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Win32 message handler
// You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
// - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application, or clear/overwrite your copy of the mouse data.
// - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application, or clear/overwrite your copy of the keyboard data.
// Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
LRESULT WINAPI WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hWnd, msg, wParam, lParam))
        return true;

    switch (msg)
    {
    case WM_SIZE:
        if (wParam == SIZE_MINIMIZED)
            return 0;
        g_ResizeWidth = (UINT)LOWORD(lParam); // Queue resize
        g_ResizeHeight = (UINT)HIWORD(lParam);
        return 0;
    case WM_SYSCOMMAND:
        if ((wParam & 0xfff0) == SC_KEYMENU) // Disable ALT application menu
            return 0;
        break;
    case WM_DESTROY:
        ::PostQuitMessage(0);
        return 0;
    }
    return ::DefWindowProcW(hWnd, msg, wParam, lParam);
}


//{
//    static float f = 0.0f;
//    static int counter = 0;

//    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

//    ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
//    ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
//    ImGui::Checkbox("Another Window", &show_another_window);

//    ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
//    ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

//    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
//        counter++;
//    ImGui::SameLine();
//    ImGui::Text("counter = %d", counter);

//    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);
//    ImGui::End();
//}
