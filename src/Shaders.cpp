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

ShadersEffect::ShadersEffect() : m_shader(nullptr), m_allWindows(false)
{
    reconfigure(ReconfigureAll);

    QAction* a = new QAction(this);
    a->setObjectName(QStringLiteral("Shaders"));
    a->setText(i18n("Toggle Shaders Effect"));
    KGlobalAccel::self()->setDefaultShortcut(a, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    KGlobalAccel::self()->setShortcut(a, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_R, a);
    connect(a, &QAction::triggered, this, &ShadersEffect::slotToggleScreenShaders);

    QAction* b = new QAction(this);
    b->setObjectName(QStringLiteral("ShadersWindow"));
    b->setText(i18n("Toggle Shaders Effect on Window"));
    KGlobalAccel::self()->setDefaultShortcut(b, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    KGlobalAccel::self()->setShortcut(b, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_Z, b);
    connect(b, &QAction::triggered, this, &ShadersEffect::slotToggleWindow);

    connect(effects, &EffectsHandler::windowClosed, this, &ShadersEffect::slotWindowClosed);

    // If the setting "Enable by default" is enabled, trigger the effect on first run.
    if (ShadersConfig::defaultEnabled()) {
        a->trigger();
    }
}

ShadersEffect::~ShadersEffect()
{
    delete m_shader;
}

void ShadersEffect::reconfigure(ReconfigureFlags) {
    if (!supported()) {
        return;
    }

    m_shadersLoaded = false;

    ShadersConfig::self()->read();

    // Find path where shader files are in.
    QString shaderPath = ShadersConfig::shaderPath().trimmed();
    if (!shaderPath.endsWith("/")) {
        shaderPath.append("/");
    }
    if (QString::compare(m_shaderPath, shaderPath) != 0) {
        m_foundShaderPath = false;
        if (shaderPath.isEmpty()) {
           return;
        }
        QDir shadersDir(shaderPath);
        if (shadersDir.exists() && shadersDir.isReadable() && !shadersDir.isEmpty()) {
            m_shaderPath = shaderPath;
            m_foundShaderPath = true;
        }
    }

    // Failed to find path where shader files are in.
    if (!m_foundShaderPath) {
        return;
    }

    // Iterate shaders files and append them to their respectful buffers.
    QDir shadersDir(m_shaderPath);
    shadersDir.setFilter(QDir::Files);
    shadersDir.setSorting(QDir::Name | QDir::IgnoreCase);
    QFileInfoList shadersList = shadersDir.entryInfoList();
    QByteArray fragmentBuf, vertexBuf;
    bool foundSettings = false;
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
            return;
        }

        QByteArray shaderBuf = shaderFile.readAll();
        shaderFile.close();

        // Settings file should always be first and in both shader buffers.
        if (!foundSettings && isGlsl && curFile.endsWith(m_settingsName)) {
            fragmentBuf.prepend(shaderBuf);
            vertexBuf.prepend(shaderBuf);
            foundSettings = true;
            continue;
        }

        if (isFrag || isGlsl) {
            fragmentBuf.append(shaderBuf);
        }

        if (isVert || isGlsl) {
            vertexBuf.append(shaderBuf);
        }
    }

    // Didn't find settings file.
    if (!foundSettings) {
        return;
    }

    // Generate the shader.
    m_shader = KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture, vertexBuf, fragmentBuf);

    // Shader is invalid.
    if (!m_shader->isValid()) {
        return;
    }

    // Shader succsesfully generated.
    m_shadersLoaded = true;

    // Monitor changes to the settings file, if modified, re-generate the shader.
    QString tmpSettingsPath = m_settingsName;
    tmpSettingsPath.prepend(m_shaderPath);
    if (QString::compare(tmpSettingsPath, m_settingsPath) != 0) {
        m_settingsWatcher.removePath(m_settingsPath);
        m_settingsPath.clear();
        m_settingsPath.append(tmpSettingsPath);
        disconnect(&m_settingsWatcher);
        if (m_settingsWatcher.addPath(m_settingsPath)) {
            connect(&m_settingsWatcher, &QFileSystemWatcher::fileChanged, this, &ShadersEffect::slotReconfigure);
        }
    }

    // Check if blacklist is enabled.
    QString blacklist = ShadersConfig::blacklist();
    m_blacklist = blacklist.toLower().split(",");
    m_blacklistEn = !blacklist.isEmpty();

    // Check if whitelist is enabled.
    QString whitelist = ShadersConfig::whitelist();
    m_whitelist = whitelist.toLower().split(",");
    m_whitelistEn = !whitelist.isEmpty();
}

bool ShadersEffect::supported()
{
#ifdef KWIN_HAVE_OPENGLES
    return false;
#else
    // GLSL 1.4 needs GL >= 3.1
    return effects->compositingType() == OpenGLCompositing && hasGLVersion(3, 1);
#endif
}

void ShadersEffect::drawWindow(EffectWindow* w, int mask, const QRegion &region, WindowPaintData& data)
{
    bool useShader = m_shadersLoaded && m_allWindows != m_windows.contains(w);
    if (useShader && (m_blacklistEn || m_whitelistEn)) {
        QString windowName = w->windowClass().split(" ")[1].toLower();
        if (m_blacklist.contains(windowName)) {
            useShader = false;
        } else if (m_whitelistEn && !m_whitelist.contains(windowName)) {
            useShader = false;
        }
    }
    if (useShader) {
        ShaderManager *shaderManager = ShaderManager::instance();
        shaderManager->pushShader(m_shader);
        m_shader->setUniform(m_shader->uniformLocation("g_Random"), (float) drand48());
        data.shader = m_shader;
        effects->drawWindow(w, mask, region, data);
        ShaderManager::instance()->popShader();
        return;
    }
    effects->drawWindow(w, mask, region, data);
}

// Reconfigure variables if settings file is modified.
void ShadersEffect::slotReconfigure() {
    reconfigure(ReconfigureAll);
}

void ShadersEffect::slotWindowClosed(EffectWindow* w)
{
    m_windows.removeOne(w);
}

void ShadersEffect::slotToggleScreenShaders()
{
    // Toggle if user wants shaders on all windows or not.
    m_allWindows = !m_allWindows;
    effects->addRepaintFull();
}

void ShadersEffect::slotToggleWindow()
{
    if (!effects->activeWindow()) {
        return;
    }
    // Toggle if user wants shaders on active window or not.
    m_windows.contains(effects->activeWindow()) ?
        (void) m_windows.removeOne(effects->activeWindow()) :
               m_windows.append(effects->activeWindow()
    );
    effects->activeWindow()->addRepaintFull();
}

bool ShadersEffect::isActive() const
{
    return (m_allWindows || !m_windows.isEmpty());
}

} // namespace
