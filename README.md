# KWin Shader Effects
KDE Plasma / KWin desktop effect aimed at adding post processing effects to video games using GLSL shaders.

Example of FakeHDR + Adaptive Sharpen in Life is Strange 2:  
![Comparison](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/comparison.png)

Example of Vibrance and Levels in KDE Plasma's System Settings:
![Shaders](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shaders.png)
## Index
- [Requirements](#requirements)
- [Installing](#installing)
- [Uninstalling](#uninstalling)
- [Acquiring The Shader Files](#acquiring-the-shader-files)
- [Enabling The KWin Desktop Effect](#enabling-the-kwin-desktop-effect)
- [Keyboard Shortcuts](#keyboard-shortcuts)
- [Modifying The Shader Settings](#modifying-the-shader-settings)
- [Blacklisting Applications](#blacklisting-applications)
- [Whitelisting Applications](#whitelisting-applications)
- [Finding Application Names](#finding-application-names)
- [Enabling On Login](#enabling-on-login)
- [Notes](#notes)
- [TODO](#todo)
- [Screenshots](#screenshots)
## Requirements
- Arch Based (untested):

`sudo pacman -Syu extra-cmake-modules kwin kf5`
- Debian Based (untested):

`sudo apt install git cmake g++ gettext extra-cmake-modules qttools5-dev libkf5globalaccel-dev libkf5kio-dev kinit-dev kwin-de`
- Fedora Based (untested):

`sudo dnf install git cmake gcc-c++ extra-cmake-modules qt5-qttools-devel qt5-qttools-static kf5-kguiaddons-devel kf5-kglobalaccel-devel kf5-kio-devel kf5-ki18n-devel kwin-devel qt5-qtbase-devel`
- OpenSUSE Based (tested):

`sudo zypper in -t pattern devel_basis && sudo zypper in cmake extra-cmake-modules kguiaddons-devel kio-devel kwin5-devel kglobalaccel-devel ki18n-devel`

## Installing
After installing, log out and in of the session to load the effect, or restart kwin.
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

Set the `Shader Path` setting in `System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders` (there's a button to the right you can click to change the settings) to the directory you copied the shaders to (use the real path).

For example `/home/kevin/.local/share/kwin-effect-shaders_shaders`

![Shaders path directory](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shader_path.png)

## Enabling The KWin Desktop Effect
`System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders`

Put a checkmark in the box and click accept.

By default all shaders are disabled, see `Modifying The Shader Settings`.

![Desktop Effects](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/settings1.png)

## Keyboard Shortcuts
NOTE: The keyboard shortcuts might be disabled by default.

"Shaders Effect: Toggle Shaders Effect On Current Window" ; This is set to `CTRL + META + Z` by default.

"Shaders Effect: Toggle Shaders Effect On All Windows"    ; This is set to `CTRL + META + X` by default.

To change these:

`System Settings -> Workspace -> Shortcuts -> Shortcuts -> System Services -> Kwin`

![Keyboard Shortcuts](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/keyboard_shortcuts.png)

## Modifying The Shader Settings
Go to the directory where your shader files are, by default `~/.local/share/kwin-effect-shaders_shaders`

Copy `1_settings.glsl.example` to `1_settings.glsl` : `cp 1_settings.glsl.example 1_settings.glsl`

Modify the `1_settings.glsl` file.

Modifying the file while shaders are enabled will automatically reload the settings.

For example, to enable the Adaptive Sharpen shader:

![1_settings.glsl edit](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/shader_settings.png)

## Blacklisting Applications
In the settings for the effect (`System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders`), you can add application(s), if more than 1, seperate them with a comma.

For example: `plasmashell,Firefox`

This will block the Plasma desktop and Firefox from being processed.

The list is not case sensitive.

## Whitelisting Applications
In the settings for the effect (`System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders`), you can add application(s), if more than 1, seperate them with a comma.

For example: `kate,kcalc`

Only Kcalc and Kate will be processed.

The list is not case sensitive.

This is useful if you use the Enabled by Default option.

You can use the Blacklist at the same time as the Whitelist.

## Finding Application Names
To find an application name to use in the blacklist or whitelist, follow these steps:

`System Settings -> Window Management -> Window Rules -> Add New...`

If you're in a game, set it to windowed mode.

Click `Detect Window Properties`, click the application.

The application name is first string in the `Whole window class` (delimited by the space character).

For example, with kate, the "Whole window class" is `kate org.kde.kate`, so the name to put in the blacklist or whitelist would be kate.

Another example, lutris, the whole window class is `lutris` so the name would be lutris.

![find_application_name](https://github.com/kevinlekiller/kwin-effect-shaders/raw/main/images/find_application_name.png)

## Enabling On Login
In the settings for the effect (`System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders`), you can set the "Enabled By Default" option.

This will process all applications on login.

You can exclude applications by using the Blacklist and/or Whitelist.

## Notes
Note that X11 disables compositing in full screen applications, use Wayland if possible.

This project was initially created because some games do not support vkBasalt or ReShade.

This desktop effect is a fork of these projects: [kwin-effect-smart-invert](https://github.com/natask/kwin-effect-smart-invert), [invert](https://github.com/KDE/kwin/tree/master/src/effects/invert), [kwin-effects-yet-another-magic-lamp](https://github.com/zzag/kwin-effects-yet-another-magic-lamp).

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
