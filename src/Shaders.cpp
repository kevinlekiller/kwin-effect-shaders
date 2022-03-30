/*
    KWin - the KDE window manager
    This file is part of the KDE project.
    SPDX-FileCopyrightText: 2007 Rivo Laks <rivolaks@hot.ee>
    SPDX-FileCopyrightText: 2008 Lucas Murray <lmurray@undefinedfire.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "Shaders.h"
#include "ShadersConfig.h"
#include <QAction>
#include <QDir>
#include <QFile>
#include <kwinglutils.h>
#include <KGlobalAccel>
#include <KLocalizedString>

Q_LOGGING_CATEGORY(KWIN_SHADERS, "kwin4_effect_shaders", QtWarningMsg)

namespace KWin
{

ShadersEffect::ShadersEffect() : m_shader(nullptr), m_allWindows(false) {
    if (!supported()) {
        return;
    }

    // This is not ideal, but since ShadersConfig::configChange never emits, we don't have a choice.
    m_kwinrcPath = QStandardPaths::locate(QStandardPaths::ConfigLocation, "kwinrc", QStandardPaths::LocateFile);
    if (m_kwinrcWatcher.addPath(m_kwinrcPath)) {
        connect(&m_kwinrcWatcher, &QFileSystemWatcher::fileChanged, this, &ShadersEffect::slotReconfigureConfig);
    }

    /* Unfortunately the signal never emits.
    connect(&m_shadersConfig, &ShadersConfig::configChanged, this, &ShadersEffect::slotReconfigureConfig);
    */

    if (!m_foundShaderPath) {
        slotReconfigureConfig();
    }

    QAction* allWindowShortcut = new QAction(this);
    allWindowShortcut->setObjectName(QStringLiteral("Shaders"));
    allWindowShortcut->setText(i18n("Toggle Shaders Effect on all Windows"));
    KGlobalAccel::self()->setDefaultShortcut(allWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    KGlobalAccel::self()->setShortcut(allWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_R, allWindowShortcut);
    connect(allWindowShortcut, &QAction::triggered, this, &ShadersEffect::slotToggleScreenShaders);

    QAction* curWindowShortcut = new QAction(this);
    curWindowShortcut->setObjectName(QStringLiteral("ShadersWindow"));
    curWindowShortcut->setText(i18n("Toggle Shaders Effect on Window"));
    KGlobalAccel::self()->setDefaultShortcut(curWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    KGlobalAccel::self()->setShortcut(curWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_Z, curWindowShortcut);
    connect(curWindowShortcut, &QAction::triggered, this, &ShadersEffect::slotToggleWindowShaders);

    connect(effects, &EffectsHandler::windowClosed, this, &ShadersEffect::slotWindowClosed);

    // If the setting "Enable by default" is enabled, trigger the effect on first run.
    if (ShadersConfig::defaultEnabled()) {
        allWindowShortcut->trigger();
    }
}

ShadersEffect::~ShadersEffect() {
    delete m_shader;
}

// Reset variables to default.
void ShadersEffect::resetWindows(bool repaint) {
    m_windows.clear();
    m_allWindows = false;
    m_foundShaderPath = false;
    if (repaint) {
        m_shadersLoaded = false;
        effects->addRepaintFull();
    }
}

// Get the settings from kwinrc.
void ShadersEffect::slotReconfigureConfig() {
    resetWindows(false);
    // Removing / re-adding the file gets around an issue where when the file is modified the signal
    // is not emited after the first time. See https://doc.qt.io/qt-5/qfilesystemwatcher.html#fileChanged
    m_kwinrcWatcher.removePath(m_kwinrcPath);
    m_kwinrcWatcher.addPath(m_kwinrcPath);

    ShadersConfig::self()->load();

    // Find path where shader files are in.
    QString shaderPath = ShadersConfig::shaderPath().trimmed();
    if (!shaderPath.endsWith("/")) {
        shaderPath.append("/");
    }
    m_foundShaderPath = true;
    if (QString::compare(m_shaderPath, shaderPath) != 0) {
        m_shaderPath = shaderPath;
        QDir shadersDir(m_shaderPath);
        if (!shadersDir.isReadable() || !shadersDir.exists(m_settingsName)) {
            resetWindows(true);
            return;
        }
        slotReconfigureShader();
    }

    // Check if blacklist is enabled.
    QString blacklist = ShadersConfig::blacklist().trimmed();
    m_blacklist = blacklist.toLower().split(",");
    m_blacklistEn = !blacklist.isEmpty();

    // Check if whitelist is enabled.
    QString whitelist = ShadersConfig::whitelist().trimmed();
    m_whitelist = whitelist.toLower().split(",");
    m_whitelistEn = !whitelist.isEmpty();
}

// Create shader from shader files.
void ShadersEffect::slotReconfigureShader() {
    m_shadersLoaded = false;

    // Failed to find path where shader files are in.
    if (!m_foundShaderPath) {
        return;
    }

    // Monitor changes to the settings file, if modified, re-generate the shader.
    QString tmpSettingsPath = m_shaderPath;
    tmpSettingsPath.append(m_settingsName);
    if (QString::compare(tmpSettingsPath, m_settingsPath) != 0) {
        m_settingsWatcher.removePath(m_settingsPath);
        m_settingsPath = tmpSettingsPath;
        m_settingsWatcher.addPath(m_settingsPath);
        disconnect(&m_settingsWatcher);
        connect(&m_settingsWatcher, &QFileSystemWatcher::fileChanged, this, &ShadersEffect::slotReconfigureShader);
    }

    // Iterate shaders files and append them to their respectful buffers.
    QDir shadersDir(m_shaderPath);
    shadersDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    shadersDir.setSorting(QDir::Name | QDir::IgnoreCase);
    QFileInfoList shadersList = shadersDir.entryInfoList();
    QByteArray fragmentBuf, vertexBuf;
    for (int i = 0; i < shadersList.size(); ++i) {
        QString curFile = shadersList.at(i).absoluteFilePath();

        bool isFrag = curFile.endsWith(".frag");
        bool isVert = curFile.endsWith(".vert");
        bool isGlsl = curFile.endsWith(".glsl");
        if (!isFrag && !isVert && !isGlsl) {
            continue;
        }

        QFile shaderFile(curFile);
        if (!shaderFile.exists() || !shaderFile.open(QFile::ReadOnly)) {
            resetWindows(true);
            return;
        }

        QByteArray shaderBuf = shaderFile.readAll();
        shaderFile.close();

        // Settings file should always be first and in both shader buffers.
        if (isGlsl && curFile.endsWith(m_settingsName)) {
            fragmentBuf.prepend(shaderBuf);
            vertexBuf.prepend(shaderBuf);
            continue;
        }

        if (isFrag || isGlsl) {
            fragmentBuf.append(shaderBuf);
        }

        if (isVert || isGlsl) {
            vertexBuf.append(shaderBuf);
        }
    }

    // Generate the shader.
    m_shader = KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture, vertexBuf, fragmentBuf);

    // Shader is invalid.
    if (!m_shader->isValid()) {
        resetWindows(true);
        return;
    }

    // Shader succsesfully generated.
    m_shadersLoaded = true;
    effects->addRepaintFull();
}

