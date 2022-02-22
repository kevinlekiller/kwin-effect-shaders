# KWin Shader Effects
## Intro
KDE Plasma / KWin desktop effect aimed at adding post processing effects to video games.

While this might support X11 (not tested), it requires KWin to be compositing, and compositing is usually disabled when playing a game on X11.

This project was initially created because some games do not support vkBasalt or ReShade.

This desktop effect is a fork of these projects: [kwin-effect-smart-invert](https://github.com/natask/kwin-effect-smart-invert), [invert](https://github.com/KDE/kwin/tree/master/src/effects/invert) [kwin-effects-yet-another-magic-lamp](https://github.com/zzag/kwin-effects-yet-another-magic-lamp)

Most of the included shaders are from libretro / mpv / SweetFX / vkBasalt, you can find the license for those shaders in the shader file (src/shaders/shader.frag).

I'm not an expert on OpenGL or glsl, if you are, if you can, please optimize the shaders, or add more.

## Requirements (not tested)
Arch Based: `sudo pacman -Syu extra-cmake-modules kwin kf5`

Debian Based: `sudo apt install git cmake g++ gettext extra-cmake-modules qttools5-dev libqt5x11extras5-dev libkf5configwidgets-dev libkf5crash-dev libkf5globalaccel-dev libkf5kio-dev libkf5notifications-dev kinit-dev kwin-de`

Fedora Based: `sudo dnf install git cmake gcc-c++ extra-cmake-modules qt5-qttools-devel qt5-qttools-static qt5-qtx11extras-devel kf5-kconfigwidgets-devel kf5-kcrash-devel kf5-kguiaddons-devel kf5-kglobalaccel-devel kf5-kio-devel kf5-ki18n-devel kf5-knotifications-devel kf5-kinit-devel kwin-devel qt5-qtbase-devel libepoxy-devel`

## Installing

After installing, log out / in of the session to load the effect.

### From source:

`git clone https://github.com/kevinlekiller/kwin-effect-shaders`

`cd kwin-effect-shader`

`# Use the install.sh script:`

`./install.sh`

### Arch (AUR):

https://aur.archlinux.org/packages/kwin-effect-shaders-git

## Uninstall

`./install.sh UNINSTALL`

## Enabling / Disabling the effect

System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders

Put a checkmark in the box and click accept.

By default all shaders are disabled, see "Modifying the shader files".

![Desktop Effects](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/settings1.png)

## Custom Keyboard shortcuts

System Settings -> Workspace -> Shortcuts -> Shortcuts -> System Services -> Kwin -> Toggle Shaders Effect

![Keyboard Shortcuts](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shortcuts.png)

## Modifying the shader files

Copy the shader files to somewhere in your home directory:

`cp -r /usr/share/kwin/shaders ~/.local/share/`

Set the `Shader Path` setting in `System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders` (there's a button to the right you can click to change the settings) to the directory you copied the shaders to (use the real path).

For example `/home/kevin/.local/share/shaders`

Modify the `shaders.frag` file in that directory.

![Shaders path directory](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/settings2.png)

![shader.frag edit](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shader_file.png)

## Toggling on/off the shaders

The default shortcuts are (META is the SUPER key):

The shortcuts might be disabled by default, see "Custom Keyboard Shortcuts".

Toggle on/off active application (this is what you probably want to use): `CTRL + META + Z`

You can use this on any program, not just a game, the "System Settings" window for example.

Toggle on/off whole screen (all applications): `CTRL + META + R`

## TODO

Get the blacklist working.

Get the vertex shader working (all it does currently is remove window decorations).

Implement a whitelist system, where a program which matches the whitelist the shader processing will be automatically enabled.

Seperate shaders into their own files, instead of all in 1 file.

Add more shaders.

## Screenshots

[Debanding](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=ae7aca12-941d-11ec-a554-13fc6baea232)

[Debanding](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=ff817972-92b2-11ec-a554-13fc6baea232)

[FakeHDR](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=07c3c256-92b4-11ec-a554-13fc6baea232)

[Levels](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=5fb083ae-92b5-11ec-a554-13fc6baea232)
