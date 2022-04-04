#include "ShadersUI.h"
#include "ui_ShadersUI.h"
#include <QPushButton>
#include <QRegularExpression>

ShadersUI::ShadersUI(QWidget *parent) : QDialog(parent), ui(new Ui::ShadersUI) {
    ui->setupUi(this);
    connect(ui->button_CloseWindow, &QDialogButtonBox::clicked, this, &ShadersUI::slotHideWindow);
    connect(ui->button_ShaderSave, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderSaveRequested);
    connect(ui->button_OrderSave, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderSaveRequested);
    connect(ui->button_ShaderApply, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderTestRequested);
    connect(ui->button_OrderApply, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderTestRequested);
    connect(ui->button_SettingsSave, &QDialogButtonBox::clicked, this, &ShadersUI::slotSettingsSaveRequested);
    connect(ui->val_allWindows, &QCheckBox::stateChanged, this, &ShadersUI::slotAllWindowsToggled);
    connect(ui->button_MoveShaderUp, &QPushButton::clicked, this, &ShadersUI::slotMoveShaderUp);
    connect(ui->button_MoveShaderDown, &QPushButton::clicked, this, &ShadersUI::slotMoveShaderDown);
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
    ui->button_ShaderSave->button(QDialogButtonBox::Save)->setText(saveText);
    ui->button_OrderSave->button(QDialogButtonBox::Save)->setText(saveText);
}

void ShadersUI::slotShaderSaveRequested() {
    setSaveButtonText(false);
    setUIShaderValues();
    emit signalShaderSaveRequested();
}

void ShadersUI::slotShaderTestRequested() {
    setSaveButtonText(true);
    setUIShaderValues();
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
    QRegularExpression enabledRegex("^#define\\s+([A-Z0-9_]+)_ENABLED\\s+(\\d)");
    QRegularExpression setting1Regex("^#define\\s+([A-Z0-9_]+)\\s+(\\S+)");
    QRegularExpression setting2Regex("^uniform\\s+(.+?)\\s+=\\s+(.+?)$");
    bool foundOrder = false, foundDefine = false;
    QMap<QString, QMap<QString, QString>> settings;
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
               QMap<QString, QString> enabled;
               enabled.insert("Enabled", matches.captured(2));
               curShader = matches.captured(1);
               settings.insert(curShader, enabled);
               if (matches.captured(2).toInt() == 1) {
                   enabledShaders.append(curShader).append(", ");
               }
               foundDefine = true;
            }
            continue;
        }
        if (curLine.startsWith("#endif")) {
            foundDefine = false;
            continue;
        }
        if (curLine.startsWith("#define")) {
            QRegularExpressionMatch matches = setting1Regex.match(curLine);
            if (matches.hasMatch()) {
               QMap<QString, QString> setting;
               setting.insert(matches.captured(1), matches.captured(2));
               settings.insert(curShader, setting);
            }
            continue;
        }
        if (curLine.startsWith("uniform")) {
            // Does not work: QRegularExpressionMatch matches = isUniform ? setting2Regex.match(curLine) : setting1Regex.match(curLine);
            QRegularExpressionMatch matches = setting2Regex.match(curLine);
            if (matches.hasMatch()) {
               QMap<QString, QString> setting;
               setting.insert(matches.captured(1), matches.captured(2));
               settings.insert(curShader, setting);
            }
        }
    }
    // Set enabled shaders list on the status tab.
    if (enabledShaders.endsWith(", ")) {
        enabledShaders.chop(2);
        ui->val_enabledShaders->setText(enabledShaders);
    }
    // Set the data on the shader order tab.
    ui->val_ShaderOrder->clear();
    ui->val_ShaderOrder->addItems(shaderOrder);
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

QByteArray ShadersUI::getShadersText() {
    return QVariant(ui->val_ShadersText->toPlainText()).toByteArray();
}
