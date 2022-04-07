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

Q_LOGGING_CATEGORY(KWIN_SHADERS, "kwin_effect_shaders", QtWarningMsg)

namespace KWin {

/**
 * Construct.
 *
 * @brief ShadersEffect::ShadersEffect
 */
ShadersEffect::ShadersEffect() : m_shader(nullptr), m_effectEnabled(false) {
    // Initialize settings.
    m_settings = new QSettings("kevinlekiller", "kwin_effect_shaders");

    // Fetch settings.
    processBWList(m_settings->value("Blacklist").toString(), false);
    processBWList(m_settings->value("Whitelist").toString(), true);
    processShaderPath(m_settings->value("ShaderPath").toString());

    // Setup keyboard shortcuts.
    QAction* allWindowShortcut = new QAction(this);
    allWindowShortcut->setObjectName(QStringLiteral("Shaders"));
    allWindowShortcut->setText(i18n("Shaders Effect: Toggle On or Off the effect"));
    KGlobalAccel::self()->setDefaultShortcut(allWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    KGlobalAccel::self()->setShortcut(allWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_Z, allWindowShortcut);

    QAction* shadersUIShortcut = new QAction(this);
    shadersUIShortcut->setObjectName(QStringLiteral("ShadersUI"));
    shadersUIShortcut->setText(i18n("Shaders Effect: Opens the configuration UI"));
    KGlobalAccel::self()->setDefaultShortcut(shadersUIShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_X);
    KGlobalAccel::self()->setShortcut(shadersUIShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_X);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_X, shadersUIShortcut);

    // Setup connections.
    connect(allWindowShortcut, &QAction::triggered, this, &ShadersEffect::slotShortcutToggleEffect);
    connect(shadersUIShortcut, &QAction::triggered, this, &ShadersEffect::slotUILaunch);
    connect(&m_shadersUI, &ShadersUI::signalShaderTestRequested, this, &ShadersEffect::slotGenerateShaderFromBuffers);
    connect(&m_shadersUI, &ShadersUI::signalShaderSaveRequested, this, &ShadersEffect::slotUIShaderSaveRequested);
    connect(&m_shadersUI, &ShadersUI::signalSettingsSaveRequested, this, &ShadersEffect::slotUISettingsSaveRequested);
    connect(&m_shadersUI, &ShadersUI::signalEffectToggled, this, &ShadersEffect::slotToggleEffect);

    // If the setting "Enable by default" is enabled, trigger the effect on first run.
    if (m_settings->value("DefaultEnabled").toBool()) {
        allWindowShortcut->trigger();
    }
}

/**
 * Destruct.
 *
 * @brief ShadersEffect::~ShadersEffect
 */
ShadersEffect::~ShadersEffect() {
    delete m_settings;
    delete m_shader;
}

/**
 * Process user specified white or black list.
 * The list should be the names of window names seperated by a comma.
 * The list only allows windows with specified name to be processed.
 *
 * @brief ShadersEffect::processBWList
 * @param isWhitelist ->
 *             true : Whitelist
 *            false : Blacklist
 */
void ShadersEffect::processBWList(QString list, bool isWhitelist) {
    QStringList llist = list.trimmed().toLower().split(",");
    QString listType;
    if (isWhitelist) {
        listType = "Whitelist";
        m_whitelist = llist;
        m_shadersUI.setWhitelist(list);
        m_whitelistEn = !list.isEmpty();
    } else {
        listType = "Blacklist";
        m_blacklist = llist;
        m_shadersUI.setBlacklist(list);
        m_blacklistEn = !list.isEmpty();
    }

    if (QString::compare(list, m_settings->value(listType).toString()) != 0) {
        m_settings->setValue(listType, list);
    }
}

/**
 * Process user specified shader path.
 * The path should contain glsl files ending with glsl, frag and vert extensions.
 * The path must contain the 1_settings.glsl.example file.
 * If the path looks valid, the slotPopulateShaderBuffers() function is executed.
 *
 * @brief ShadersEffect::processShaderPath
 * @param shaderPath -> The shader path to process.
 */
void ShadersEffect::processShaderPath(QString shaderPath) {
    shaderPath = shaderPath.trimmed();
    if (shaderPath.isEmpty()) {
        shaderPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, "kwin-effect-shaders_shaders", QStandardPaths::LocateDirectory);
        if (shaderPath.isEmpty()) {
            resetWindows();
            return;
        }
    }
    if (!shaderPath.endsWith("/")) {
        shaderPath.append("/");
    }
    if (QString::compare(m_shaderPath, shaderPath) != 0) {
        if (m_shaderPathWatcher.directories().contains(m_shaderPath)) {
            m_shaderPathWatcher.removePath(m_shaderPath);
            disconnect(&m_shaderPathWatcher, &QFileSystemWatcher::directoryChanged, this, &ShadersEffect::slotPopulateShaderBuffers);
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
            connect(&m_shaderPathWatcher, &QFileSystemWatcher::directoryChanged, this, &ShadersEffect::slotPopulateShaderBuffers);
        }
        slotPopulateShaderBuffers();
    }
}

