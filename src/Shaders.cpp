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

    m_shadersUI.setWindowTitle("Shaders Configuration UI");

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

    connect(&m_shadersUI, &ShadersUI::signalShaderTestRequested, this, &ShadersEffect::slotUIShaderTestRequested);
    connect(&m_shadersUI, &ShadersUI::signalShaderSaveRequested, this, &ShadersEffect::slotUIShaderSaveRequested);
    connect(&m_shadersUI, &ShadersUI::signalSettingsSaveRequested, this, &ShadersEffect::slotUISettingsSaveRequested);

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
    m_shadersUI.setBlacklist(blacklist);
    m_settings->setValue("Blacklist", m_blacklist);
    m_blacklistEn = !blacklist.isEmpty();
}

// Check if whitelist is enabled.
void ShadersEffect::processWhitelist(QString whitelist) {
    m_whitelist = whitelist.trimmed().toLower().split(",");
    m_shadersUI.setWhitelist(whitelist);
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
        if (!shadersDir.isReadable()) {
            resetWindows();
            return;
        }
        m_shadersUI.setShaderPath(m_shaderPath);
        m_shaderSettingsPath = m_shaderPath;
        m_shaderSettingsPath.append(m_shaderSettingsName);
        if (!shadersDir.exists(m_shaderSettingsName)) {
            QString exampleName = m_shaderSettingsPath;
            exampleName.append(".example");
            QFile exampleFile(exampleName);
            if (!exampleFile.exists() || !exampleFile.copy(m_shaderSettingsPath)) {
                exampleFile.close();
                m_shaderSettingsPath = "";
                resetWindows();
                return;
            }
            exampleFile.close();
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

void ShadersEffect::slotUIShaderTestRequested() {
    generateShaderFromBuffer();
}

void ShadersEffect::slotUIShaderSaveRequested() {
    QFile settingsFile(m_shaderSettingsPath);
    if (!settingsFile.open(QFile::WriteOnly | QFile::Truncate)) {
        return;
    }
    m_shaderSettingsBuf = m_shadersUI.getShadersText();
    settingsFile.write(m_shaderSettingsBuf);
    settingsFile.close();
    slotReconfigureShader();
}

void ShadersEffect::slotUISettingsSaveRequested() {
    processShaderPath(m_shadersUI.getShaderPath());
    processBlacklist(m_shadersUI.getBlacklist());
    processWhitelist(m_shadersUI.getWhitelist());
    m_settings->setValue("DefaultEnabled", m_shadersUI.getDefaultEnabled());
    m_settings->sync();
}

void ShadersEffect::slotUILaunch() {
    if (m_shadersUI.isVisible()) {
        return;
    }
    QString shaderPath = m_shaderPath;
    if (shaderPath.endsWith("/")) {
        shaderPath.chop(1);
    }
    m_shadersUI.setShaderPath(shaderPath);
    m_shadersUI.setDefaultEnabled(m_settings->value("DefaultEnabled").toBool());
    if (QString::compare(m_shadersUI.getShadersText(), m_shaderSettingsBuf) != 0) {
        m_shadersUI.setShadersText(m_shaderSettingsBuf);
    }
    m_shadersUI.setShaderCompiled(m_shadersLoaded);
    if (m_shadersUI.isHidden()) {
        m_shadersUI.show();
    } else {
        m_shadersUI.open();
    }
    updateStatusCount();

}

void ShadersEffect::compileShader(QByteArray *vertex, QByteArray *fragment) {
    m_shader = ShaderManager::instance()->generateCustomShader(ShaderTrait::MapTexture, *vertex, *fragment);

    // Shader is invalid.
    if (!m_shader->isValid()) {
        resetWindows();
        return;
    }
    m_shadersLoaded = true;
    effects->addRepaintFull();
    m_shadersBeingConfigured = false;
}

void  ShadersEffect::generateShaderFromBuffer() {
    if (m_shaderArr.empty()) {
        return;
    }
    m_shadersBeingConfigured = true;
    QByteArray fragmentBuf, vertexBuf;
    QMapIterator<QString, QHash<qint64, QByteArray>> shaders(m_shaderArr);
    while (shaders.hasNext()) {
        shaders.next();
        QString curFile = shaders.key();
        if (curFile.endsWith(m_shaderSettingsName)) {
            m_shaderSettingsBuf = m_shadersUI.getShadersText();
            fragmentBuf.prepend(m_shaderSettingsBuf);
            vertexBuf.prepend(m_shaderSettingsBuf);
            continue;
        }
        bool isFrag = curFile.endsWith(".frag");
        bool isVert = curFile.endsWith(".vert");
        bool isGlsl = curFile.endsWith(".glsl");
        QHashIterator<qint64, QByteArray> shaderValues(shaders.value());
        shaderValues.next();
        if (isGlsl || isFrag) {
            fragmentBuf.append(shaderValues.value());
        }
        if (isGlsl || isVert) {
            vertexBuf.append(shaderValues.value());
        }
    }
    compileShader(&vertexBuf, &fragmentBuf);
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

        QFileInfo shaderFileInfo(curFile);
        qint64 lastModified = shaderFileInfo.lastModified().currentSecsSinceEpoch();
        QByteArray shaderBuf;
        if (m_shaderArr.contains(curFile) && m_shaderArr.value(curFile).contains(lastModified)) {
            shaderBuf = m_shaderArr.value(curFile).value(lastModified);
        } else {
            QFile shaderFile(curFile);
            if (!shaderFile.exists() || !shaderFile.open(QFile::ReadOnly)) {
                resetWindows();
                return;
            }

            shaderBuf = shaderFile.readAll();
            shaderFile.close();
            QHash <qint64, QByteArray> shaderHash;
            shaderHash.insert(lastModified, shaderBuf);
            m_shaderArr.insert(curFile, shaderHash);
        }


        if (curFile.endsWith(m_shaderSettingsName)) {
            m_settingsModified = lastModified;
            m_shaderSettingsBuf = shaderBuf;
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
    compileShader(&vertexBuf, &fragmentBuf);
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
