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

#include "ShadersUI.h"
#include "ui_ShadersUI.h"
#include <QPushButton>
#include <QRegularExpression>

/**
 * Construct.
 *
 * @brief ShadersUI::ShadersUI
 * @param parent
 */
ShadersUI::ShadersUI(QWidget *parent) : QDialog(parent), ui(new Ui::ShadersUI) {
    ui->setupUi(this);
    connect(ui->button_CloseWindow, &QDialogButtonBox::clicked, this, &ShadersUI::slotHideWindow);
    connect(ui->button_AdvancedSave, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderSaveRequested);
    connect(ui->button_OrderSave, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderSaveRequested);
    connect(ui->button_ShadersSave, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderSaveRequested);
    connect(ui->button_AdvancedApply, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderTestRequested);
    connect(ui->button_OrderApply, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderTestRequested);
    connect(ui->button_ShadersApply, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderTestRequested);
    connect(ui->button_SettingsSave, &QDialogButtonBox::clicked, this, &ShadersUI::slotSettingsSaveRequested);
    connect(ui->val_EnableEffect, &QCheckBox::stateChanged, this, &ShadersUI::slotToggleEffect);
    connect(ui->button_MoveShaderUp, &QPushButton::clicked, this, &ShadersUI::slotMoveShaderUp);
    connect(ui->button_MoveShaderDown, &QPushButton::clicked, this, &ShadersUI::slotMoveShaderDown);
    connect(ui->table_Shaders, &QTableWidget::cellClicked, this, &ShadersUI::slotToggleShader);
    connect(ui->table_Shaders, &QTableWidget::itemChanged, this, &ShadersUI::slotEditShaderSetting);
}

/**
 * Destruct.
 *
 * @brief ShadersUI::~ShadersUI
 */
ShadersUI::~ShadersUI() {
    delete ui;
}

/**
 * Hides the UI.
 *
 * @brief ShadersUI::slotHideWindow
 */
void ShadersUI::slotHideWindow() {
    //m_UIPosition = pos();
    hide();
    emit signalWindowHidden();
}

/**
 * Set the text on the save buttons.
 *
 * @brief ShadersUI::setSaveButtonText
 * @param modified
 */
void ShadersUI::setSaveButtonText(bool modified) {
    QString saveText = modified ? "(*) Save" : "Save";
    ui->button_AdvancedSave->button(QDialogButtonBox::Save)->setText(saveText);
    ui->button_ShadersSave->button(QDialogButtonBox::Save)->setText(saveText);
    ui->button_OrderSave->button(QDialogButtonBox::Save)->setText(saveText);
}

/**
 * User requested saving the shader settings.
 *
 * @brief ShadersUI::slotShaderSaveRequested
 */
void ShadersUI::slotShaderSaveRequested() {
    setUIShaderValues();
    setSaveButtonText(false);
    emit signalShaderSaveRequested();
}

/**
 * User requested testing shader settings.
 *
 * @brief ShadersUI::slotShaderTestRequested
 */
void ShadersUI::slotShaderTestRequested() {
    setUIShaderValues();
    setSaveButtonText(true);
    emit signalShaderTestRequested();
}

/**
 * User requested moving shader up in order list.
 *
 * @brief ShadersUI::slotMoveShaderUp
 */
void ShadersUI::slotMoveShaderUp() {
    int row = ui->val_ShaderOrder->currentRow();
    QListWidgetItem *item = ui->val_ShaderOrder->takeItem(row);
    ui->val_ShaderOrder->insertItem(--row, item);
    ui->val_ShaderOrder->setCurrentRow(row);
}

/**
 * User requested moving shader down in order list.
 *
 * @brief ShadersUI::slotMoveShaderDown
 */
void ShadersUI::slotMoveShaderDown() {
    int row = ui->val_ShaderOrder->currentRow();
    QListWidgetItem *item = ui->val_ShaderOrder->takeItem(row);
    ui->val_ShaderOrder->insertItem(++row, item);
    ui->val_ShaderOrder->setCurrentRow(row);
}

/**
 * User requested saving main settings.
 *
 * @brief ShadersUI::slotSettingsSaveRequested
 */
void ShadersUI::slotSettingsSaveRequested() {
    emit signalSettingsSaveRequested();
}

/**
 * User requested to enable the effect.
 *
 * @brief ShadersUI::slotToggleEffect
 * @param state
 */
