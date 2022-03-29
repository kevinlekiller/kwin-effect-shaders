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
#include <QFileSystemWatcher>
#include <kwinglutils.h>
#include <kwinglplatform.h>
#include <KGlobalAccel>
#include <KLocalizedString>
#include <QStandardPaths>
#include <QMatrix4x4>

Q_LOGGING_CATEGORY(KWIN_SHADERS, "kwin4_effect_shaders", QtWarningMsg)

namespace KWin
{

ShadersEffect::ShadersEffect()
    :   m_shader(nullptr),
        m_allWindows(false)
{
    reconfigure(ReconfigureAll);

    QAction* a = new QAction(this);
    a->setObjectName(QStringLiteral("Shaders"));
    a->setText(i18n("Toggle Shaders Effect"));
    KGlobalAccel::self()->setDefaultShortcut(a, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    KGlobalAccel::self()->setShortcut(a, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_R, a);
    connect(a, &QAction::triggered, this, &ShadersEffect::toggleScreenShaders);

    QAction* b = new QAction(this);
    b->setObjectName(QStringLiteral("ShadersWindow"));
    b->setText(i18n("Toggle Shaders Effect on Window"));
    KGlobalAccel::self()->setDefaultShortcut(b, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    KGlobalAccel::self()->setShortcut(b, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    effects->registerGlobalShortcut(Qt::CTRL + Qt::META + Qt::Key_Z, b);
    connect(b, &QAction::triggered, this, &ShadersEffect::toggleWindow);

    connect(effects, &EffectsHandler::windowClosed, this, &ShadersEffect::slotWindowClosed);

    if (ShadersConfig::defaultEnabled()) {
        a->trigger();
    }
}

ShadersEffect::~ShadersEffect()
{
    delete m_shader;
}

void ShadersEffect::reconfigure(ReconfigureFlags) {
    m_shadersLoaded = false;
    ShadersConfig::self()->read();
    QString blacklist = ShadersConfig::blacklist();
    m_blacklist = blacklist.toLower().split(",");
    m_blacklistEn = !blacklist.isEmpty();
    QString whitelist = ShadersConfig::whitelist();
    m_whitelist = whitelist.toLower().split(",");
    m_whitelistEn = !whitelist.isEmpty();
    QString shaderPath = ShadersConfig::shaderPath().trimmed();
    bool foundPath = false;
    if (!shaderPath.isEmpty()) {
        QDir dir = QDir(shaderPath);
        if (dir.exists() && dir.isReadable() && !dir.isEmpty()) {
            m_shaderPath = shaderPath;
            foundPath = true;
        }
    }
    if (!foundPath) {
        m_shaderPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kwin/shaders"), QStandardPaths::LocateDirectory);
    }
    if (!m_shaderPath.endsWith("/")) {
        m_shaderPath.append("/");
    }
}

void ShadersEffect::slotReloadShader() {
    loadShaders();
}

bool ShadersEffect::supported()
{
    // Shaders are version 140
#ifdef KWIN_HAVE_OPENGLES
    return false;
#endif
    return effects->compositingType() == OpenGLCompositing;
}

void ShadersEffect::loadShaders()
{
    reconfigure(ReconfigureAll);
    QByteArray fragmentBuf, vertexBuf;

    // Read settings file and put it in both the fragment and vertex buffers.
    QString curFile = shaderPath();
    curFile.append(m_settingsName);
    QFile file(curFile);
    if (!file.exists() || !file.open(QFile::ReadOnly)) {
        file.close();
        return;
    }
    fragmentBuf.append(file.readAll());
    vertexBuf.append(fragmentBuf);
    file.close();

    // Iterate all fragment and vertex files and append them to the buffers.
    QDir shadersDir(shaderPath());
    shadersDir.setFilter(QDir::Files);
    shadersDir.setSorting(QDir::Name | QDir::IgnoreCase);
    QFileInfoList shadersList = shadersDir.entryInfoList();
    for (int i = 0; i < shadersList.size(); ++i) {
        QFileInfo shaderInfo = shadersList.at(i);
        curFile = shaderInfo.absoluteFilePath();
        bool isFrag = curFile.endsWith(".frag");
        if (!isFrag && !curFile.endsWith(".vert")) {
            continue;
        }
        QFile file(curFile);
        if (!file.exists() || !file.open(QFile::ReadOnly)) {
            file.close();
            return;
        }
        isFrag ? fragmentBuf.append(file.readAll()) : vertexBuf.append(file.readAll());
        file.close();
    }

    // Generate the shader.
    m_shader = KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture, vertexBuf, fragmentBuf);

    // Monitor changes to the settings file, if modified, re-generate the shader.
    if (m_shader->isValid()) {
        QString tmpSettingsPath = m_settingsName;
        tmpSettingsPath.prepend(shaderPath());
        if (QString::compare(tmpSettingsPath, m_settingsPath) != 0) {
            m_settingsWatcher.removePath(m_settingsPath);
            m_settingsPath.clear();
            m_settingsPath.append(tmpSettingsPath);
            disconnect(&m_settingsWatcher);
            if (m_settingsWatcher.addPath(m_settingsPath)) {
                connect(&m_settingsWatcher, &QFileSystemWatcher::fileChanged, this, &ShadersEffect::slotReloadShader);
            }
        }
        m_shadersLoaded = true;
    }
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

void ShadersEffect::paintEffectFrame(KWin::EffectFrame* frame, const QRegion &region, double opacity, double frameOpacity)
{
    effects->paintEffectFrame(frame, region, opacity, frameOpacity);
}

void ShadersEffect::slotWindowClosed(EffectWindow* w)
{
    m_windows.removeOne(w);
}

void ShadersEffect::toggleScreenShaders()
{
    m_allWindows = !m_allWindows;
    // Only load shaders if enabled.
    if (m_allWindows) {
        loadShaders();
    }
    effects->addRepaintFull();
}

void ShadersEffect::toggleWindow()
{
    if (!effects->activeWindow()) {
        return;
    }
    // Check if the current window already has shaders on it.
    if (m_windows.contains(effects->activeWindow())) {
        m_windows.removeOne(effects->activeWindow());
    } else {
        loadShaders();
        m_windows.append(effects->activeWindow());
    }
    effects->activeWindow()->addRepaintFull();
}

bool ShadersEffect::isActive() const
{
    return (m_allWindows || !m_windows.isEmpty());
}

bool ShadersEffect::provides(Feature f)
{
    return f == Nothing;
}

} // namespace
