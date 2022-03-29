/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2007 Rivo Laks <rivolaks@hot.ee>
Copyright (C) 2008 Lucas Murray <lmurray@undefinedfire.com>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#ifndef KWIN_SHADERS_H
#define KWIN_SHADERS_H

#include <kwineffects.h>
#include <QFileSystemWatcher>
namespace KWin
{

/**
 * Applies fragment shaders to the display.
 */
class ShadersEffect : public Effect
{
    Q_OBJECT
public:
    ShadersEffect();
    ~ShadersEffect() override;
    void reconfigure(ReconfigureFlags) override;
    void drawWindow(EffectWindow* w, int mask, const QRegion &region, WindowPaintData& data) override;
    bool isActive() const override;
    int requestedEffectChainPosition() const override;

    static bool supported();

public Q_SLOTS:
    void slotToggleScreenShaders();
    void slotToggleWindow();
    void slotWindowClosed(KWin::EffectWindow *w);
    void slotReconfigure();

private:
    GLShader* m_shader;
    bool m_allWindows;
    bool m_blacklistEn;
    bool m_whitelistEn;
    bool m_shadersLoaded;
    bool m_foundShaderPath;
    const QString m_settingsName = "1_settings.glsl";
    QString m_shaderPath;
    QString m_settingsPath;
    QStringList m_blacklist;
    QStringList m_whitelist;
    QList<EffectWindow*> m_windows;
    QFileSystemWatcher m_settingsWatcher;
};

inline int ShadersEffect::requestedEffectChainPosition() const
{
    return 100;
}

} // namespace

#endif