void ShadersUI::slotToggleEffect(int state) {
    bool checked = state == Qt::Checked ? true : false;
    setEffectEnabled(checked);
    emit signalEffectToggled(checked);
}

/**
 * User requested to enable or disable a shader.
 *
 * @brief ShadersUI::slotToggleShader
 * @param row
 * @param column
 */
void ShadersUI::slotToggleShader(int row, int column) {
    QString settingName = ui->table_Shaders->item(row, 0)->text().trimmed();
    if (!settingName.startsWith("SHADER_")) {
        return;
    }

    QString value = ui->table_Shaders->item(row, 1)->text();
    int on = QString::compare(value, "On");
    if (on != 0 && QString::compare(value, "Off") != 0) {
        return;
    }

    QString replacement = ui->table_Shaders->item(row, 0)->text().remove(0, 7);
    QString shadersText = ui->val_ShadersText->toPlainText();
    QString regex = "^#define\\s+";
    regex.append(replacement).append("_ENABLED\\s+\\d+");
    QRegularExpression replaceRegex(regex);
    replaceRegex.setPatternOptions(QRegularExpression::MultilineOption);
    replacement.prepend("#define ").append("_ENABLED ").append(on == 0 ? "0" : "1");
    shadersText.replace(replaceRegex, replacement);
    setShadersText(shadersText);
}

/**
 * User wants to edit a shader setting.
 *
 * @brief ShadersUI::slotEditShaderSetting
 * @param item
 */
void ShadersUI::slotEditShaderSetting(QTableWidgetItem *item) {
    if (item->column() == 0) {
        return;
    }

    QString settingName = ui->table_Shaders->item(item->row(), 0)->text().trimmed();
    if (settingName.startsWith("SHADER_")) {
        return;
    }

    QString settingValue = item->text().trimmed();
    if (settingValue.isEmpty()) {
        return;
    }

    QString replacement, regex;
    if (settingValue.startsWith("vec3(")) {
        replacement.append("uniform vec3 ").append(settingName).append(" = ").append(settingValue).append(";");
        regex.append("^uniform\\s+vec3\\s+").append(settingName).append("\\s+=\\s+.+?").append(";");
    } else {
        replacement.append("#define ").append(settingName).append(" ").append(settingValue);
        regex.append("^#define\\s+").append(settingName).append("\\s+[\\d.]+");
    }
    QRegularExpression replaceRegex(regex);
    replaceRegex.setPatternOptions(QRegularExpression::MultilineOption);
    QString shadersText = ui->val_ShadersText->toPlainText();
    shadersText.replace(replaceRegex, replacement);
    setShadersText(shadersText);
}

/**
 * Displays the UI if it's not hidden.
 *
 * @brief ShadersUI::displayUI
 */
void ShadersUI::displayUI() {
    if (!isHidden()) {
        open();
        return;
    }
    /* https://github.com/kevinlekiller/kwin-effect-shaders/issues/2
    if (!m_UIPosition.isNull()) {
        move(m_UIPosition);
    }*/
    show();
}

/**
 * Sets the blacklist to the UI.
 *
 * @brief ShadersUI::setBlacklist
 * @param blacklist
 */
void ShadersUI::setBlacklist(QString blacklist) {
    ui->val_Blacklist->setPlainText(blacklist);
}

/**
 * Sets the whitelist to the UI.
 *
 * @brief ShadersUI::setWhitelist
 * @param whitelist
 */
void ShadersUI::setWhitelist(QString whitelist) {
    ui->val_Whitelist->setPlainText(whitelist);
}

/**
 * Sets the shader path on the UI.
 *
 * @brief ShadersUI::setShaderPath
 * @param shaderpath
 */
void ShadersUI::setShaderPath(QString shaderpath) {
    if (shaderpath.endsWith("/")) {
        shaderpath.chop(1);
    }
    ui->val_ShaderPath->setPlainText(shaderpath);
}

/**
 * Sets if the "defaut enabled" option is enabled to the UI.
 *
 * @brief ShadersUI::setDefaultEnabled
 * @param value
 */
void ShadersUI::setDefaultEnabled(bool value) {
    ui->val_DefaultEnabled->setChecked(value);
}

/**
 * Sets if the "auto apply" option is enabled to the ui.
 *
 * @brief ShadersUI::setAutoApply
 * @param value
 */
void ShadersUI::setAutoApply(bool value) {
    m_autoApply = value;
    ui->val_AutoApply->setChecked(value);
}

