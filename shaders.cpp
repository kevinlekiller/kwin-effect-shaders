/*
    KWin - the KDE window manager
    This file is part of the KDE project.
    SPDX-FileCopyrightText: 2007 Rivo Laks <rivolaks@hot.ee>
    SPDX-FileCopyrightText: 2008 Lucas Murray <lmurray@undefinedfire.com>
    SPDX-License-Identifier: GPL-2.0-or-later
*/

#include "shaders.h"
#include "shadersconfig.h"
#include <QAction>
#include <QFile>
#include <kwinglutils.h>
#include <kwinglplatform.h>
#include <KGlobalAccel>
#include <KLocalizedString>
#include <QStandardPaths>

#include <QMatrix4x4>

Q_LOGGING_CATEGORY(KWIN_SHADERS, "kwin_effect_shaders", QtWarningMsg)

namespace KWin
{

ShadersEffect::ShadersEffect()
    :   m_inited(false),
        m_valid(true),
        m_shader(nullptr),
        m_allWindows(false)
{
    initConfig<ShadersConfig>();
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

    reconfigure(ReconfigureAll);
    connect(effects, &EffectsHandler::windowClosed, this, &ShadersEffect::slotWindowClosed);
}

ShadersEffect::~ShadersEffect()
{
    delete m_shader;
}

void ShadersEffect::reconfigure(ReconfigureFlags) {
    ShadersConfig::self()->read();
    m_blocklist = ShadersConfig::blocklist().toLower().split(",");
}

bool ShadersEffect::supported()
{
    return effects->compositingType() == OpenGLCompositing;
}

/*GLShader* ShadersEffect::readShader() {
    auto shader = m_shader;
    if (KWin::GLPlatform::instance()->glslVersion() != KWin::kVersionNumber(1, 40)) {
        return nullptr;
    }
    const QString fragmentshader = QStandardPaths::locate(QStandardPaths::GenericDataLocation, QStringLiteral("kwin/shaders/1.40/shaders.frag"));
    QFile file(fragmentshader);
    if (!file.open(QFile::ReadOnly)) {
        return nullptr;
    }
    QByteArray frag = file.readAll();
    shader = KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture, QByteArray(), frag);
    file.close();
    return shader;
}

bool ShadersEffect::loadData()
{
    m_inited = true;
    m_shader = readShader();
    return m_shader->isValid();
}*/

bool ShadersEffect::loadData()
{
  m_inited = true;

  QString shadersDir(QStringLiteral("kwin/shaders/1.10/"));
#ifdef KWIN_HAVE_OPENGLES
  const qint64 coreVersionNumber = kVersionNumber(3, 0);
#else
  const qint64 version = KWin::kVersionNumber(1, 40);
#endif
  if (KWin::GLPlatform::instance()->glslVersion() >= version)
    shadersDir = QStringLiteral("kwin/shaders/1.40/");

  const QString fragmentshader = QStandardPaths::locate(QStandardPaths::GenericDataLocation, shadersDir + QStringLiteral("shaders.frag"));


  QFile file(fragmentshader);
  if (file.open(QFile::ReadOnly))
  {
    QByteArray frag = file.readAll();
    m_shader = KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture, QByteArray(), frag);
    file.close();

    if (!m_shader->isValid()) {
      //qCCritical(KWINEFFECTS) << "The shader failed to load!";
      return false;
    }
    return true;
  }
  else {
    deleteLater();
    return false;
  }
}

/**
 * Returns shader from package kwin/shaders/1.40/.
 * **/
GLShader* ShadersEffect::readShader()
{
  auto shader = m_shader;
  QString shadersDir(QStringLiteral("kwin/shaders/1.10/"));
#ifdef KWIN_HAVE_OPENGLES
  const qint64 coreVersionNumber = kVersionNumber(3, 0);
#else
  const qint64 version = KWin::kVersionNumber(1, 40);
#endif
  if (KWin::GLPlatform::instance()->glslVersion() >= version)
    shadersDir = QStringLiteral("kwin/shaders/1.40/");

  const QString fragmentshader = QStandardPaths::locate(QStandardPaths::GenericDataLocation, shadersDir + QStringLiteral("shaders.frag"));


  QFile file(fragmentshader);
  if (file.open(QFile::ReadOnly))
  {
    QByteArray frag = file.readAll();
    shader = KWin::ShaderManager::instance()->generateCustomShader(KWin::ShaderTrait::MapTexture, QByteArray(), frag);
    file.close();}
  return shader;
}


QString ShadersEffect::getWindowApplicationName(EffectWindow * w) {
    auto windowClass = w->windowClass();
    return windowClass.split(" ")[1].toLower();
}

void ShadersEffect::drawWindow(EffectWindow* w, int mask, const QRegion &region, WindowPaintData& data)
{
    // Load if we haven't already
    if (m_valid && !m_inited)
        m_valid = loadData();

    bool useShader = m_valid && (m_allWindows != m_windows.contains(w)) && !m_blocklist.contains(getWindowApplicationName(w));
    auto shader = m_windows_shader.value(w, m_shader);
    if (useShader) {
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
    m_windows_shader.clear();
    m_allWindows = !m_allWindows;
    effects->addRepaintFull();
}

void ShadersEffect::toggleWindow()
{
    if (!effects->activeWindow()) {
        return;
    }
    m_windows_shader.insert(effects->activeWindow(), readShader());
    if (!m_windows.contains(effects->activeWindow()))
        m_windows.append(effects->activeWindow());
    else
        m_windows.removeOne(effects->activeWindow());
    effects->activeWindow()->addRepaintFull();
}

bool ShadersEffect::isActive() const
{
    return m_valid && (m_allWindows || !m_windows.isEmpty());
}

bool ShadersEffect::provides(Feature f)
{
    return (f == ScreenInversion || f != ScreenInversion);
}

} // namespace
