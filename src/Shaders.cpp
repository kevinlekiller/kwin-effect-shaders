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

#include "Shaders.h"
#include <QAction>
#include <QDir>
#include <QFile>
#include <kwinglplatform.h>
#include <kwinglutils.h>
#include <KGlobalAccel>
#include <KLocalizedString>

Q_LOGGING_CATEGORY(KWIN_SHADERS, "kwin_effect_shaders", QtWarningMsg)

namespace KWin {

/**
 * @brief Construct.
 */
ShadersEffect::ShadersEffect() : m_shader(nullptr), m_effectEnabled(false) {
    // Initialize settings.
    m_settings = new QSettings("kevinlekiller", "kwin_effect_shaders");
    if (m_settings->value("Blacklist") == QVariant()) {
        m_settings->setValue("Blacklist", "");
    }
    if (m_settings->value("Whitelist") == QVariant()) {
        m_settings->setValue("Whitelist", "");
    }
    if (m_settings->value("AutoEnable") == QVariant()) {
        m_settings->setValue("AutoEnable", false);
    }
    m_settings->sync();
    if (m_settingsWatcher.addPath(m_settings->fileName())) {
        connect(&m_settingsWatcher, &QFileSystemWatcher::fileChanged, this, &ShadersEffect::slotSettingsFileChanged);
    }

    // Fetch settings.
    slotSettingsFileChanged();

    // Setup keyboard shortcuts.
    QAction* toggleEffectShortcut = new QAction(this);
    toggleEffectShortcut->setObjectName(QStringLiteral("ShadersToggle"));
    toggleEffectShortcut->setText(i18n("Shaders Effect: Toggle On or Off the effect"));
    KGlobalAccel::self()->setDefaultShortcut(toggleEffectShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    KGlobalAccel::self()->setShortcut(toggleEffectShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_Z, toggleEffectShortcut);

    QAction* reloadShadersShortcut = new QAction(this);
    reloadShadersShortcut->setObjectName(QStringLiteral("ShadersReload"));
    reloadShadersShortcut->setText(i18n("Shaders Effect: Forces a reload of the shader files"));
    KGlobalAccel::self()->setDefaultShortcut(reloadShadersShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_A);
    KGlobalAccel::self()->setShortcut(reloadShadersShortcut, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_A);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_A, reloadShadersShortcut);

    // Setup connections.
    connect(toggleEffectShortcut, &QAction::triggered, this, &ShadersEffect::slotShortcutToggleEffect);
    connect(reloadShadersShortcut, &QAction::triggered, this, &ShadersEffect::slotPopulateShaderBuffers);

    // If the setting "Auto Enable" is enabled, trigger the effect on first run.
    if (m_settings->value("AutoEnable").toBool()) {
        toggleEffectShortcut->trigger();
    }
}

/**
 * @brief Destruct.
 */
ShadersEffect::~ShadersEffect() {
    delete m_settings;
    delete m_shader;
}

/**
 * @brief Process user specified white or black list.
 * The list should be the names of window names seperated by a comma.
 *
 * @param list        -> The list.
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
        m_whitelistEn = !list.isEmpty();
    } else {
        listType = "Blacklist";
        m_blacklist = llist;
        m_blacklistEn = !list.isEmpty();
    }
    if (QString::compare(list, m_settings->value(listType).toString()) != 0) {
        m_settings->setValue(listType, list);
        m_settings->sync();
    }
}

/**
 * @brief Process user specified shader path.
 * The path should contain glsl files ending with glsl, frag and vert extensions.
 * The path must contain the 1_settings.glsl.example file.
 * If the path looks valid, the slotPopulateShaderBuffers() function is executed.
 *
 * @param shaderPath -> The shader path to process.
 */
void ShadersEffect::processShaderPath(QString shaderPath) {
    shaderPath = shaderPath.trimmed();
    if (shaderPath.isEmpty()) {
        shaderPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, "kwin-effect-shaders_shaders", QStandardPaths::LocateDirectory);
        if (shaderPath.isEmpty()) {
            resetEffect();
            return;
        }
    }
    if (!shaderPath.endsWith("/")) {
        shaderPath.append("/");
    }
    if (QString::compare(m_shaderPath, shaderPath) == 0) {
        return;
    }
    m_shaderPath = shaderPath;
    QDir shadersDir(shaderPath);
    if (!shadersDir.isReadable()) {
        resetEffect();
        return;
    }
    if (m_shaderSettingsWatcher.files().contains(m_shaderSettingsPath)) {
        m_shaderSettingsWatcher.removePath(m_shaderSettingsPath);
        disconnect(&m_shaderSettingsWatcher, &QFileSystemWatcher::fileChanged, this, &ShadersEffect::slotPopulateShaderBuffers);
    }
    m_shaderSettingsPath = m_shaderPath;
    m_shaderSettingsPath.append(m_shaderSettingsName);
    if (!shadersDir.exists(m_shaderSettingsName)) {
        QString exampleName = m_shaderSettingsPath;
        exampleName.append(".example");
        QFile exampleFile(exampleName);
        if (!exampleFile.exists() || !exampleFile.copy(m_shaderSettingsPath)) {
            exampleFile.close();
            resetEffect();
            return;
        }
        exampleFile.close();
    }
    m_settings->setValue("ShaderPath", m_shaderPath);
    if (m_shaderSettingsWatcher.addPath(m_shaderSettingsPath)) {
        connect(&m_shaderSettingsWatcher, &QFileSystemWatcher::fileChanged, this, &ShadersEffect::slotPopulateShaderBuffers);
    }
    slotPopulateShaderBuffers();
}

