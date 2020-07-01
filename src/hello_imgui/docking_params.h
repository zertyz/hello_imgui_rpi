#pragma once
#include "imgui.h"
#include "hello_imgui/runner_callbacks.h"
#include <functional>
#include <string>
#include <vector>
#include <utility>
#include <stdio.h>

namespace HelloImGui
{
/**
@@md#DockingExample
**Docking params: Example usage**

````cpp
HelloImGui::RunnerParams runnerParams;
runnerParams.imGuiWindowParams.defaultImGuiWindowType =
    HelloImGui::DefaultImGuiWindowType::ProvideFullScreenDockSpace;

runnerParams.dockingParams.dockingSplits =
{
    // First, add a bottom space whose height is 25% of the app height
    // This will split the preexisting default dockspace "MainDockSpace"
    // in two parts.
    { "MainDockSpace", "BottomSpace", ImGuiDir_Down, 0.25 },
    // Then, add a space to the left which occupies a column
    // whose width is 25% of the app height
    { "MainDockSpace", "LeftSpace", ImGuiDir_Left, 0.25 }
    // We now have three spaces: "MainDockSpace", "BottomSpace", and "LeftSpace"
};
runnerParams.dockingParams.dockableWindows =
{
    // A Window named "Main" will be placed in "MainDockSpace".
    // Its Gui is provided by the VoidFunction "MainGui"
    {"Main", "MainDockSpace", MainGui},
    // A Log  window named "Logs" will be placed in "BottomSpace".
    // Its Gui is provided by the VoidFunction "ShowLogs"
    {"Logs", "BottomSpace", ShowLogs},
    // A Command panel named "Commands" will be placed in "LeftSpace".
    // Its Gui is provided by the VoidFunction "ShowCommandsPanel"
    {"Commands", "LeftSpace", ShowCommandsPanel}
};

runnerParams.imGuiWindowParams.showMenuBar = true;
runnerParams.imGuiWindowParams.showStatusBar = true;

HelloImGui::Run(runnerParams);
````
@@md
*/

/**
@@md
A DockSpaceName is a simple string that identifies a zone on the screen where windows can be docked.
@@md
*/
using DockSpaceName = std::string;

/**
@@md#DockingSplit
 **DockingSplit** is a struct that defines the way the docking splits should be applied on the screen
 in order to create new Dock Spaces. _DockingParams_ contains a _vector[DockingSplit]_,
 in order to partition the screen at your will.

_Members:_

* `initialDock`: _DockSpaceName (aka string)_

    id of the space that should be split.
    At the start, there is only one Dock Space named "MainDockSpace".
    You should start by partitioning this space, in order to create a new dock space.

* `newDock`: _DockSpaceName (aka string)_. id of the new dock space that will be created
* `direction`: *ImGuiDir_ (enum with ImGuiDir_Down, ImGuiDir_Down, ImGuiDir_Left, ImGuiDir_Right)*.
Direction where this dock space should be created
* `ratio`: _float, default=0.25f_. Ratio of the initialDock size that should be used by the new dock space
@@md
*/
struct DockingSplit
{
    DockSpaceName initialDock;
    DockSpaceName newDock;
    ImGuiDir_ direction;
    float ratio = 0.25f;
};

/**
@@md#DockableWindow
 **DockableWindow** is a struct that represents a window that can be docked.

_Members:_

* `label`: _string_. Title of the window.
* `dockSpaceName`: _DockSpaceName (aka string)_. Id of the dock space where this window
   should initialy be placed
* `GuiFunction`: _VoidFuntion_. Any function that will render this window's Gui.
* `isVisible`: _bool, default=true_. Flag that indicates whether this window is visible or not.
* `canBeClosed`: _bool, default=true_. Flag that indicates whether the user can close this window.
* `callBeginEnd`: _bool, default=true_. Flag that indicates whether ImGui::Begin and ImGui::End
   calls should be added automatically (with the given "label"). Set to false if you want to call
   ImGui::Begin/End yourself
* `isAboutWindow`: _bool, default=false_. Flag that indicate whether this window should be opened
   when the user clicks the menu item "About". The menu item "About" will appear only if one dockable
   window is marked with this flag.
@@md
**/
struct DockableWindow
{
    DockableWindow(
        const std::string & label_ = "",
        const DockSpaceName & dockSpaceName_ = "",
        const VoidFunction guiFonction_ = {},
        bool isVisible_ = true,
        bool canBeClosed_ = true)
    : label(label_), dockSpaceName(dockSpaceName_),
          GuiFonction(guiFonction_),
      isVisible(isVisible_),
          canBeClosed(canBeClosed_) {}

    std::string label;

    DockSpaceName dockSpaceName;

    VoidFunction GuiFonction = {};

    bool isVisible = true;
    bool canBeClosed = true;
    bool callBeginEnd = true;
    bool isAboutWindow = false;
};

/**
@@md#DockingParams
 **DockingParams** contains all the settings concerning the docking, 
 together _with the Gui functions for the docked windows_.

 _Members:_

* `dockingSplits`: _vector[DockingSplit]_.
  Defines the way docking splits should be applied on the screen in order to create new Dock Spaces
* `dockableWindows`: _vector[DockableWindow]_.
  List of the dockable windows, together with their Gui code
* `resetUserDockLayout`: _bool, default=true_.
  Reset user layout at application startup
@@md
 */
struct DockingParams
{
    std::vector<DockingSplit> dockingSplits;

    std::vector<DockableWindow> dockableWindows;

    bool resetUserDockLayout = true;

    // wasDockLayoutApplied is an internal variable
    bool wasDockLayoutApplied = false;
};
} // namespace HelloImGui
