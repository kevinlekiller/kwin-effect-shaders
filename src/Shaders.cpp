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
    :   m_inited(false),
        m_valid(true),
        m_shader(nullptr),
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
}

ShadersEffect::~ShadersEffect()
{
    delete m_shader;
}

void ShadersEffect::reconfigure(ReconfigureFlags) {
    ShadersConfig::self()->read();
    m_blacklist = ShadersConfig::blacklist().toLower().split(",");
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

bool ShadersEffect::loadData()
{
    reconfigure(ReconfigureAll);
    m_inited = true;

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
    return false;
}

QString ShadersEffect::getWindowApplicationName(EffectWindow * w) {
    auto windowClass = w->windowClass();
    return windowClass.split(" ")[1].toLower();
}

void ShadersEffect::drawWindow(EffectWindow* w, int mask, const QRegion &region, WindowPaintData& data)
{
    // Load if we haven't already
    if (m_valid && !m_inited) {
        m_valid = loadData();
    }
    if (!m_valid) {
        return;
    }

    bool useShader =
        m_valid &&
        (m_allWindows != m_windows.contains(w)) &&
        !m_blacklist.contains(getWindowApplicationName(w)
    );
    if (useShader) {
        auto shader = m_windows_shader.value(w, m_shader);
        ShaderManager *shaderManager = ShaderManager::instance();
        shaderManager->pushShader(shader);
        data.shader = shader;
    }

    effects->drawWindow(w, mask, region, data);

    if (useShader) {
        ShaderManager::instance()->popShader();
    }
}

void ShadersEffect::paintEffectFrame(KWin::EffectFrame* frame, const QRegion &region, double opacity, double frameOpacity)
{
    if (m_valid && m_allWindows) {
        frame->setShader(m_shader);
        ShaderBinder binder(m_shader);
        effects->paintEffectFrame(frame, region, opacity, frameOpacity);
    } else {
        effects->paintEffectFrame(frame, region, opacity, frameOpacity);
    }
}

void ShadersEffect::slotWindowClosed(EffectWindow* w)
{
    m_windows_shader.remove(w);
    m_windows.removeOne(w);
}

void ShadersEffect::toggleScreenShaders()
{
    m_valid = loadData(); //hotswap
    if (!m_valid) {
        return;
    }
    m_windows_shader.clear();
    m_allWindows = !m_allWindows;
    effects->addRepaintFull();
}

void ShadersEffect::toggleWindow()
{
    if (!effects->activeWindow()) {
        return;
    }
    m_valid = loadData();
    if (!m_valid) {
        return;
    }
    m_windows_shader.insert(effects->activeWindow(), m_shader);
    if (!m_windows.contains(effects->activeWindow())) {
        m_windows.append(effects->activeWindow());
    } else {
        m_windows.removeOne(effects->activeWindow());
    }
    effects->activeWindow()->addRepaintFull();
}

bool ShadersEffect::isActive() const
{
    return m_valid && (m_allWindows || !m_windows.isEmpty());
}

bool ShadersEffect::provides(Feature f)
{
    return f == Nothing;
}

} // namespace
