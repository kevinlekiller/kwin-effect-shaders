/*
    KWin - the KDE window manager
    This file is part of the KDE project.
    SPDX-FileCopyrightText: 2007 Rivo Laks <rivolaks@hot.ee>
    SPDX-FileCopyrightText: 2008 Lucas Murray <lmurray@undefinedfire.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "Shaders.h"
#include <QAction>
#include <QDir>
#include <QFile>
#include <kwinglutils.h>
#include <KGlobalAccel>
#include <KLocalizedString>

Q_LOGGING_CATEGORY(KWIN_SHADERS, "kwin4_effect_shaders", QtWarningMsg)

namespace KWin {

ShadersEffect::ShadersEffect() : m_shader(nullptr), m_allWindows(false) {
    if (!supported()) {
        return;
    }

    QAction* allWindowShortcut = new QAction(this);
    allWindowShortcut->setObjectName(QStringLiteral("Shaders"));
    allWindowShortcut->setText(i18n("Shaders Effect: Toggle Shaders Effect On All Windows"));
    KGlobalAccel::self()->setDefaultShortcut(allWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_X);
    KGlobalAccel::self()->setShortcut(allWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_X);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_X, allWindowShortcut);
    connect(allWindowShortcut, &QAction::triggered, this, &ShadersEffect::slotToggleScreenShaders);

    QAction* curWindowShortcut = new QAction(this);
    curWindowShortcut->setObjectName(QStringLiteral("ShadersWindow"));
    curWindowShortcut->setText(i18n("Shaders Effect: Toggle Shaders Effect On Current Window"));
    KGlobalAccel::self()->setDefaultShortcut(curWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    KGlobalAccel::self()->setShortcut(curWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_Z, curWindowShortcut);
    connect(curWindowShortcut, &QAction::triggered, this, &ShadersEffect::slotToggleWindowShaders);

    QAction* shadersUIShortcut = new QAction(this);
    shadersUIShortcut->setObjectName(QStringLiteral("ShadersUI"));
    shadersUIShortcut->setText(i18n("Shaders Effect: Opens the configuration UI"));
    KGlobalAccel::self()->setDefaultShortcut(shadersUIShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_A);
    KGlobalAccel::self()->setShortcut(shadersUIShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_A);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_A, shadersUIShortcut);
    connect(shadersUIShortcut, &QAction::triggered, this, &ShadersEffect::slotUILaunch);

    connect(effects, &EffectsHandler::windowClosed, this, &ShadersEffect::slotWindowClosed);

    m_settings = new QSettings("kevinlekiller", "ShadersEffect");

    processBlacklist(m_settings->value("Blacklist").toString());
    processWhitelist(m_settings->value("Whitelist").toString());
    processShaderPath(m_settings->value("ShaderPath").toString());

    // If the setting "Enable by default" is enabled, trigger the effect on first run.
    if (m_settings->value("DefaultEnabled").toBool()) {
        allWindowShortcut->trigger();
    }
}

ShadersEffect::~ShadersEffect() {
    delete m_settings;
    delete m_shader;
}

// Check if blacklist is enabled.
void ShadersEffect::processBlacklist(QString blacklist) {
    m_blacklist = blacklist.trimmed().toLower().split(",");
    m_settings->setValue("Blacklist", m_blacklist);
    m_blacklistEn = !blacklist.isEmpty();
}

// Check if whitelist is enabled.
void ShadersEffect::processWhitelist(QString whitelist) {
    m_whitelist = whitelist.trimmed().toLower().split(",");
    m_settings->setValue("Whitelist", m_whitelist);
    m_whitelistEn = !whitelist.isEmpty();
}

// Find path where shader files are in.
void ShadersEffect::processShaderPath(QString shaderPath) {
    shaderPath = shaderPath.trimmed();
    if (!shaderPath.endsWith("/")) {
        shaderPath.append("/");
    }
    if (QString::compare(m_shaderPath, shaderPath) != 0) {
        if (m_shaderPathWatcher.directories().contains(m_shaderPath)) {
            m_shaderPathWatcher.removePath(m_shaderPath);
            disconnect(&m_shaderPathWatcher, &QFileSystemWatcher::directoryChanged, this, &ShadersEffect::slotReconfigureShader);
        }
        m_shaderPath = shaderPath;
        QDir shadersDir(shaderPath);
        if (!shadersDir.isReadable() || !shadersDir.exists(m_shaderSettingsName)) {
            resetWindows();
            return;
        }
        m_settings->setValue("ShaderPath", m_shaderPath);
        if (m_shaderPathWatcher.addPath(m_shaderPath)) {
            connect(&m_shaderPathWatcher, &QFileSystemWatcher::directoryChanged, this, &ShadersEffect::slotReconfigureShader);
        }
        slotReconfigureShader();
    }
}

// Reset values to default.
void ShadersEffect::resetWindows() {
    m_windows.clear();
    m_allWindows = false;
    m_shadersLoaded = false;
    m_shadersBeingConfigured = false;
    effects->addRepaintFull();
}

void ShadersEffect::updateStatusCount() {
    if (!m_shadersUI.isVisible()) {
        return;
    }
    m_shadersUI.setNumWindowsStatus(m_windows.count() + (int) m_allWindows);
}

void ShadersEffect::slotUILaunch() {
    if (m_shadersUI.isVisible()) {
        return;
    }
    QString shaderPath = m_shaderPath;
    if (shaderPath.endsWith("/")) {
        shaderPath.chop(1);
    }
    m_shadersUI.setWindowTitle("Shaders Configuration UI");
    m_shadersUI.setShaderPath(shaderPath);
    m_shadersUI.setBlacklist(m_blacklist.join(","));
    m_shadersUI.setWhitelist(m_whitelist.join(","));
    m_shadersUI.setDefaultEnabled(m_settings->value("DefaultEnabled").toBool());
    updateStatusCount();
    connect(&m_shadersUI, &QDialog::finished, this, &ShadersEffect::slotUIClosed);
    m_shadersUI.open();
}

void ShadersEffect::slotUIClosed() {
    disconnect(&m_shadersUI, &QDialog::finished, this, &ShadersEffect::slotUIClosed);
    m_settings->sync();
    processShaderPath(m_shadersUI.getShaderPath());
    processBlacklist(m_shadersUI.getBlacklist());
    processWhitelist(m_shadersUI.getWhitelist());
    m_settings->setValue("DefaultEnabled", m_shadersUI.getDefaultEnabled());
}

// Build shader if needed.
void ShadersEffect::slotReconfigureShader() {
    // In case this is triggered multiple times in a fast succession.
    if (m_shadersBeingConfigured) {
        return;
    }
    m_shadersBeingConfigured = true;
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
            resetWindows();
            return;
        }

        QByteArray shaderBuf = shaderFile.readAll();
        shaderFile.close();

        if (curFile.endsWith(m_shaderSettingsName)) {
            fragmentBuf.append(shaderBuf);
            vertexBuf.append(shaderBuf);
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
    m_shader = ShaderManager::instance()->generateCustomShader(ShaderTrait::MapTexture, vertexBuf, fragmentBuf);

    // Shader is invalid.
    if (!m_shader->isValid()) {
        resetWindows();
        return;
    }

    // Shader succsesfully generated.
    m_shadersLoaded = true;
    effects->addRepaintFull();
    m_shadersBeingConfigured = false;
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
        if ((m_blacklistEn && m_blacklist.contains(windowName, Qt::CaseInsensitive)) ||
            (m_whitelistEn && !m_whitelist.contains(windowName, Qt::CaseInsensitive))
        ) {
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
    updateStatusCount();
}

// User activated keybind to toggle shader on all windows.
void ShadersEffect::slotToggleScreenShaders() {
    if (!m_shadersLoaded) {
        return;
    }
    // Toggle if user wants shaders on all windows or not.
    m_allWindows = !m_allWindows;
    updateStatusCount();
    effects->addRepaintFull();
}

// User activated keybind to toggle shader on active window.
void ShadersEffect::slotToggleWindowShaders() {
    if (!m_shadersLoaded || !effects->activeWindow()) {
        return;
    }
    // Toggle if user wants shaders on active window or not.
    if (m_windows.contains(effects->activeWindow())) {
        m_windows.removeOne(effects->activeWindow());
    } else {
        m_windows.append(effects->activeWindow());
    }
    updateStatusCount();
    effects->activeWindow()->addRepaintFull();
}

// If shaders are active on any window.
bool ShadersEffect::isActive() const {
    return m_shadersLoaded && (m_allWindows || !m_windows.isEmpty());
}

} // namespace
