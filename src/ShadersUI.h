#ifndef SHADERSUI_H
#define SHADERSUI_H

#include <QDialog>

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
    QByteArray getShadersText();
    void setBlacklist(QString);
    void setWhitelist(QString);
    void setShaderPath(QString);
    void setDefaultEnabled(bool);
    void setNumWindowsStatus(int);
    void setShaderCompiled(bool);
    void setShadersText(QByteArray);
    void setShadersText(QString);
    void setAllWindows(bool);
    void displayUI();

Q_SIGNALS:
    void signalShaderTestRequested();
    void signalShaderSaveRequested();
    void signalSettingsSaveRequested();
    void signalAllWindowsToggled(bool);
    void signalWindowHidden();

private:
    Ui::ShadersUI* ui;
    //QPoint m_UIPosition;
    void parseSettingsBuffer();
    void setSaveButtonText(bool);

private Q_SLOTS:
    void slotHideWindow();
    void slotShaderTestRequested();
    void slotShaderSaveRequested();
    void slotMoveShaderUp();
    void slotMoveShaderDown();
    void slotSettingsSaveRequested();
    void slotAllWindowsToggled(int);
};

#endif // SHADERSUI_H
