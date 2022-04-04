#include "ShadersUI.h"
#include "ui_ShadersUI.h"
#include <QPushButton>
#include <QRegularExpression>

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
    connect(ui->val_allWindows, &QCheckBox::stateChanged, this, &ShadersUI::slotAllWindowsToggled);
    connect(ui->button_MoveShaderUp, &QPushButton::clicked, this, &ShadersUI::slotMoveShaderUp);
    connect(ui->button_MoveShaderDown, &QPushButton::clicked, this, &ShadersUI::slotMoveShaderDown);
    connect(ui->table_Shaders, &QTableWidget::cellClicked, this, &ShadersUI::slotToggleShader);
    connect(ui->table_Shaders, &QTableWidget::itemChanged, this, &ShadersUI::slotEditShaderSetting);
}

ShadersUI::~ShadersUI() {
    delete ui;
}

void ShadersUI::slotHideWindow() {
    //m_UIPosition = pos();
    hide();
    emit signalWindowHidden();
}

void ShadersUI::setSaveButtonText(bool modified) {
    QString saveText = modified ? "(*) Save" : "Save";
    ui->button_AdvancedSave->button(QDialogButtonBox::Save)->setText(saveText);
    ui->button_ShadersSave->button(QDialogButtonBox::Save)->setText(saveText);
    ui->button_OrderSave->button(QDialogButtonBox::Save)->setText(saveText);
}

void ShadersUI::slotShaderSaveRequested() {
    setUIShaderValues();
    setSaveButtonText(false);
    emit signalShaderSaveRequested();
}

void ShadersUI::slotShaderTestRequested() {
    setUIShaderValues();
    setSaveButtonText(true);
    emit signalShaderTestRequested();
}

void ShadersUI::slotMoveShaderUp() {
    int row = ui->val_ShaderOrder->currentRow();
    QListWidgetItem *item = ui->val_ShaderOrder->takeItem(row);
    ui->val_ShaderOrder->insertItem(--row, item);
    ui->val_ShaderOrder->setCurrentRow(row);
}

void ShadersUI::slotMoveShaderDown() {
    int row = ui->val_ShaderOrder->currentRow();
    QListWidgetItem *item = ui->val_ShaderOrder->takeItem(row);
    ui->val_ShaderOrder->insertItem(++row, item);
    ui->val_ShaderOrder->setCurrentRow(row);
}

void ShadersUI::slotSettingsSaveRequested() {
    emit signalSettingsSaveRequested();
}

void ShadersUI::slotAllWindowsToggled(int state) {
    emit signalAllWindowsToggled(state == Qt::Checked ? true : false);
}

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

void ShadersUI::setBlacklist(QString value) {
    ui->val_Blacklist->setPlainText(value);
}

void ShadersUI::setWhitelist(QString value) {
    ui->val_Whitelist->setPlainText(value);
}

void ShadersUI::setShaderPath(QString value) {
    ui->val_ShaderPath->setPlainText(value);
}

void ShadersUI::setDefaultEnabled(bool value) {
    ui->val_DefaultEnabled->setChecked(value);
}

void ShadersUI::setAutoApply(bool value) {
    m_autoApply = value;
    ui->val_AutoApply->setChecked(value);
}

void ShadersUI::setNumWindowsStatus(int numWindows) {
    ui->val_Status->setText(QVariant(numWindows).toString());
}

void ShadersUI::setShaderCompiled(bool compiled) {
    ui->val_ShaderCompiled->setText(compiled ? "Yes" : "No");
}

void ShadersUI::setShadersText(QByteArray text) {
    if (QString::compare(QVariant(ui->val_ShadersText->toPlainText()).toByteArray(), text) != 0) {
        ui->val_ShadersText->setPlainText(text);
        setUIShaderValues();
    }
}

void ShadersUI::setShadersText(QString text) {
    if (QString::compare(ui->val_ShadersText->toPlainText(), text) != 0) {
        ui->val_ShadersText->setPlainText(text);
        setUIShaderValues();
    }
}

void ShadersUI::setAllWindows(bool allWindows) {
    ui->val_allWindows->setChecked(allWindows);
}

void ShadersUI::setUIShaderValues() {
    updateShaderOrder();
    parseSettingsBuffer();
}

/**
 * If the user moves shaders up or down, update that here.
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
        ui->val_enabledShaders->setText(enabledShaders);
    }
    // Set the data on the shader order tab.
    ui->val_ShaderOrder->clear();
    ui->val_ShaderOrder->addItems(shaderOrder);

    if (m_autoApply) {
        setSaveButtonText(true);
        emit signalShaderTestRequested();
    }
}

QString ShadersUI::getBlacklist() {
    return ui->val_Blacklist->toPlainText();
}

QString ShadersUI::getWhitelist() {
    return ui->val_Whitelist->toPlainText();
}

QString ShadersUI::getShaderPath() {
    return ui->val_ShaderPath->toPlainText();
}

bool ShadersUI::getDefaultEnabled() {
    return ui->val_DefaultEnabled->isChecked();
}

bool ShadersUI::getAutoApply() {
    bool value = ui->val_AutoApply->isChecked();
    m_autoApply = value;
    return value;
}

QByteArray ShadersUI::getShadersText() {
    return QVariant(ui->val_ShadersText->toPlainText()).toByteArray();
}
