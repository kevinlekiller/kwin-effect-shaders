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
 * @brief Construct.
 *
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
 * @brief Destruct.
 */
ShadersUI::~ShadersUI() {
    delete ui;
}

/**
 * @brief Set the text on the save buttons.
 *
 * @param modified -> If the shader settings were modified or not.
 */
void ShadersUI::setSaveButtonText(bool modified) {
    QString saveText = modified ? "(*) Save" : "Save";
    ui->button_AdvancedSave->button(QDialogButtonBox::Save)->setText(saveText);
    ui->button_ShadersSave->button(QDialogButtonBox::Save)->setText(saveText);
    ui->button_OrderSave->button(QDialogButtonBox::Save)->setText(saveText);
}

/**
 * @brief User requested saving the shader settings.
 */
void ShadersUI::slotShaderSaveRequested() {
    setUIShaderValues();
    setSaveButtonText(false);
    emit signalShaderSaveRequested();
}

/**
 * @brief User requested testing shader settings.
 */
void ShadersUI::slotShaderTestRequested() {
    setUIShaderValues();
    setSaveButtonText(true);
    emit signalShaderTestRequested();
}

/**
 * @brief User requested moving shader up in order list.
 */
void ShadersUI::slotMoveShaderUp() {
    int row = ui->val_ShaderOrder->currentRow();
    QListWidgetItem *item = ui->val_ShaderOrder->takeItem(row);
    ui->val_ShaderOrder->insertItem(--row, item);
    ui->val_ShaderOrder->setCurrentRow(row);
}

/**
 * @brief User requested moving shader down in order list.
 */
void ShadersUI::slotMoveShaderDown() {
    int row = ui->val_ShaderOrder->currentRow();
    QListWidgetItem *item = ui->val_ShaderOrder->takeItem(row);
    ui->val_ShaderOrder->insertItem(++row, item);
    ui->val_ShaderOrder->setCurrentRow(row);
}

/**
 * @brief User requested saving main settings.
 */
void ShadersUI::slotSettingsSaveRequested() {
    emit signalSettingsSaveRequested();
}

/**
 * @brief User requested to enable the effect.
 *
 * @param state -> The state of the checkbox.
 */
void ShadersUI::slotToggleEffect(int state) {
    bool checked = state == Qt::Checked ? true : false;
    setEffectEnabled(checked);
    emit signalEffectToggled(checked);
}

/**
 * @brief User requested to enable or disable a shader.
 *
 * @param row    -> The row in the shader table.
 * @param column -> The column in the shader table.
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

    QString shadersText = ui->val_ShadersText->toPlainText();
    QString regex = "^#define\\s+";
    settingName.remove(0, 7);
    regex.append(settingName).append("_ENABLED\\s+\\d+");
    QRegularExpression replaceRegex(regex, QRegularExpression::MultilineOption);
    settingName.prepend("#define ").append("_ENABLED ").append(on == 0 ? "0" : "1");
    shadersText.replace(replaceRegex, settingName);
    setShadersText(shadersText);
}

/**
 * @brief User wants to edit a shader setting.
 *
 * @param item -> The item in the shader table.
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
    QRegularExpression replaceRegex(regex, QRegularExpression::MultilineOption);
    QString shadersText = ui->val_ShadersText->toPlainText();
    shadersText.replace(replaceRegex, replacement);
    setShadersText(shadersText);
}

/**
 * @brief Displays the UI if it's not hidden.
 */
void ShadersUI::displayUI() {
    if (!isHidden()) {
        open();
        return;
    }
    show();
}

/**
 * @brief Hides the UI.
 */
void ShadersUI::hideUI() {
    hide();
}

/**
 * @brief Hides the UI when the user clicks the OK button.
 */
void ShadersUI::slotHideWindow() {
    hideUI();
}

/**
 * @brief Sets the blacklist to the UI.
 *
 * @param blacklist -> The list.
 */
void ShadersUI::setBlacklist(QString blacklist) {
    ui->val_Blacklist->setPlainText(blacklist);
}

/**
 * @brief Sets the whitelist to the UI.
 *
 * @param whitelist -> The list
 */
void ShadersUI::setWhitelist(QString whitelist) {
    ui->val_Whitelist->setPlainText(whitelist);
}

/**
 * @brief Sets the shader path on the UI.
 *
 * @param shaderpath -> The path.
 */
void ShadersUI::setShaderPath(QString shaderpath) {
    if (shaderpath.endsWith("/")) {
        shaderpath.chop(1);
    }
    ui->val_ShaderPath->setPlainText(shaderpath);
}

/**
 * @brief Sets an error message on the UI.
 *
 * @param error -> The error message.
 */
void ShadersUI::setError(QString error) {
    ui->val_Error->setText(error);
}

/**
 * @brief Sets if the "defaut enabled" option is enabled to the UI.
 *
 * @param value -> If it's enabled or not.
 */
void ShadersUI::setDefaultEnabled(bool value) {
    ui->val_DefaultEnabled->setChecked(value);
}

/**
 * @brief Sets if the "auto apply" option is enabled to the ui.
 *
 * @param value -> If it's enable or not.
 */
void ShadersUI::setAutoApply(bool value) {
    m_autoApply = value;
    ui->val_AutoApply->setChecked(value);
}

