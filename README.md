# KWin Shader Effects
KDE Plasma / KWin desktop effect aimed at adding post processing effects to video games using GLSL shaders.

Example of FakeHDR + Adaptive Sharpen in Life is Strange 2:  
![Comparison](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/comparison.png)

Example of Vibrance and Levels in KDE Plasma's System Settings:
![Shaders](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shaders.png)
## Index
- [Requirements](#requirements-not-tested)
- [Install](#install)
- [Uninstall](#uninstall)
- [Toggling The KWin Effect](#toggling-the-kwin-effect)
- [Custom Keyboard Shortcuts](#custom-keyboard-shortcuts)
- [Copying Shaders To Home Directory](#copying-shaders-to-home-directory)
- [Modifying Shader Settings](#modifying-shader-settings)
- [Toggling The Shaders](#toggling-the-shaders)
- [Blacklist Applications](#blacklist-applications)
- [Whitelist Applications](#whitelist-applications)
- [Enable On Login](#enable-on-login)
- [Notes](#notes)
- [TODO](#todo)
- [Screenshots](#screenshots)
## Requirements (not tested)
- Arch Based:

`sudo pacman -Syu extra-cmake-modules kwin kf5`
- Debian Based:

`sudo apt install git cmake g++ gettext extra-cmake-modules qttools5-dev libqt5x11extras5-dev libkf5configwidgets-dev libkf5crash-dev libkf5globalaccel-dev libkf5kio-dev libkf5notifications-dev kinit-dev kwin-de`
- Fedora Based:

`sudo dnf install git cmake gcc-c++ extra-cmake-modules qt5-qttools-devel qt5-qttools-static qt5-qtx11extras-devel kf5-kconfigwidgets-devel kf5-kcrash-devel kf5-kguiaddons-devel kf5-kglobalaccel-devel kf5-kio-devel kf5-ki18n-devel kf5-knotifications-devel kf5-kinit-devel kwin-devel qt5-qtbase-devel libepoxy-devel`

## Install
After installing, log out and in of the session to load the effect, or restart kwin.
### From Source:
    git clone https://github.com/kevinlekiller/kwin-effect-shaders
    cd kwin-effect-shader
    # Use the install.sh script:
    ./install.sh

### Arch (AUR):
https://aur.archlinux.org/packages/kwin-effect-shaders-git

## Uninstall
    ./install.sh UNINSTALL

## Toggling The KWin Effect
`System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders`  
Put a checkmark in the box and click accept.  
By default all shaders are disabled, see `Modifying Shader Settings`.

![Desktop Effects](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/settings1.png)

## Custom Keyboard Shortcuts
`System Settings -> Workspace -> Shortcuts -> Shortcuts -> System Services -> Kwin -> Toggle Shaders Effect`

![Keyboard Shortcuts](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shortcuts.png)

## Copying Shaders To Home Directory
If you have ran the `install.sh` script, the following steps are done automatically.  
Copy the shader files to somewhere in your home directory:

    cp -r /usr/share/kwin/shaders ~/.local/share/  
Set the `Shader Path` setting in `System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders` (there's a button to the right you can click to change the settings) to the directory you copied the shaders to (use the real path). 
For example `/home/kevin/.local/share/shaders`

![Shaders path directory](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/settings2.png)

## Modifying Shader Settings
Go to the directory where your shader files are, by default `~/.local/share/shaders`  
Modify the `1_settings.glsl` file in that directory.

If shader(s) are enabled, Modifying the settings will trigger the shader(s) to be reloaded.

For example, to enable Adaptive sharpen:

![1_settings.glsl edit](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shader_file.png)

## Toggling The Shaders
The default shortcuts are (META is the SUPER key):  
The shortcuts might be disabled by default, see "Custom Keyboard Shortcuts".  
Toggle on and off active application (this is what you probably want to use): `CTRL + META + Z`  
You can use this on any program, not just a game, the "System Settings" window for example.  
Toggle on and off whole screen (all applications): `CTRL + META + R`

## Blacklist Applications
In the settings for the effect (`System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders`), you can add application(s), if more than 1, seperate them with a comma.  
For example: `plasmashell,Firefox`  
This will block the Plasma desktop and Firefox from being processed.  
The list is not case sensitive.

## Whitelist Applications
In the settings for the effect (`System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders`), you can add application(s), if more than 1, seperate them with a comma.  
For example: `kate,kcalc`  
Only Kcalc and Kate will be processed.  
The list is not case sensitive.  
This is useful if you use the Enabled by Default option.  
You can use the Blacklist at the same time as the Whitelist.

## Enable On Login
In the settings for the effect (`System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders`), you can set the "Enabled By Default" option.  
This will process all applications on login.  
You can exclude applications by using the Blacklist and/or Whitelist.

## Notes
Note that X11 disables compositing in full screen applications, use Wayland if possible.  
This project was initially created because some games do not support vkBasalt or ReShade.  
This desktop effect is a fork of these projects: [kwin-effect-smart-invert](https://github.com/natask/kwin-effect-smart-invert), [invert](https://github.com/KDE/kwin/tree/master/src/effects/invert), [kwin-effects-yet-another-magic-lamp](https://github.com/zzag/kwin-effects-yet-another-magic-lamp).  
Most of the included shaders are from libretro, mpv, SweetFX, vkBasalt.  
You can find the licenses for those shaders in the shader files (src/shaders/).  
I'm not an expert on OpenGL or glsl, if you are, PR's are welcome.  
Antialiasing alternatives:  
If you have a AMD  gpu, see https://docs.mesa3d.org/gallium/postprocess.html#current-filters  
ex. on Steam game: `pp_jimenezmlaa_color=32 %command%`

## TODO
Add more shaders.

## Screenshots
[Debanding](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=ae7aca12-941d-11ec-a554-13fc6baea232)  
[Debanding](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=ff817972-92b2-11ec-a554-13fc6baea232)  
[FakeHDR](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=07c3c256-92b4-11ec-a554-13fc6baea232)  
[Levels](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=5fb083ae-92b5-11ec-a554-13fc6baea232)  
