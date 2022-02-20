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

#ifndef KWIN_SHADERS_CONFIG_H
#define KWIN_SHADERS_CONFIG_H

#include <kcmodule.h>
#include "ui_shaders_config.h"

class KShortcutsEditor;

namespace KWin
{

class ShadersEffectConfigForm : public QWidget, public Ui::ShadersEffectConfigForm
    {
        Q_OBJECT
        public:
            explicit ShadersEffectConfigForm(QWidget* parent);
};
class ShadersEffectConfig : public KCModule
{
    Q_OBJECT
public:
    explicit ShadersEffectConfig(QWidget* parent = nullptr, const QVariantList& args = QVariantList());
    ~ShadersEffectConfig() override;

public Q_SLOTS:
    void save() override;
    void defaults() override;

private:
    ShadersEffectConfigForm* m_ui;
};

} // namespace

#endif