/**
 * @brief If any step of the shader detection / generation process fails,
 * some variables are reset to their default.
 */
void ShadersEffect::resetEffect() {
    m_effectEnabled = false;
    m_shadersLoaded = false;
    effects->addRepaintFull();
}

/**
 * @brief Reads shader files from m_shaderPath and stores in them in buffer.
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
            resetEffect();
            return;
        }
        QByteArray shaderBuf = shaderFile.readAll();
        shaderFile.close();

        QHash <qint64, QByteArray> shaderHash;
        shaderHash.insert(lastModified, shaderBuf);
        m_shaderArr.insert(curFile, shaderHash);
    }
    slotGenerateShaderFromBuffers();
}

/**
 * @brief Try to compile the shader from the buffers.
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
        QByteArray value = shaders.value().values().first();
        if (curFile.endsWith(m_shaderSettingsName)) {
            fragmentBuf.prepend(value);
            vertexBuf.prepend(value);
            continue;
        }
        bool isGlsl = curFile.endsWith(".glsl");
        if (isGlsl || curFile.endsWith(".frag")) {
            fragmentBuf.append(value);
        }
        if (isGlsl || curFile.endsWith(".vert")) {
            vertexBuf.append(value);
        }
    }
    m_shader = ShaderManager::instance()->generateCustomShader(ShaderTrait::MapTexture, vertexBuf, fragmentBuf);

    // Shader is invalid.
    if (!m_shader->isValid()) {
        resetEffect();
        return;
    }
    m_shadersLoaded = true;
    effects->addRepaintFull();
}

void ShadersEffect::slotSettingsFileChanged() {
    m_settings->sync();
    processBWList(m_settings->value("Blacklist").toString(), false);
    processBWList(m_settings->value("Whitelist").toString(), true);
    processShaderPath(m_settings->value("ShaderPath").toString());
}

/**
 * @brief Tells KWin if the plugin should be loaded or not.
 *
 * @return bool -> User has support for GLSL version 140 (or 300 if GLES).
 */
bool ShadersEffect::supported() {
    if (effects->compositingType() != OpenGLCompositing ||
        (GLPlatform::instance()->isGLES() && GLPlatform::instance()->glslVersion() < 300) ||
        GLPlatform::instance()->glslVersion() < 140
    ) {
        return false;
    }
    return true;
}

/**
 * @brief Reimplements: KWin::EffectsHandler::prepPaintWindow
 *
 * @param w
 * @param data
 * @param presentTime
 */
void ShadersEffect::prePaintWindow(EffectWindow* w, WindowPrePaintData& data, std::chrono::milliseconds presentTime) {
    effects->prePaintWindow(w, data, presentTime);
}

/**
 * @brief Reimplements: KWin::EffectsHandler::paintWindow
 *
 * @param w
 * @param mask
 * @param region
 * @param data
 */
void ShadersEffect::paintWindow(EffectWindow* w, int mask, const QRegion region, WindowPaintData& data) {
    // Check if window is blacklisted or whitelisted.
    if (m_blacklistEn || m_whitelistEn) {
        QString windowName = w->windowClass().trimmed().split(" ")[0];
        if ((m_blacklistEn && m_blacklist.contains(windowName, Qt::CaseInsensitive)) || (m_whitelistEn && !m_whitelist.contains(windowName, Qt::CaseInsensitive))) {
            effects->paintWindow(w, mask, region, data);
            return;
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
 * @brief Reimplements: KWin::EffectsHandler::postPaintWindow
 *
 * @param w
 */
void ShadersEffect::postPaintWindow(EffectWindow* w) {
    effects->postPaintWindow(w);
}

/**
 * @brief When the user activates the keyboard shortcut, enable or disable the effect.
 */
void ShadersEffect::slotShortcutToggleEffect() {
    slotToggleEffect(!m_effectEnabled);
}

/**
 * @brief User wants to enable or disable the effect.
 * @param status
 */
void ShadersEffect::slotToggleEffect(bool status) {
    if (!m_shadersLoaded) {
        return;
    }
    m_effectEnabled = status;
    effects->addRepaintFull();
}

/**
 * @brief Return if the effect is enabled.
 * This determines if the overriden paint functions are executed or not.
 *
 * @return bool -> The effect is enabled or not.
 */
bool ShadersEffect::isActive() const {
    return m_effectEnabled;
}

} // namespace
