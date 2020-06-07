#ifdef HELLOIMGUI_USE_SDL_DIRECTX11


#include "imgui.h"
#include "examples/imgui_impl_dx11.h"
#include "examples/imgui_impl_sdl.h"
#include <SDL.h>
#include <SDL_syswm.h>
#include <d3d11.h>
#include <stdio.h>
#include <sstream>

#include "runner_sdl_directx11.h"


// Data
static ID3D11Device* g_pd3dDevice = NULL;
static ID3D11DeviceContext* g_pd3dDeviceContext = NULL;
static IDXGISwapChain* g_pSwapChain = NULL;
static ID3D11RenderTargetView* g_mainRenderTargetView = NULL;

// Forward declarations of helper functions
bool CreateDeviceD3D(HWND hWnd);
void CleanupDeviceD3D();
void CreateRenderTarget();
void CleanupRenderTarget();


namespace HelloImGui
{
    void RunnerSdlDirectX11::Impl_InitBackend()
    {
        if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
        {
            std::stringstream msg;
            msg << "RunnerSdlDirectX11::Impl_InitBackend error " << SDL_GetError();
            throw std::runtime_error(msg.str().c_str());
        }
    }

    void RunnerSdlDirectX11::Impl_Select_Gl_Version()
    {
    }

    std::string RunnerSdlDirectX11::Impl_GlslVersion() 
    { 
      return "";
    }

    void RunnerSdlDirectX11::Impl_CreateWindowAndContext()
    {
        const auto &backendWindowParams = params.appWindowParams;
        ImVec2 windowPosition = backendWindowParams.windowPosition;
        ImVec2 windowSize = backendWindowParams.windowSize;

        int xPos = SDL_WINDOWPOS_CENTERED, yPos = SDL_WINDOWPOS_CENTERED;
        if (windowPosition.x >= -10000.f)
        {
            xPos = static_cast<int>(windowPosition.x);
            yPos = static_cast<int>(windowPosition.y);
        }

        // Setup window
        SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
        mWindow = SDL_CreateWindow("Dear ImGui SDL2+DirectX11 example",
                                              SDL_WINDOWPOS_CENTERED,
                                              SDL_WINDOWPOS_CENTERED,
                                              1280,
                                              720,
                                              window_flags);
        SDL_SysWMinfo wmInfo;
        SDL_VERSION(&wmInfo.version);
        SDL_GetWindowWMInfo(mWindow, &wmInfo);
        HWND hwnd = (HWND)wmInfo.info.win.window;


        if (backendWindowParams.fullScreen)
        {
            SDL_SetWindowFullscreen(mWindow, SDL_WINDOW_FULLSCREEN);
        }

        // Initialize Direct3D
        if (!CreateDeviceD3D(hwnd))
        {
            CleanupDeviceD3D();
            throw std::runtime_error(
                "RunnerSdlDirectX11::Impl_CreateWindowAndContext(): Failed CreateDeviceD3D!");
        }
    }

    void RunnerSdlDirectX11::Impl_InitGlLoader()
    {
    }

    void RunnerSdlDirectX11::Impl_SetupPlatformRendererBindings()
    {
        ImGui_ImplSDL2_InitForD3D(mWindow);
        ImGui_ImplDX11_Init(g_pd3dDevice, g_pd3dDeviceContext);
    }

    bool RunnerSdlDirectX11::Impl_PollEvents()
    {
        SDL_Event event;
        bool exitRequired = false;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                exitRequired = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
                event.window.windowID == SDL_GetWindowID(mWindow))
            {
                exitRequired = true;
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED &&
                event.window.windowID == SDL_GetWindowID(mWindow))
            {
                // Release all outstanding references to the swap chain's buffers before resizing.
                CleanupRenderTarget();
                g_pSwapChain->ResizeBuffers(0, 0, 0, DXGI_FORMAT_UNKNOWN, 0);
                CreateRenderTarget();
            }
        }
        return exitRequired;
    }

    void RunnerSdlDirectX11::Impl_NewFrame_3D() 
    { 
      ImGui_ImplDX11_NewFrame(); 
    }

    void RunnerSdlDirectX11::Impl_NewFrame_Backend() 
    { 
      ImGui_ImplSDL2_NewFrame(mWindow); 
    }

    void RunnerSdlDirectX11::Impl_Frame_3D_ClearColor()
    {
    }

    void RunnerSdlDirectX11::Impl_RenderDrawData_To_3D() 
    { 
        g_pd3dDeviceContext->OMSetRenderTargets(1, &g_mainRenderTargetView, NULL);
        g_pd3dDeviceContext->ClearRenderTargetView(g_mainRenderTargetView, 
          (float*)&params.imGuiWindowParams.backgroundColor);
        ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
    }

    void RunnerSdlDirectX11::Impl_UpdateAndRenderAdditionalPlatformWindows()
    {
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
        }
    }

    void RunnerSdlDirectX11::Impl_Cleanup()
    {
        ImGui_ImplDX11_Shutdown();
        ImGui_ImplSDL2_Shutdown();
        ImGui::DestroyContext();

        CleanupDeviceD3D();
        SDL_DestroyWindow(mWindow);
        SDL_Quit();
    }

    void RunnerSdlDirectX11::Impl_SwapBuffers() 
    { 
        g_pSwapChain->Present(1, 0);  // Present with vsync
        // g_pSwapChain->Present(0, 0); // Present without vsync
    }

}  // namespace HelloImGui

#endif  // #ifdef HELLOIMGUI_USE_SDL_DIRECTX11
