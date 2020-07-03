#include "hello_imgui/internal/menu_statusbar.h"
#include "hello_imgui/internal/docking_details.h"
#include "imgui.h"
#include "imgui_internal.h"
#include <string>

namespace HelloImGui
{
namespace Menu_StatusBar
{

void ShowDefaultAppMenu_Quit(RunnerParams & runnerParams)
{
    std::string appName = runnerParams.appWindowParams.windowTitle.c_str();
    if (appName.empty())
        appName = "App";

    auto& dockableWindows = runnerParams.dockingParams.dockableWindows;

    bool hideMenuBecauseEmpty = false;
#ifdef HELLOIMGUI_CANNOTQUIT
    hideMenuBecauseEmpty = true;
#endif
    if (hideMenuBecauseEmpty)
        return;

    if (ImGui::BeginMenu(appName.c_str()))
    {
#ifndef HELLOIMGUI_CANNOTQUIT
        if (ImGui::MenuItem( "Quit"))
            runnerParams.appShallExit = true;
#endif
        ImGui::EndMenu();
    }

}

void ShowMenu(RunnerParams & runnerParams)
{
    bool hasMenu = ImGui::GetCurrentWindow()->Flags & ImGuiWindowFlags_MenuBar;
    if (!hasMenu)
        return;

    ImGui::BeginMenuBar();

    if (runnerParams.imGuiWindowParams.showMenu_App_QuitAbout)
        ShowDefaultAppMenu_Quit(runnerParams);

    if (runnerParams.imGuiWindowParams.showMenu_View)
        DockingDetails::ShowViewMenu(runnerParams);

    if (runnerParams.callbacks.ShowMenus)
        runnerParams.callbacks.ShowMenus();

    ImGui::EndMenuBar();
}

void ShowStatus_Fps()
{
    ImGui::SameLine(ImGui::GetIO().DisplaySize.x - 90.f);
    ImGui::Text("FPS: %.1f", ImGui::GetIO().Framerate);
}

void ShowStatusBar(const RunnerParams & params)
{
    float statusWindowHeight = 30.f;
    ImVec2 windowLocation( 0., ImGui::GetIO().DisplaySize.y - statusWindowHeight );
    ImVec2 windowSize(ImGui::GetIO().DisplaySize.x, statusWindowHeight);
    ImGui::SetNextWindowPos(windowLocation);
    ImGui::SetNextWindowSize(windowSize);
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking;
    ImGui::Begin("StatusBar", nullptr, windowFlags);

    if (params.callbacks.ShowStatus)
        params.callbacks.ShowStatus();

    if (params.imGuiWindowParams.showStatus_Fps)
        ShowStatus_Fps();

    ImGui::End();
}


}  // namespace Menu_StatusBar
}  // namespace HelloImGui
