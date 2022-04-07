#ifndef SHADERSUI_H
#define SHADERSUI_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class ShadersUI;
}

class ShadersUI : public QDialog {
    Q_OBJECT

public:
    explicit ShadersUI(QWidget *parent = nullptr);
    ~ShadersUI();
    QString getBlacklist();
    QString getWhitelist();
    QString getShaderPath();
    bool getDefaultEnabled();
    bool getAutoApply();
    QByteArray getShadersText();
    void setBlacklist(QString);
    void setWhitelist(QString);
    void setShaderPath(QString);
    void setDefaultEnabled(bool);
    void setAutoApply(bool);
    void setShaderCompiled(bool);
    void setShadersText(QByteArray);
    void setShadersText(QString);
    void setEffectEnabled(bool);
    void displayUI();

    bool m_autoApply = false;

Q_SIGNALS:
    void signalShaderTestRequested();
    void signalShaderSaveRequested();
    void signalSettingsSaveRequested();
    void signalEffectToggled(bool);
    void signalWindowHidden();

private:
    void parseSettingsBuffer();
    void setUIShaderValues();
    void setUIEnabledShaders();
    void updateShaderOrder();
    void setSaveButtonText(bool);

    Ui::ShadersUI* ui;
    //QPoint m_UIPosition;

private Q_SLOTS:
    void slotHideWindow();
    void slotShaderTestRequested();
    void slotShaderSaveRequested();
    void slotMoveShaderUp();
    void slotMoveShaderDown();
    void slotSettingsSaveRequested();
    void slotToggleEffect(int);
    void slotToggleShader(int, int);
    void slotEditShaderSetting(QTableWidgetItem  *);
};

#endif // SHADERSUI_H