/**
 * If any step of the shader detection / generation process fails,
 * some variables are reset to their default.
 *
 * @brief ShadersEffect::resetWindows
 */
void ShadersEffect::resetWindows() {
    m_effectEnabled = false;
    m_shadersLoaded = false;
    effects->addRepaintFull();
}

/**
 * The user wants to save their modifications to the shader settings file.
 *
 * @brief ShadersEffect::slotUIShaderSaveRequested
 */
void ShadersEffect::slotUIShaderSaveRequested() {
    QFile settingsFile(m_shaderSettingsPath);
    if (!settingsFile.open(QFile::WriteOnly | QFile::Truncate)) {
        return;
    }
    settingsFile.write(m_shadersUI.getShadersText());
    settingsFile.close();
    slotPopulateShaderBuffers();
}

/**
 * The user wants to save main program settings.
 *
 * @brief ShadersEffect::slotUISettingsSaveRequested
 */
void ShadersEffect::slotUISettingsSaveRequested() {
    processShaderPath(m_shadersUI.getShaderPath());
    processBWList(m_shadersUI.getBlacklist(), false);
    processBWList(m_shadersUI.getWhitelist(), true);
    m_settings->setValue("DefaultEnabled", m_shadersUI.getDefaultEnabled());
    m_settings->setValue("AutoApply", m_shadersUI.getAutoApply());
    m_settings->sync();
}

/**
 * Passes some variables to the UI instance, then opens the UI.
 *
 * @brief ShadersEffect::slotUILaunch
 */
void ShadersEffect::slotUILaunch() {
    if (m_shadersUI.isVisible()) {
        return;
    }
    QString shaderPath = m_shaderPath;
    m_shadersUI.setShaderPath(shaderPath);
    m_shadersUI.setDefaultEnabled(m_settings->value("DefaultEnabled").toBool());
    m_shadersUI.setAutoApply(m_settings->value("AutoApply").toBool());
    m_shadersUI.setShaderCompiled(m_shadersLoaded);
    m_shadersUI.displayUI();
}

/**
 * Reads shader files from m_shaderPath and stores in them in buffer.
 *
 * @brief ShadersEffect::slotPopulateShaderBuffers
 */
void ShadersEffect::slotPopulateShaderBuffers() {
    QDir shadersDir(m_shaderPath);
    shadersDir.setFilter(QDir::Files | QDir::NoDotAndDotDot);
    shadersDir.setSorting(QDir::Name | QDir::IgnoreCase);

    QFileInfoList shadersList = shadersDir.entryInfoList();
    for (int i = 0; i < shadersList.size(); ++i) {
        QString curFile = shadersList.at(i).absoluteFilePath();
        if (!curFile.endsWith(".frag") && !curFile.endsWith(".vert") && !curFile.endsWith(".glsl")) {
            continue;
        }

        QFileInfo shaderFileInfo(curFile);
        qint64 lastModified = shaderFileInfo.lastModified().currentSecsSinceEpoch();
        if (m_shaderArr.contains(curFile)) {
            if (m_shaderArr.value(curFile).contains(lastModified)) {
                continue;
            }
            m_shaderArr.remove(curFile);
        }

        QFile shaderFile(curFile);
        if (!shaderFile.open(QFile::ReadOnly)) {
            resetWindows();
            return;
        }
        QByteArray shaderBuf = shaderFile.readAll();
        shaderFile.close();

        QHash <qint64, QByteArray> shaderHash;
        shaderHash.insert(lastModified, shaderBuf);
        m_shaderArr.insert(curFile, shaderHash);

        if (curFile.endsWith(m_shaderSettingsName)) {
            m_shadersUI.setShadersText(shaderBuf);
            continue;
        }
    }
    slotGenerateShaderFromBuffers();
}

/**
 * Try to compile the shader from the buffers.
 *
 * @brief ShadersEffect::slotGenerateShaderFromBuffers
 */
