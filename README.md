# KWin Effect Shaders
KDE Plasma plugin aimed at adding post processing effects to video games using GLSL shaders.\
Example of FakeHDR and Adaptive Sharpen in Life is Strange 2:
![Comparison](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/comparison.png)
## Usage
- [Install the required dependencies.](#dependencies)
- [Install the plugin.](#installing)
- [Acquire the shader files.](#acquiring-the-shader-files)
- Log out and in of Plasma.
- [Modify the shader settings.](#modifying-the-shader-settings)
- [Enable the effect by using the keyboard shortcut.](#keyboard-shortcuts)
## Requirements
KWin compositing with GLSL 1.40 or higher for OpenGL or GLSL 3.0 or higher for OpenGL ES.
## Dependencies
- Arch Based (untested):

      sudo pacman -Syu extra-cmake-modules kwin kf5
- Debian Based (untested):

      sudo apt install git g++ cmake extra-cmake-modules libkf5kio-dev libkf5globalaccel-dev libkf5i18n-dev kwin-dev gettext qttools5-dev kinit-dev kwin-de
- Fedora Based (untested):

      sudo dnf install git gcc-c++ cmake extra-cmake-modules kf5-kio-devel kf5-kglobalaccel-devel kf5-ki18n-devel kwin-devel qt5-qttools-devel qt5-qttools-static qt5-qtbase-devel
- OpenSUSE Based (tested):

      sudo zypper in git gcc-c++ cmake extra-cmake-modules kio-devel kglobalaccel-devel ki18n-devel kwin5-devel
## Installing
NOTE: After installing, log out and in of the session to load the plugin.\
If the plugin does not load, see [Enabling or Disabling The Plugin](#enabling-or-disabling-the-plugin).
### From Source:
    git clone https://github.com/kevinlekiller/kwin-effect-shaders
    cd kwin-effect-shaders
    # Use the install.sh script:
    ./install.sh
### Arch (AUR):
https://aur.archlinux.org/packages/kwin-effect-shaders-git
## Uninstalling
    ./install.sh UNINSTALL
## Acquiring The Shader Files
NOTE: If you have ran the `install.sh` script, everything within this section is done automatically.

Clone the shader files to somewhere in your home directory:

    git clone https://github.com/kevinlekiller/kwin-effect-shaders_shaders ~/.local/share/kwin-effect-shaders_shaders

If you use the aforementioned default location for the shaders, it will be automatically detected.\
If you use a custom location, set the `Shader Path`:\
By modifying the configuration file at `~/.config/kevinlekiller/kwin_effect_shaders.conf`\
Or by setting it in the [configuration UI](#gui).
## Keyboard Shortcuts
NOTE: The keyboard shortcuts might be disabled by default.

To change them:\
`System Settings -> Workspace -> Shortcuts -> Shortcuts -> System Services -> KWin`

![Modifying keyboard shortcuts](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/keyboard_shortcuts.png)
## Modifying The Shader Settings
### GUI
https://github.com/kevinlekiller/kwin-effect-shaders_gui
### Manually
Open the `1_settings.glsl` file in a text editor. By default `1_settings.glsl` is in `~/.local/share/kwin-effect-shaders_shaders`

![Shader Settings](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shader_settings.png)
## Enabling or Disabling The Plugin
To enable or disable the plugin:\
`System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders`\
Toggle the checkmark in the box and click `Apply`.

![Toggling the plugin](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/toggle_plugin.png)
## Notes
Note that X11 disables compositing in full screen applications, use Wayland if possible.

This project was initially created because some games do not support vkBasalt or ReShade.

This plugin is a fork of these projects: [kwin-effect-smart-invert](https://github.com/natask/kwin-effect-smart-invert), [invert](https://github.com/KDE/kwin/tree/master/src/effects/invert), [kwin-effects-yet-another-magic-lamp](https://github.com/zzag/kwin-effects-yet-another-magic-lamp).

I'm not an expert on OpenGL or glsl, if you are, PR's are welcome.

Antialiasing alternatives:\
If you have a AMD gpu, see the [gallium documentation.](https://docs.mesa3d.org/gallium/postprocess.html#current-filters)\
ex. on Steam game: `pp_jimenezmlaa_color=32 %command%`
## TODO
Try to have each shader run on its own, which would solve some issues. -> Currently my knowledge of OpenGL is limited, from what I can understand, this requires creating / using frame and vertex buffer objects. See [issue #3](https://github.com/kevinlekiller/kwin-effect-shaders/issues/3).

Add some kind of profile system, where a user could have different settings for different games.
## Screenshots
- [CAS + Tonemap](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=9f8f759e-b2e4-11ec-b5bb-6595d9b17862)
- [Debanding](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=ae7aca12-941d-11ec-a554-13fc6baea232)
- [Debanding](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=ff817972-92b2-11ec-a554-13fc6baea232)
- [FakeHDR](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=07c3c256-92b4-11ec-a554-13fc6baea232)
- [Levels](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=5fb083ae-92b5-11ec-a554-13fc6baea232)
