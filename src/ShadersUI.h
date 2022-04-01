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
    void setBlacklist(QString);
    void setWhitelist(QString);
    void setShaderPath(QString);
    void setDefaultEnabled(bool);
    void setNumWindowsStatus(int);

private:
    Ui::ShadersUI* ui;
};

#endif // SHADERSUI_H
