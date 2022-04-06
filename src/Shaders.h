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
#include <QSettings>
#include "ShadersUI.h"

namespace KWin {

/**
 * Applies fragment shaders to the display.
 */
class ShadersEffect : public Effect {
    Q_OBJECT

public:
    ShadersEffect();
    ~ShadersEffect() override;
    void prePaintWindow(EffectWindow* w, WindowPrePaintData& data, std::chrono::milliseconds presentTime) override;
    void paintWindow(EffectWindow* w, int mask, const QRegion region, WindowPaintData& data) override;
    void postPaintWindow(EffectWindow* w) override;
    bool isActive() const override;
    int requestedEffectChainPosition() const override;
    static bool supported();

private:
    GLShader* m_shader;
    QSettings *m_settings;
    ShadersUI m_shadersUI;
    bool m_allWindows;
    bool m_shadersLoaded = false;
    bool m_blacklistEn = false;
    bool m_whitelistEn = false;
    bool m_autoApply = false;
    bool m_useShader;
    const QString m_shaderSettingsName = "1_settings.glsl";
    QString m_shaderSettingsPath;
    QString m_shaderPath;
    QList<EffectWindow*> m_windows;
    QFileSystemWatcher m_shaderPathWatcher;
    QStringList m_blacklist;
    QStringList m_whitelist;
    QMap<QString, QHash<qint64, QByteArray>> m_shaderArr;

    void resetWindows();
    void updateStatusCount();
    void processBWList(QString, bool);
    void processShaderPath(QString);

private Q_SLOTS:
    void slotUILaunch();
    void slotUIShaderSaveRequested();
    void slotUISettingsSaveRequested();
    void slotUIToggledAllWindows(bool);
    void slotPopulateShaderBuffers();
    void slotGenerateShaderFromBuffers();
    void slotToggleScreenShaders();
    void slotToggleWindowShaders();
    void slotWindowClosed(KWin::EffectWindow *w);
};

inline int ShadersEffect::requestedEffectChainPosition() const {
    return 100;
}

} // namespace

#endif
