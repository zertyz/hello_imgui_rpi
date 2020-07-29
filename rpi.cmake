# This script does the following:
#	- Replaces the need of Microsoft's vcpkg cmake toolchain file -- which is tricky on the rPi
#	- Sets the configs needed to compile to Raspberry Pi's 0, 1, 2 & 3 using the closed source Broadcom Drivers (with SDL2 installed and compiled to also use them)
#
# TO CREATE THE MAKEFILES FOR THE RASPBERRY PI (tested with the broadcom drivers and SDL2 compiled to use them):
# mkdir build; cd build; cmake -DCMAKE_TOOLCHAIN_FILE=../rpi.cmake -DCMAKE_BUILD_TYPE=Release -DHELLOIMGUI_USE_SDL_OPENGL3=ON .. | less -# 1; find -name link.txt -exec sed -i -s "s|-lSDL2::SDL2-static|`pkg-config --static --libs sdl2`|" "{}" \; -exec sed -i -s 's|-static ||' "{}" \; -exec sed -i -s 's|/usr/lib/libSDL2main.a ||' "{}" \; -ls; make VERBOSE=1; find -type f -executable -ls
#
# To create the git patch files,
# cd external/imgui; git diff >../../external.imgui.rPi.patch; cd -; git diff >hello_imgui.rPi.patch
# To apply them (provide you copy those 3 + this file),
# cd external/imgui; git apply ../../external.imgui.rPi.patch; cd -; git apply hello_imgui.rPi.patch


# Mark variables as used so cmake doesn't complain about them
mark_as_advanced(CMAKE_TOOLCHAIN_FILE)

# compilation flags for the Raspberry Pi
add_compile_definitions(__RPI0123_BROADCOM__)
add_compile_definitions(IMGUI_IMPL_OPENGL_ES2)

# SDL2 package	-- on vcpkg there is a libSDL2.a, while on ArchLinux we don't. No easy fix since SDL2-static is used on the CMakeFiles.txt throughout the project
# woraround: after running cmake, the 'link.txt' files have to be tweeked before running 'make'. See the docs on the beginning of this file
find_package(SDL2 REQUIRED)
if (SDL2_FOUND)
	message(STATUS ${SDL2_LIBRARY} " SDL2 Library has been found!")
	include_directories(${SDL2_INCLUDE_DIR})
else()
      message(FATAL_ERROR "Cannot find SDL2 Library!")
endif()
