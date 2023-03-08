#ifndef BASESETTINGS_H
#define BASESETTINGS_H

#include <QWidget>

namespace Ui {
class BaseSettings;
}

class BaseSettings : public QWidget
{
    Q_OBJECT

public:
    QString lastPickedBackground;
    QPalette backgroundButtonPalette;

private:
    QVariantMap m_config;

public:
    explicit BaseSettings(QWidget *parent = nullptr);
    ~BaseSettings();

    QVariantMap getConfig(bool needToLoad = false);
    void setUiValues();

signals:
    void settingsWereSaved(const QVariantMap &config);

private slots:
    void on_saveButton_released();

private:
    Ui::BaseSettings *ui;
};

#endif // BASESETTINGS_H