/**
 * @brief Sets if the shader is compiled to the UI.
 *
 * @param compiled -> Is the shader compiled?
 */
void ShadersUI::setShaderCompiled(bool compiled) {
    ui->val_ShaderCompiled->setText(compiled ? "Yes" : "No");
}

/**
 * @brief Sets the shader settings text to the advanced UI tab.
 *
 * @param text -> The text with the shader settings.
 */
void ShadersUI::setShadersText(QByteArray text) {
    if (QString::compare(QVariant(ui->val_ShadersText->toPlainText()).toByteArray(), text) != 0) {
        ui->val_ShadersText->setPlainText(text);
        setUIShaderValues();
    }
}

/**
 * @brief Sets the shader settings text to the advanced UI tab.
 *
 * @param text -> The text with the shader settings.
 */
void ShadersUI::setShadersText(QString text) {
    if (QString::compare(ui->val_ShadersText->toPlainText(), text) != 0) {
        ui->val_ShadersText->setPlainText(text);
        setUIShaderValues();
    }
}

/**
 * @brief Sets the checkmark to checked or unchecked based on the current status of the effect.
 *
 * @param status -> The current status.
 */
void ShadersUI::setEffectEnabled(bool status) {
    ui->val_EnableEffect->setChecked(status);
}

/**
 * @brief Reprocesses some UI variables if the shader settings were modified.
 */
void ShadersUI::setUIShaderValues() {
    updateShaderOrder();
    parseSettingsBuffer();
}

/**
 * @brief If the user moves shaders up or down, update that here.
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
    QRegularExpression orderRegex("^const\\s+int\\s+SHADER_ORDER.+?^SHADERS\\);\\s+//", QRegularExpression::DotMatchesEverythingOption | QRegularExpression::MultilineOption);
    shadersText = shadersText.replace(orderRegex, order);
    setShadersText(shadersText);
}

/**
 * @brief Process the shader settings, set variables to the UI.
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
    QString curShader, enabledShaders, curTooltip;
    for (int i = 0; i < lines.size(); ++i) {
        QString curLine = lines.at(i).trimmed();

        // Set the shader order on the UI.
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

        // Find the start of shader's settings.
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

        // Reached the end of this shader's settings.
        if (curLine.startsWith("#endif")) {
            foundDefine = false;
            continue;
        }

        // Skip adding the setting / tooltip to the UI if the shader is disabled.
        if (!curShaderEnabled) {
            continue;
        }

        // Get the tooltip for the current setting.
        if (curLine.startsWith("//")) {
            curLine = curLine.remove(0, 2).trimmed();
            curTooltip.append("<p>").append(curLine).append("</p>");
        }

        // Put the setting onto the UI with its tooltip.
        bool isDefine = curLine.startsWith("#define");
        if (isDefine || curLine.startsWith("uniform")) {
            QRegularExpressionMatch matches;
            matches.operator=(isDefine? setting1Regex.match(curLine) : setting2Regex.match(curLine));
            if (matches.hasMatch()) {
                ui->table_Shaders->insertRow(curTableRow);
                QTableWidgetItem *nameItem = new QTableWidgetItem(matches.captured(1));
                nameItem->setFlags(nameItem->flags() & ~Qt::ItemIsEditable);
                QTableWidgetItem *settingItem = new QTableWidgetItem(matches.captured(2));
                curTooltip = curTooltip.trimmed();
                if (!curTooltip.isEmpty()) {
                    curTooltip.prepend("<html><head/><body>").append("</body></html>");
                    nameItem->setToolTip(curTooltip);
                    settingItem->setToolTip(curTooltip);
                }
                ui->table_Shaders->setItem(curTableRow, 0, nameItem);
                ui->table_Shaders->setItem(curTableRow, 1, settingItem);
                curTableRow++;
            }
            curTooltip.clear();
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
 * @brief Returns the blacklist from the UI.
 *
 * @return The blacklist.
 */
QString ShadersUI::getBlacklist() {
    return ui->val_Blacklist->toPlainText();
}

/**
 * @brief Returns the whitelist from the UI.
 *
 * @return The whitelist.
 */
QString ShadersUI::getWhitelist() {
    return ui->val_Whitelist->toPlainText();
}

/**
 * @brief Returns the shader path from the UI.
 *
 * @return The path.
 */
QString ShadersUI::getShaderPath() {
    return ui->val_ShaderPath->toPlainText();
}

/**
 * @brief Returns the status of the "default enabled" setting from the UI.
 *
 * @return If the checkbox is checked or not.
 */
bool ShadersUI::getDefaultEnabled() {
    return ui->val_DefaultEnabled->isChecked();
}

/**
 * @brief Returns the status of the "auto apply " setting from the UI.
 *
 * @return If the checkbox is checked or not.
 */
bool ShadersUI::getAutoApply() {
    bool value = ui->val_AutoApply->isChecked();
    m_autoApply = value;
    return value;
}

/**
 * @brief Returns the current shader settings from the UI.
 *
 * @return The shader settings text.
 */
QByteArray ShadersUI::getShadersText() {
    return QVariant(ui->val_ShadersText->toPlainText()).toByteArray();
}
