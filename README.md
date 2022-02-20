# KWin Shader Effects
## Intro
KDE Plasma / KWin desktop effect aimed at adding post processing effects to video games.

While this might support X11 (not tested), it requires KWin to be compositing, and compositing is usually disabled when playing a game on X11.

This project was initially created because some games do not support vkBasalt or ReShade.

This desktop effect is a fork of these projects: https://github.com/natask/kwin-effect-smart-invert https://github.com/KDE/kwin/tree/master/src/effects/invert

Most of the included shaders included are from libretro / mpv / SweetFX, you can find the license for those shaders in the shader file (data/shader.frag).

I'm not an expert on OpenGL or glsl, if you are, if you can, please optimize the shaders, or add more.

This was written in a few hours, so it's not very cleanly done, ideally there would be support for OpenGL's version 110 to be more compatible, also ideally the shaders should be split into individual files ; Maybe a project for me or someone in the future.

Also, more shortcuts for enabling / disabling effects instead of editing shaders.frag

## Requirements
See https://github.com/natask/kwin-effect-smart-invert#dependencies

## Building
git clone https://github.com/kevinlekiller/kwin-effect-shaders

Edit the `data/shader.frag` file, enabling shaders / setting the options how you want.

cd kwin-effect-shaders

mkdir -p build

cd build

cmake ..

make

sudo make install

Log out / in of the session.

## Enabling / Disabling

System Settings -> Workspace -> Workspace Behavior -> Desktop Effects -> Appearance -> Shaders

The default shortcuts are:

META is the Super key (between CTRL and ALT).

Toggle on/off whole screen (all applications): `CTRL + META + R`

Toggle on/off active application: `CTRL + META + Z`

The shortcuts might be disabled by default, see "Custom Shortcuts".

You can edit the shaders.frag file (`/usr/share/kwin/shaders/1.40/shaders.frag`), then toggle off/on to reload.

## Custom shortcuts

System Settings -> Workspace -> Shortcuts -> Shortcuts -> System Services -> Kwin -> Toggle Shaders Effect







