/********************************************************************
 KWin - the KDE window manager
 This file is part of the KDE project.

Copyright (C) 2007 Rivo Laks <rivolaks@hot.ee>

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************/

#include "shaders_config.h"
// KConfigSkeleton
#include "shadersconfig.h"

#include <kwineffects_interface.h>

#include <QAction>

#include <KGlobalAccel>
#include <KLocalizedString>
#include <KActionCollection>
#include <KShortcutsEditor>
#include <KPluginMetaData>
#include <KPluginFactory>

#include <QVBoxLayout>

#include <QLineEdit>

K_PLUGIN_FACTORY_WITH_JSON(ShadersEffectConfigFactory,
                           "shadersConfig.json",
                           registerPlugin<KWin::ShadersEffectConfig>();)

namespace KWin
{
    ShadersEffectConfigForm::ShadersEffectConfigForm(QWidget* parent) : QWidget(parent)
    {
        setupUi(this);
    }


ShadersEffectConfig::ShadersEffectConfig(QWidget* parent, const QVariantList& args) :
    KCModule(parent, args)
{
    QVBoxLayout* layout = new QVBoxLayout(this);
    m_ui = new ShadersEffectConfigForm(this);
    layout->addWidget(m_ui);

    // Shortcut config. The shortcut belongs to the component "kwin"!
    KActionCollection *actionCollection = new KActionCollection(this, QStringLiteral("kwin"));
    actionCollection->setComponentDisplayName(i18n("KWin"));

    QAction* a = actionCollection->addAction(QStringLiteral("Shaders"));
    a->setText(i18n("Toggle shaders Effect on all Windows"));
    a->setProperty("isConfigurationAction", true);
    KGlobalAccel::self()->setDefaultShortcut(a, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);
    KGlobalAccel::self()->setShortcut(a, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_R);

    QAction* b = actionCollection->addAction(QStringLiteral("ShadersWindow"));
    b->setText(i18n("Toggle shaders Effect on Window"));
    b->setProperty("isConfigurationAction", true);
    KGlobalAccel::self()->setDefaultShortcut(b, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);
    KGlobalAccel::self()->setShortcut(b, QList<QKeySequence>() << Qt::CTRL + Qt::META + Qt::Key_Z);

    m_ui->shortcutEditor->addCollection(actionCollection);
    connect(m_ui->shortcutEditor, &KShortcutsEditor::keyChange, this, &ShadersEffectConfig::markAsChanged);

    ShadersConfig::instance("kwinrc");
    addConfig(ShadersConfig::self(), m_ui);

    load();
}

ShadersEffectConfig::~ShadersEffectConfig()
{
    // Undo (only) unsaved changes to global key shortcuts
    m_ui->shortcutEditor->undo();
}

void ShadersEffectConfig::save()
{
    KCModule::save();

    m_ui->shortcutEditor->save();    // undo() will restore to this state from now on

    OrgKdeKwinEffectsInterface interface(QStringLiteral("org.kde.KWin"),
                                         QStringLiteral("/Effects"),
                                         QDBusConnection::sessionBus());
    interface.reconfigureEffect(QStringLiteral("kwin_effect_shaders"));
}

void ShadersEffectConfig::defaults()
{
    m_ui->shortcutEditor->allDefault();
    KCModule::defaults();
}


} // namespace

#include "shaders_config.moc"
