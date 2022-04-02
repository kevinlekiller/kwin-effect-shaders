#include "ShadersUI.h"
#include "ui_ShadersUI.h"
#include <QPushButton>


ShadersUI::ShadersUI(QWidget *parent) : QDialog(parent), ui(new Ui::ShadersUI) {
    ui->setupUi(this);
    connect(ui->button_CloseWindow, &QDialogButtonBox::clicked, this, &ShadersUI::slotHideWindow);
    connect(ui->button_ShaderSave, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderSaveRequested);
    connect(ui->button_ShaderTest, &QDialogButtonBox::clicked, this, &ShadersUI::slotShaderTestRequested);
    connect(ui->button_SettingsSave, &QDialogButtonBox::clicked, this, &ShadersUI::slotSettingsSaveRequested);
}

ShadersUI::~ShadersUI() {
    delete ui;
}

void ShadersUI::slotHideWindow() {
    hide();
    emit signalWindowHidden();
}

void ShadersUI::slotShaderSaveRequested() {
    ui->button_ShaderSave->button(QDialogButtonBox::Save)->setText("Save");
    emit signalShaderSaveRequested();
}

void ShadersUI::slotShaderTestRequested() {
    ui->button_ShaderSave->button(QDialogButtonBox::Save)->setText("(*) Save");
    emit signalShaderTestRequested();
}

void ShadersUI::slotSettingsSaveRequested() {
    emit signalSettingsSaveRequested();
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
    ui->val_ShadersText->setPlainText(text);
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


