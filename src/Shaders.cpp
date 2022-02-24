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
#ifdef DEBUGON
    logFile.setFileName("/tmp/kwin4_effect_shaders.log");
    logFile.open(QIODevice::Append | QIODevice::Text);
    QTextStream out(&logFile);
    out << "ShadersEffect::ShadersEffect()\n";
#endif
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
#ifdef DEBUGON
    out << "ShadersEffect::ShadersEffect() -> ShadersConfig::defaultEnabled() -> " << ShadersConfig::defaultEnabled() << "\n";
#endif
}

ShadersEffect::~ShadersEffect()
{
#ifdef DEBUGON
    QTextStream out(&logFile);
    out << "ShadersEffect::~ShadersEffect()\n";
    logFile.close();
#endif
    delete m_shader;
}

void ShadersEffect::reconfigure(ReconfigureFlags) {
#ifdef DEBUGON
    QTextStream out(&logFile);
    out << "ShadersEffect::reconfigure()\n";
#endif
    ShadersConfig::self()->read();
    QString blacklist = ShadersConfig::blacklist();
    m_blacklist = blacklist.toLower().split(",");
    m_blacklistEn = !blacklist.isEmpty();
    QString whitelist = ShadersConfig::whitelist();
    m_whitelist = whitelist.toLower().split(",");
    m_whitelistEn = !whitelist.isEmpty();
#ifdef DEBUGON
    out << "ShadersEffect::reconfigure() -> blacklist -> '" << blacklist << "'\n";
    out << "ShadersEffect::reconfigure() -> whitelist -> '" << whitelist << "'\n";
#endif
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

bool ShadersEffect::supported()
{
    // Shaders are version 140
#ifdef KWIN_HAVE_OPENGLES
    return false;
#endif
    return effects->compositingType() == OpenGLCompositing;
}

bool ShadersEffect::loadShaders()
{
#ifdef DEBUGON
    QTextStream out(&logFile);
    out << "ShadersEffect::loadShaders()\n";
#endif
    reconfigure(ReconfigureAll);

    QString fragmentShader("shaders.frag");
    fragmentShader.prepend(shaderPath());
    QFile ffile(fragmentShader);
    if (!ffile.exists() || !ffile.open(QFile::ReadOnly)) {
        return false;
    }

    QString vertexShader("shaders.vert");
    vertexShader.prepend(shaderPath());
    QFile vfile(vertexShader);
    if (!vfile.exists() || !vfile.open(QFile::ReadOnly)) {
        ffile.close();
        return false;
    }

    m_shader = KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture, vfile.readAll(), ffile.readAll());
    ffile.close();
    vfile.close();

    if (m_shader->isValid()) {
        return true;
    }
#ifdef DEBUGON
    out << "ShadersEffect::loadShaders() -> Shader is invalid\n";
#endif
    return false;
}

void ShadersEffect::drawWindow(EffectWindow* w, int mask, const QRegion &region, WindowPaintData& data)
{
#ifdef DEBUGON
    QTextStream out(&logFile);
    out << "ShadersEffect::drawWindow() -> " << w << "\n";
#endif
    bool useShader = m_shader->isValid() && (m_allWindows != m_windows.contains(w));
    if (useShader && (m_blacklistEn || m_whitelistEn)) {
        QString windowName = w->windowClass().split(" ")[1].toLower();
        if (m_blacklist.contains(windowName)) {
#ifdef DEBUGON
            out << "ShadersEffect::drawWindow() -> Blacklist on " << windowName << "\n";
#endif
            useShader = false;
        } else if (m_whitelistEn && !m_whitelist.contains(windowName)) {
#ifdef DEBUGON
            out << "ShadersEffect::drawWindow() -> No whitelist on " << windowName << "\n";
#endif
            useShader = false;
        }
    }
    if (useShader) {
        ShaderManager *shaderManager = ShaderManager::instance();
        shaderManager->pushShader(m_shader);
        data.shader = m_shader;
    }

    effects->drawWindow(w, mask, region, data);

    if (useShader) {
        ShaderManager::instance()->popShader();
    }
}

void ShadersEffect::paintEffectFrame(KWin::EffectFrame* frame, const QRegion &region, double opacity, double frameOpacity)
{
#ifdef DEBUGON
    QTextStream out(&logFile);
    out << "ShadersEffect::paintEffectFrame()\n";
#endif
    effects->paintEffectFrame(frame, region, opacity, frameOpacity);
}

void ShadersEffect::slotWindowClosed(EffectWindow* w)
{
#ifdef DEBUGON
    QTextStream out(&logFile);
    out << "ShadersEffect::slotWindowClosed()\n";
#endif
    m_windows.removeOne(w);
}

void ShadersEffect::toggleScreenShaders()
{
#ifdef DEBUGON
    QTextStream out(&logFile);
    out << "ShadersEffect::toggleScreenShaders()\n";
#endif
    m_allWindows = !m_allWindows;
    // Only load shaders if enabled.
    if (m_allWindows) {
        loadShaders();
    }
    effects->addRepaintFull();
}

void ShadersEffect::toggleWindow()
{
#ifdef DEBUGON
    QTextStream out(&logFile);
    out << "ShadersEffect::toggleWindow()\n";
#endif
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
#ifdef DEBUGON
    QTextStream out(&logFile);
    out << "ShadersEffect::provides()\n";
#endif
    return f == Nothing;
}

} // namespace