// Checks for GLSL 140 support.
bool ShadersEffect::supported() {
#ifdef KWIN_HAVE_OPENGLES
    return false;
#else
    // GLSL 1.4 needs GL >= 3.1
    return effects->compositingType() == OpenGLCompositing && hasGLVersion(3, 1);
#endif
}

// Draw the window with the requested shader.
void ShadersEffect::drawWindow(EffectWindow* w, int mask, const QRegion &region, WindowPaintData& data) {
    bool useShader = m_shadersLoaded && m_allWindows != m_windows.contains(w);
    // Check if window is blacklisted or whitelisted.
    if (useShader && (m_blacklistEn || m_whitelistEn)) {
        QString windowName = w->windowClass().trimmed().split(" ")[0];
        if ((m_blacklistEn && m_blacklist.contains(windowName, Qt::CaseInsensitive)) || (m_whitelistEn && !m_whitelist.contains(windowName, Qt::CaseInsensitive))) {
            useShader = false;
        }
    }
    if (!useShader) {
        effects->drawWindow(w, mask, region, data);
        return;
    }
    ShaderManager::instance()->pushShader(m_shader);
    m_shader->setUniform(m_shader->uniformLocation("g_Random"), (float) drand48());
    data.shader = m_shader;
    effects->drawWindow(w, mask, region, data);
    ShaderManager::instance()->popShader();
}

// Window was closed, remove from array.
void ShadersEffect::slotWindowClosed(EffectWindow* w) {
    m_windows.removeOne(w);
}

// User activated keybind to toggle shader on all windows.
void ShadersEffect::slotToggleScreenShaders() {
    if (!m_shadersLoaded) {
        return;
    }
    // Toggle if user wants shaders on all windows or not.
    m_allWindows = !m_allWindows;
    effects->addRepaintFull();
}

// User activated keybind to toggle shader on active window.
void ShadersEffect::slotToggleWindowShaders() {
    if (!m_shadersLoaded || !effects->activeWindow()) {
        return;
    }
    // Toggle if user wants shaders on active window or not.
    m_windows.contains(effects->activeWindow()) ?
        (void) m_windows.removeOne(effects->activeWindow()) :
               m_windows.append(effects->activeWindow()
    );
    effects->activeWindow()->addRepaintFull();
}

// If shaders are active on any window.
bool ShadersEffect::isActive() const {
    return m_shadersLoaded && (m_allWindows || !m_windows.isEmpty());
}

} // namespace
