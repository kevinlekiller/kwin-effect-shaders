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

Q_SIGNALS:
    void signalShaderTestRequested();
    void signalShaderSaveRequested();
    void signalSettingsSaveRequested();
    void signalWindowHidden();

private:
    Ui::ShadersUI* ui;

private Q_SLOTS:
    void slotHideWindow();
    void slotShaderTestRequested();
    void slotShaderSaveRequested();
    void slotSettingsSaveRequested();
};

#endif // SHADERSUI_H
