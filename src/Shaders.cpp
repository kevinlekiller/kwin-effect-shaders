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
ShadersEffect::ShadersEffect() : m_shader(nullptr), m_allWindows(false) {
    // Initialize settings.
    m_settings = new QSettings("kevinlekiller", "ShadersEffect");

    // Fetch settings.
    processBlacklist(m_settings->value("Blacklist").toString());
    processWhitelist(m_settings->value("Whitelist").toString());
    processShaderPath(m_settings->value("ShaderPath").toString());

    // Setup keyboard shortcuts.
    QAction* allWindowShortcut = new QAction(this);
    allWindowShortcut->setObjectName(QStringLiteral("Shaders"));
    allWindowShortcut->setText(i18n("Shaders Effect: Toggle Shaders Effect On All Windows"));
    KGlobalAccel::self()->setDefaultShortcut(allWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_X);
    KGlobalAccel::self()->setShortcut(allWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_X);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_X, allWindowShortcut);

    QAction* curWindowShortcut = new QAction(this);
    curWindowShortcut->setObjectName(QStringLiteral("ShadersWindow"));
    curWindowShortcut->setText(i18n("Shaders Effect: Toggle Shaders Effect On Current Window"));
    KGlobalAccel::self()->setDefaultShortcut(curWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    KGlobalAccel::self()->setShortcut(curWindowShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_Z, curWindowShortcut);

    QAction* shadersUIShortcut = new QAction(this);
    shadersUIShortcut->setObjectName(QStringLiteral("ShadersUI"));
    shadersUIShortcut->setText(i18n("Shaders Effect: Opens the configuration UI"));
    KGlobalAccel::self()->setDefaultShortcut(shadersUIShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_A);
    KGlobalAccel::self()->setShortcut(shadersUIShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_A);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_A, shadersUIShortcut);

    // Setup connections.
    connect(allWindowShortcut, &QAction::triggered, this, &ShadersEffect::slotToggleScreenShaders);
    connect(curWindowShortcut, &QAction::triggered, this, &ShadersEffect::slotToggleWindowShaders);
    connect(shadersUIShortcut, &QAction::triggered, this, &ShadersEffect::slotUILaunch);
    connect(effects, &EffectsHandler::windowClosed, this, &ShadersEffect::slotWindowClosed);
    connect(&m_shadersUI, &ShadersUI::signalShaderTestRequested, this, &ShadersEffect::slotGenerateShaderFromBuffers);
    connect(&m_shadersUI, &ShadersUI::signalShaderSaveRequested, this, &ShadersEffect::slotUIShaderSaveRequested);
    connect(&m_shadersUI, &ShadersUI::signalSettingsSaveRequested, this, &ShadersEffect::slotUISettingsSaveRequested);
    connect(&m_shadersUI, &ShadersUI::signalAllWindowsToggled, this, &ShadersEffect::slotUIToggledAllWindows);

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
 * Process user specified blacklist.
 * The blacklist should be the names of window names seperated by a comma.
 * The blacklist is used to determine if a window name is blacklisted from being processed.
 *
 * @brief ShadersEffect::processBlacklist
 * @param blacklist -> The blacklist to process.
 */
void ShadersEffect::processBlacklist(QString blacklist) {
    m_blacklist = blacklist.trimmed().toLower().split(",");
    m_shadersUI.setBlacklist(blacklist);
    m_settings->setValue("Blacklist", m_blacklist);
    m_blacklistEn = !blacklist.isEmpty();
}

/**
 * Process user specified whitelist.
 * The whitelist should be the names of window names seperated by a comma.
 * The whitelist only allows windows with specified name to be processed.
 *
 * @brief ShadersEffect::processWhitelist
 * @param whitelist -> The whitelist to process.
 */