/**
 * Sets if the shader is compiled to the UI.
 *
 * @brief ShadersUI::setShaderCompiled
 * @param compiled
 */
void ShadersUI::setShaderCompiled(bool compiled) {
    ui->val_ShaderCompiled->setText(compiled ? "Yes" : "No");
}

/**
 * Sets the shader settings text to the advanced UI tab.
 *
 * @brief ShadersUI::setShadersText
 * @param text
 */
void ShadersUI::setShadersText(QByteArray text) {
    if (QString::compare(QVariant(ui->val_ShadersText->toPlainText()).toByteArray(), text) != 0) {
        ui->val_ShadersText->setPlainText(text);
        setUIShaderValues();
    }
}

/**
 * Sets the shader settings text to the advanced UI tab.
 *
 * @brief ShadersUI::setShadersText
 * @param text
 */
void ShadersUI::setShadersText(QString text) {
    if (QString::compare(ui->val_ShadersText->toPlainText(), text) != 0) {
        ui->val_ShadersText->setPlainText(text);
        setUIShaderValues();
    }
}

/**
 * Sets the checkmark to checked or unchecked based on the current status of the effect.
 *
 * @brief ShadersUI::setEffectEnabled
 * @param status
 */
void ShadersUI::setEffectEnabled(bool status) {
    ui->val_EnableEffect->setChecked(status);
}

/**
 * Reprocesses some UI variables if the shader settings were modified.
 *
 * @brief ShadersUI::setUIShaderValues
 */
void ShadersUI::setUIShaderValues() {
    updateShaderOrder();
    parseSettingsBuffer();
}

/**
 * If the user moves shaders up or down, update that here.
 *
 * @brief ShadersUI::updateShaderOrder
 */
void ShadersUI::updateShaderOrder() {
    if (!ui->val_ShaderOrder->count()) {
        return;
    }
    QString shadersText = ui->val_ShadersText->toPlainText();
    QString order = "const int SHADER_ORDER[SHADERS+1] = int[] ( // Don't change this line.\n\n";
    for (int i = 0; i < ui->val_ShaderOrder->count(); ++i) {
           order.append("    SHADER_").append(ui->val_ShaderOrder->item(i)->text()).append(",\n");
    }
    order.append("\nSHADERS); //");
    QRegularExpression orderRegex("^const\\s+int\\s+SHADER_ORDER.+?^SHADERS\\);\\s+//");
    orderRegex.setPatternOptions(QRegularExpression::DotMatchesEverythingOption | QRegularExpression::MultilineOption);
    shadersText = shadersText.replace(orderRegex, order);
    setShadersText(shadersText);
}

/**
 * Process the shader settings, set variables to the UI.
 *
 * @brief ShadersUI::parseSettingsBuffer
 */