void ShadersEffect::slotGenerateShaderFromBuffers() {
    if (m_shaderArr.empty()) {
        return;
    }
    QByteArray fragmentBuf, vertexBuf;
    QMapIterator<QString, QHash<qint64, QByteArray>> shaders(m_shaderArr);
    while (shaders.hasNext()) {
        shaders.next();
        QString curFile = shaders.key();
        if (curFile.endsWith(m_shaderSettingsName)) {
            QByteArray settingsBuf = m_shadersUI.getShadersText();
            fragmentBuf.prepend(settingsBuf);
            vertexBuf.prepend(settingsBuf);
            continue;
        }
        bool isGlsl = curFile.endsWith(".glsl");
        QHashIterator<qint64, QByteArray> shaderValues(shaders.value());
        shaderValues.next();
        if (isGlsl || curFile.endsWith(".frag")) {
            fragmentBuf.append(shaderValues.value());
        }
        if (isGlsl || curFile.endsWith(".vert")) {
            vertexBuf.append(shaderValues.value());
        }
    }
    m_shader = ShaderManager::instance()->generateCustomShader(ShaderTrait::MapTexture, vertexBuf, fragmentBuf);

    // Shader is invalid.
    if (!m_shader->isValid()) {
        resetWindows();
        return;
    }
    m_shadersLoaded = true;
    effects->addRepaintFull();
}

/**
 * Tells KWin if the plugin should be loaded or not.
 *
 * @brief ShadersEffect::supported
 * @return
 *   true -> If the user's device has GLSL 1.40 support.
 *  false -> If not the above.
 */
bool ShadersEffect::supported() {
#ifdef KWIN_HAVE_OPENGLES
    return false;
#else
    // GLSL 1.4 needs GL >= 3.1
    return effects->compositingType() == OpenGLCompositing && hasGLVersion(3, 1);
#endif
}

/**
 * Overriden function.
 *
 * @brief ShadersEffect::prePaintWindow
 * @param w
 * @param data
 * @param presentTime
 */
void ShadersEffect::prePaintWindow(EffectWindow* w, WindowPrePaintData& data, std::chrono::milliseconds presentTime) {
    effects->prePaintWindow(w, data, presentTime);
}

/**
 * Overriden function.
 *
 * @brief ShadersEffect::paintWindow
 * @param w
 * @param mask
 * @param region
 * @param data
 */
void ShadersEffect::paintWindow(EffectWindow* w, int mask, const QRegion region, WindowPaintData& data) {
    m_useShader = isActive();
    if (!m_useShader) {
        effects->paintWindow(w, mask, region, data);
        return;
    } else if (m_blacklistEn || m_whitelistEn) {
        // Check if window is blacklisted or whitelisted.
        QString windowName = w->windowClass().trimmed().split(" ")[0];
        if ((m_blacklistEn && m_blacklist.contains(windowName, Qt::CaseInsensitive)) || (m_whitelistEn && !m_whitelist.contains(windowName, Qt::CaseInsensitive))) {
            m_useShader = false;
        }
    }
    ShaderBinder bind(m_shader);
    m_shader->setUniform("g_Random", (float) drand48());
    m_shader->setUniform("g_TextureSize", QVector2D(effects->virtualScreenSize().width(), effects->virtualScreenSize().height()));
    m_shader->setUniform("modelViewProjectionMatrix", data.projectionMatrix());
    data.shader = m_shader;
    effects->paintWindow(w, mask, region, data);
}

/**
 * Overriden function.
 *
 * @brief ShadersEffect::postPaintWindow
 * @param w
 */
void ShadersEffect::postPaintWindow(EffectWindow* w) {
    effects->postPaintWindow(w);
}

/**
 * When the user activates the keyboard shortcut, enable or disable the effect.
 *
 * @brief ShadersEffect::slotShortcutToggleEffect
 */
void ShadersEffect::slotShortcutToggleEffect() {
    if (!m_shadersLoaded) {
        return;
    }
    slotToggleEffect(!m_effectEnabled);
}

/**
 * User wants to enable or disable the effect.
 *
 * @brief ShadersEffect::slotToggleEffect
 * @param status
 */
void ShadersEffect::slotToggleEffect(bool status) {
    if (!m_shadersLoaded) {
        return;
    }
    m_effectEnabled = status;
    m_shadersUI.setEffectEnabled(m_effectEnabled);
    effects->addRepaintFull();
}

/**
 * Return if any window has the shader applied to it.
 *
 * @brief ShadersEffect::isActive
 * @return
 *    true -> At least one window has the shader applied to it.
 *   false -> No window has the shader applied to it.
 */
bool ShadersEffect::isActive() const {
    return m_shadersLoaded && m_effectEnabled;
}

} // namespace
