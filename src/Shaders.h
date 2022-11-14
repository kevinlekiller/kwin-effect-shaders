/**
 * Copyright (C) 2022  kevinlekiller
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef KWIN_SHADERS_H
#define KWIN_SHADERS_H

#include <kwineffects.h>
#include <QFileSystemWatcher>
#include <QLocalServer>
#include <QSettings>

namespace KWin {

/**
 * @brief Applies fragment / vertex shaders to the display.
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
    std::unique_ptr<GLShader> m_shader;
    QLocalServer *m_server;
    QSettings *m_settings;
    bool m_effectEnabled;
    bool m_shadersLoaded = false;
    bool m_whitelistEn = false;
    const QString m_shaderSettingsName = "1_settings.glsl";
    QString m_shaderSettingsPath;
    QString m_shaderPath;
    QFileSystemWatcher m_settingsWatcher;
    QStringList m_whitelist;
    QMap<QString, QHash<qint64, QByteArray>> m_shaderArr;

    void resetEffect();
    void processWhiteList(QString);
    void processShaderPath(QString);

private Q_SLOTS:
    void slotHandleConnection();
    void slotToggleEffect(bool);
    bool slotPopulateShaderBuffers();
    void slotGenerateShaderFromBuffers();
    void slotShortcutToggleEffect();
    void slotSettingsFileChanged();
};

inline int ShadersEffect::requestedEffectChainPosition() const {
    return 100;
}

} // namespace

#endif
