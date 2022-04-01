#include "ShadersUI.h"
#include "ui_ShadersUI.h"

ShadersUI::ShadersUI(QWidget *parent) : QDialog(parent), ui(new Ui::ShadersUI) {
    ui->setupUi(this);
}

void ShadersUI::setBlacklist(QString value) {
    ui->val_Blacklist->setPlainText(value);
}

void ShadersUI::setWhitelist(QString value) {
    ui->val_Whitelist->setPlainText(value);
}

void ShadersUI::setShaderPath(QString value) {
    ui->val_ShaderPath->setText(value);
}

void ShadersUI::setDefaultEnabled(bool value) {
    ui->val_DefaultEnabled->setChecked(value);
}

void ShadersUI::setNumWindowsStatus(int numWindows) {
    ui->val_windowsShaders->setText(QVariant(numWindows).toString());
}

QString ShadersUI::getBlacklist() {
    return ui->val_Blacklist->toPlainText();
}

QString ShadersUI::getWhitelist() {
    return ui->val_Whitelist->toPlainText();
}

QString ShadersUI::getShaderPath() {
    return ui->val_ShaderPath->text();
}

bool ShadersUI::getDefaultEnabled() {
    return ui->val_DefaultEnabled->isChecked();
}

ShadersUI::~ShadersUI() {
    delete ui;
}
