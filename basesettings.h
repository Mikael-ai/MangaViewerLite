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
    explicit BaseSettings(QWidget *mainWindow,
                          QWidget *parent = nullptr);
    ~BaseSettings();

    void setUiValues(QVariantMap config);

signals:
    void settingsWereSaved(const QVariantMap &config);

private slots:
    void on_saveButton_released();

private:
    Ui::BaseSettings *ui;
};

#endif // BASESETTINGS_H