void ShadersEffect::processWhitelist(QString whitelist) {
    m_whitelist = whitelist.trimmed().toLower().split(",");
    m_shadersUI.setWhitelist(whitelist);
    m_settings->setValue("Whitelist", m_whitelist);
    m_whitelistEn = !whitelist.isEmpty();
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
        shaderPath = QStandardPaths::locate(QStandardPaths::DataLocation, "kwin-effect-shaders_shaders", QStandardPaths::LocateDirectory);
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
    m_windows.clear();
    m_allWindows = false;
    m_shadersLoaded = false;
    effects->addRepaintFull();
}

/**
 * Tells the UI the number of windows which are being processed.
 *
 * @brief ShadersEffect::updateStatusCount
 */
void ShadersEffect::updateStatusCount() {
    if (!m_shadersUI.isVisible()) {
        return;
    }
    m_shadersUI.setNumWindowsStatus(m_windows.count() + (int) m_allWindows);
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
    processBlacklist(m_shadersUI.getBlacklist());
    processWhitelist(m_shadersUI.getWhitelist());
    m_settings->setValue("DefaultEnabled", m_shadersUI.getDefaultEnabled());
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
    if (shaderPath.endsWith("/")) {
        shaderPath.chop(1);
    }
    m_shadersUI.setShaderPath(shaderPath);
    m_shadersUI.setDefaultEnabled(m_settings->value("DefaultEnabled").toBool());
    m_shadersUI.setShaderCompiled(m_shadersLoaded);
    m_shadersUI.displayUI();
    updateStatusCount();
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
        if (m_shaderArr.contains(curFile) && m_shaderArr.value(curFile).contains(lastModified)) {
            continue;
        }

        QFile shaderFile(curFile);
        if (!shaderFile.exists() || !shaderFile.open(QFile::ReadOnly)) {
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
    m_useShader = m_shadersLoaded && m_allWindows != m_windows.contains(w);
    // Check if window is blacklisted or whitelisted.
    if (m_useShader && (m_blacklistEn || m_whitelistEn)) {
        QString windowName = w->windowClass().trimmed().split(" ")[0];
        if ((m_blacklistEn && m_blacklist.contains(windowName, Qt::CaseInsensitive)) ||
            (m_whitelistEn && !m_whitelist.contains(windowName, Qt::CaseInsensitive))
        ) {
            m_useShader = false;
        }
    }
    if (!m_useShader) {
        effects->paintWindow(w, mask, region, data);
        return;
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
 * When a window closes, stop tracking it.
 *
 * @brief ShadersEffect::slotWindowClosed
 * @param w
 */
void ShadersEffect::slotWindowClosed(EffectWindow* w) {
    m_windows.removeOne(w);
    updateStatusCount();
}

/**
 * When the user activates the keyboard shortcut, we mark all
 * windows as wanting or not wanting the shader applied to it.
 *
 * @brief ShadersEffect::slotToggleScreenShaders
 */
void ShadersEffect::slotToggleScreenShaders() {
    if (!m_shadersLoaded) {
        return;
    }
    m_allWindows = !m_allWindows;
    m_shadersUI.setAllWindows(m_allWindows);
    updateStatusCount();
    effects->addRepaintFull();
}

/**
 * User toggled shader to all windows from UI.
 *
 * @brief ShadersEffect::slotUIToggledAllWindows
 * @param status
 */
void ShadersEffect::slotUIToggledAllWindows(bool status) {
    if (!m_shadersLoaded) {
        return;
    }
    m_allWindows = status;
    updateStatusCount();
    effects->addRepaintFull();
}

/**
 * When the user activates the keyboard shortcut, we mark the
 * window as wanting or not wanting the shader applied to it.
 *
 * @brief ShadersEffect::slotToggleWindowShaders
 */
void ShadersEffect::slotToggleWindowShaders() {
    if (!m_shadersLoaded || !effects->activeWindow()) {
        return;
    }
    if (m_windows.contains(effects->activeWindow())) {
        m_windows.removeOne(effects->activeWindow());
    } else {
        m_windows.append(effects->activeWindow());
    }
    updateStatusCount();
    effects->activeWindow()->addRepaintFull();
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
    return m_shadersLoaded && (m_allWindows || !m_windows.isEmpty());
}

} // namespace
