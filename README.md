# KWin Effect Shaders
KDE Plasma plugin aimed at adding post processing effects to video games using GLSL shaders.

Example of FakeHDR and Adaptive Sharpen in Life is Strange 2:

![Comparison](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/comparison.png)

## Index
- [Requirements](#requirements)
- [Dependencies](#dependencies)
- [Installing](#installing)
- [Uninstalling](#uninstalling)
- [Acquiring The Shader Files](#acquiring-the-shader-files)
- [Keyboard Shortcuts](#keyboard-shortcuts)
- [Modifying The Shader Settings](#modifying-the-shader-settings)
- [Blacklisting Applications](#blacklisting-applications)
- [Whitelisting Applications](#whitelisting-applications)
- [Finding Application Names](#finding-application-names)
- [Enabling On Login](#enabling-on-login)
- [Enabling or Disabling The Plugin](#enabling-or-disabling-the-plugin)
- [Notes](#notes)
- [TODO](#todo)
- [Screenshots](#screenshots)
## Requirements
KWin compositing with GLSL 1.40 or higher for OpenGL or GLSL 3.0 or higher for OpenGL ES.

Currently, for the GUI, Wayland is required. See [issue #4](https://github.com/kevinlekiller/kwin-effect-shaders/issues/4).
## Dependencies
- Arch Based (untested):

`sudo pacman -Syu extra-cmake-modules kwin kf5`
- Debian Based (untested):

`sudo apt install git g++ cmake extra-cmake-modules libkf5kio-dev libkf5globalaccel-dev libkf5i18n-dev kwin-dev gettext qttools5-dev kinit-dev kwin-de`
- Fedora Based (untested):

`sudo dnf install git gcc-c++ cmake extra-cmake-modules kf5-kio-devel kf5-kglobalaccel-devel kf5-ki18n-devel kwin-devel qt5-qttools-devel qt5-qttools-static qt5-qtbase-devel`
- OpenSUSE Based (tested):

`sudo zypper in git gcc-c++ cmake extra-cmake-modules kio-devel kglobalaccel-devel ki18n-devel kwin5-devel`

## Installing
NOTE: After installing, log out and in of the session to load the plugin. If the plugin does not load, see [Enabling or Disabling The Plugin](#enabling-or-disabling-the-plugin).
### From Source:
    git clone https://github.com/kevinlekiller/kwin-effect-shaders
    cd kwin-effect-shader
    # Use the install.sh script:
    ./install.sh

### Arch (AUR):
https://aur.archlinux.org/packages/kwin-effect-shaders-git

## Uninstalling
    ./install.sh UNINSTALL

## Acquiring The Shader Files
If you have ran the `install.sh` script, everything within this section is done automatically.

Clone the shader files to somewhere in your home directory:

    git clone https://github.com/kevinlekiller/kwin-effect-shaders_shaders ~/.local/share/kwin-effect-shaders_shaders

If you use the aforementioned default location for the shaders, it will be automatically detected.

If you use a custom location, set the `Shader Path` by opening the configuration UI (see [Keyboard Shortcuts](#keyboard-shortcuts)).

![Set a custom shader path](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shader_path.png)

## Keyboard Shortcuts
NOTE: The keyboard shortcuts might be disabled by default.

"Shaders Effect: Opens the configuration UI" ; This is set to `CTRL + META + X` by default.

"Shaders Effect: Toggle On or Off the effect" ; This is set to `CTRL + META + Z` by default.

To change these:

`System Settings -> Workspace -> Shortcuts -> Shortcuts -> System Services -> Kwin`

![Modifying keyboard shortcuts](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/keyboard_shortcuts.png)

## Modifying The Shader Settings
Open the configuration UI (see [Keyboard Shortcuts](#keyboard-shortcuts)), go to the `Shaders` tab.

Click on the shader you want to enable, click `Apply`. Click `Save` if you want to write the changes to the settings file.

You can also enable `Auto Apply` in the `Settings` tab, which will automatically apply the settings.

![Shaders tab](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shader_configuration.png)

## Blacklisting Applications
In the configuration UI, in the `Settings` tab, you can add application(s), if more than 1, seperate them with a comma.

For example: `plasmashell,Firefox`

This will block the Plasma desktop and Firefox from being processed.

The list is not case sensitive.

## Whitelisting Applications
In the configuration UI, in the `Settings` tab, you can add application(s), if more than 1, seperate them with a comma.

For example: `kate,kcalc`

Only Kcalc and Kate will be processed.

The list is not case sensitive.

This is useful if you use the `Enabled by Default` option.

You can use the `Blacklist` at the same time as the `Whitelist`.

## Finding Application Names
To find an application name to use in the blacklist or whitelist, follow these steps:

`System Settings -> Window Management -> Window Rules -> Add New...`

If you're in a game, set it to windowed mode.

Click `Detect Window Properties`, click the application.

The application name is first string in the `Whole window class` (delimited by the space character).

For example, with kate, the "Whole window class" is `kate org.kde.kate`, so the name to put in the `Blacklist` or `Whitelist` would be kate.

Another example, lutris, the whole window class is `lutris` so the name would be lutris.

![Find the application name](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/find_application_name.png)

## Enabling On Login
In the configuration UI, in the `Settings` tab, you can set the `Enabled By Default` option.

This will process all applications on login.

You can exclude applications by using the `Blacklist` and/or `Whitelist`.

## Enabling or Disabling The Plugin
To enable or disable the plugin:

`System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders`

Toggle the checkmark in the box and click `Apply`.

![Toggling the plugin](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/toggle_plugin.png)

## Notes
Note that X11 disables compositing in full screen applications, use Wayland if possible.

This project was initially created because some games do not support vkBasalt or ReShade.

This plugin is a fork of these projects: [kwin-effect-smart-invert](https://github.com/natask/kwin-effect-smart-invert), [invert](https://github.com/KDE/kwin/tree/master/src/effects/invert), [kwin-effects-yet-another-magic-lamp](https://github.com/zzag/kwin-effects-yet-another-magic-lamp).

I'm not an expert on OpenGL or glsl, if you are, PR's are welcome.

Antialiasing alternatives:

If you have a AMD  gpu, see https://docs.mesa3d.org/gallium/postprocess.html#current-filters

ex. on Steam game: `pp_jimenezmlaa_color=32 %command%`

## TODO
Try to have each shader run on its own, which would solve some issues. -> Currently my knowledge of OpenGL is limited, from what I can understand, this requires creating / using frame and vertex buffer objects. See [issue #3](https://github.com/kevinlekiller/kwin-effect-shaders/issues/3).

Add some kind of profile system, where a user could have different settings for different games.

## Screenshots
[CAS + Tonemap](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=9f8f759e-b2e4-11ec-b5bb-6595d9b17862)

[Debanding](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=ae7aca12-941d-11ec-a554-13fc6baea232)

[Debanding](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=ff817972-92b2-11ec-a554-13fc6baea232)

[FakeHDR](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=07c3c256-92b4-11ec-a554-13fc6baea232)

[Levels](https://cdn.knightlab.com/libs/juxtapose/latest/embed/index.html?uid=5fb083ae-92b5-11ec-a554-13fc6baea232)