void ShadersUI::parseSettingsBuffer() {
    QStringList lines = ui->val_ShadersText->toPlainText().split("\n");
    if (lines.empty()) {
        return;
    }
    int curTableRow = 0;
    ui->table_Shaders->clearContents();
    ui->table_Shaders->setRowCount(curTableRow);

    QRegularExpression enabledRegex("^#define\\s+([A-Z0-9_]+)_ENABLED\\s+(\\d)");
    QRegularExpression setting1Regex("^#define\\s+([A-Z0-9_]+)\\s+(\\S+)");
    QRegularExpression setting2Regex("^uniform\\s+.+?\\s+([A-Z0-9_]+)\\s+=\\s+(.+?);\\s*$");

    disconnect(ui->table_Shaders, &QTableWidget::itemChanged, this, &ShadersUI::slotEditShaderSetting);
    bool foundOrder = false, foundDefine = false, curShaderEnabled = false;
    QStringList shaderOrder;
    QString curShader, enabledShaders;
    for (int i = 0; i < lines.size(); ++i) {
        QString curLine = lines.at(i).trimmed();

        if (!foundOrder) {
            if (curLine.startsWith("SHADERS);")) {
                foundOrder = true;
                continue;
            }
            if (curLine.startsWith("SHADER_")) {
                curLine.remove(0, 7);
                if (curLine.endsWith(",")) {
                    curLine.chop(1);
                }
                shaderOrder.append(curLine);
                continue;
            }
        }

        if (!foundDefine) {
            QRegularExpressionMatch matches = enabledRegex.match(curLine);
            if (matches.hasMatch()) {
               curShader = matches.captured(1).prepend("SHADER_");
               curShaderEnabled = matches.captured(2).toInt() == 1;
               ui->table_Shaders->insertRow(curTableRow);
               QTableWidgetItem *curShaderItem = new QTableWidgetItem(curShader);
               curShaderItem->setFlags(curShaderItem->flags() & ~Qt::ItemIsEditable);
               ui->table_Shaders->setItem(curTableRow, 0, curShaderItem);
               QTableWidgetItem *settingItem = new QTableWidgetItem(curShaderEnabled ? "On" : "Off");
               settingItem->setFlags(settingItem->flags() & ~Qt::ItemIsEditable);
               ui->table_Shaders->setItem(curTableRow, 1, settingItem);
               curTableRow++;
               if (curShaderEnabled) {
                   QString shortShader;
                   shortShader.append(curShader).remove(0, 7);
                   enabledShaders.append(shortShader).append(", ");
               }
               foundDefine = true;
            }
            continue;
        }

        if (curLine.startsWith("#endif")) {
            foundDefine = false;
            continue;
        }

        if (!curShaderEnabled) {
            continue;
        }

        if (curLine.startsWith("#define")) {
            QRegularExpressionMatch matches = setting1Regex.match(curLine);
            if (matches.hasMatch()) {
                ui->table_Shaders->insertRow(curTableRow);
                QTableWidgetItem *nameItem = new QTableWidgetItem(matches.captured(1));
                nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
                QTableWidgetItem *settingItem = new QTableWidgetItem(matches.captured(2));
                ui->table_Shaders->setItem(curTableRow, 0, nameItem);
                ui->table_Shaders->setItem(curTableRow, 1, settingItem);
                curTableRow++;
            }
            continue;
        }

        if (curLine.startsWith("uniform")) {
            // Does not work: QRegularExpressionMatch matches = isUniform ? setting2Regex.match(curLine) : setting1Regex.match(curLine);
            QRegularExpressionMatch matches = setting2Regex.match(curLine);
            if (matches.hasMatch()) {
                ui->table_Shaders->insertRow(curTableRow);
                QTableWidgetItem *nameItem = new QTableWidgetItem(matches.captured(1));
                nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
                QTableWidgetItem *settingItem = new QTableWidgetItem(matches.captured(2));
                ui->table_Shaders->setItem(curTableRow, 0, nameItem);
                ui->table_Shaders->setItem(curTableRow, 1, settingItem);
                curTableRow++;
            }
        }
    }
    connect(ui->table_Shaders, &QTableWidget::itemChanged, this, &ShadersUI::slotEditShaderSetting);
    // Set enabled shaders list on the status tab.
    if (enabledShaders.endsWith(", ")) {
        enabledShaders.chop(2);
        ui->val_EnabledShaders->setText(enabledShaders);
    }
    // Set the data on the shader order tab.
    ui->val_ShaderOrder->clear();
    ui->val_ShaderOrder->addItems(shaderOrder);

    if (m_autoApply) {
        setSaveButtonText(true);
        emit signalShaderTestRequested();
    }
}

/**
 * Returns the blacklist from the UI.
 *
 * @brief ShadersUI::getBlacklist
 * @return
 */
QString ShadersUI::getBlacklist() {
    return ui->val_Blacklist->toPlainText();
}

/**
 * Returns the whitelist from the UI.
 *
 * @brief ShadersUI::getWhitelist
 * @return
 */
QString ShadersUI::getWhitelist() {
    return ui->val_Whitelist->toPlainText();
}

/**
 * Returns the shader path from the UI.
 *
 * @brief ShadersUI::getShaderPath
 * @return
 */
QString ShadersUI::getShaderPath() {
    return ui->val_ShaderPath->toPlainText();
}

/**
 * Returns the status of the "default enabled" setting from the UI.
 *
 * @brief ShadersUI::getDefaultEnabled
 * @return
 */
bool ShadersUI::getDefaultEnabled() {
    return ui->val_DefaultEnabled->isChecked();
}

/**
 * Returns the status of the "auto apply " setting from the UI.
 *
 * @brief ShadersUI::getAutoApply
 * @return
 */
bool ShadersUI::getAutoApply() {
    bool value = ui->val_AutoApply->isChecked();
    m_autoApply = value;
    return value;
}

/**
 * Returns the current shader settings from the UI.
 *
 * @brief ShadersUI::getShadersText
 * @return
 */
QByteArray ShadersUI::getShadersText() {
    return QVariant(ui->val_ShadersText->toPlainText()).toByteArray();
}
