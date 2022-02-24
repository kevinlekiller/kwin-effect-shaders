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
#ifdef DEBUGON
#include <QFile>
#endif

namespace KWin
{

class GLShader;

/**
 * Applies fragment shaders to the display.
 */
class ShadersEffect
    : public Effect
{
    Q_OBJECT
    Q_PROPERTY(QStringList blacklist READ blacklist);
    Q_PROPERTY(QStringList whitelist READ whitelist);
    Q_PROPERTY(QString shaderPath READ shaderPath);
public:
    ShadersEffect();
    ~ShadersEffect() override;
    void reconfigure(ReconfigureFlags) override;

    void drawWindow(EffectWindow* w, int mask, const QRegion &region, WindowPaintData& data) override;
    void paintEffectFrame(KWin::EffectFrame* frame, const QRegion &region, double opacity, double frameOpacity) override;
    bool isActive() const override;
    bool provides(Feature) override;

    int requestedEffectChainPosition() const override;

    static bool supported();

    QStringList blacklist() const;
    QStringList whitelist() const;
    QString shaderPath() const;
public Q_SLOTS:
    void toggleScreenShaders();
    void toggleWindow();
    void slotWindowClosed(KWin::EffectWindow *w);

protected:
    bool loadShaders();

private:
#ifdef DEBUGON
    QFile logFile;
#endif
    bool m_blacklistEn;
    bool m_whitelistEn;
    bool m_shaderLoaded;
    GLShader* m_shader;
    bool m_allWindows;
    QStringList m_blacklist;
    QStringList m_whitelist;
    QList<EffectWindow*> m_windows;
    QString m_shaderPath;
};

inline QStringList ShadersEffect::blacklist() const
{
        return m_blacklist;
}

inline QStringList ShadersEffect::whitelist() const
{
        return m_whitelist;
}

inline QString ShadersEffect::shaderPath() const
{
        return m_shaderPath;
}

inline int ShadersEffect::requestedEffectChainPosition() const
{
    return 100;
}

} // namespace

#endif
