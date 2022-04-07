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

#ifndef SHADERSUI_H
#define SHADERSUI_H

#include <QDialog>
#include <QTableWidgetItem>

namespace Ui {
class ShadersUI;
}

/**
 * @brief The ShadersUI class
 */
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
    void setError(QString);
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
