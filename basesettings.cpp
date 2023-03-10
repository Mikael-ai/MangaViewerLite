#include "basesettings.h"
#include "ui_basesettings.h"
#include "appUtils.h"

#include <QJsonDocument>
#include <QColorDialog>
#include <QDir>

BaseSettings::BaseSettings(QWidget *mainWindow,
                           QWidget *parent)
    : QWidget(parent),
      ui(new Ui::BaseSettings)
{
    ui->setupUi(this);
    setUiValues(appUtils->getConfig(true));

    setFixedSize(QSize(380, 250));
    setWindowTitle(tr("Settings"));

    connect(this, SIGNAL(settingsWereSaved(QVariantMap)),
            mainWindow, SLOT(loadConfigFromVariant(QVariantMap)));

    lastPickedBackground = appUtils->getConfigValue(key_background).toString();
}

BaseSettings::~BaseSettings()
{
    delete ui;
}

void BaseSettings::setUiValues(QVariantMap config)
{
    // Getting config values
    uint32_t sheetWidth = config.value(key_sheetWidth, defaultSheetWidth).toUInt();
    uint32_t vScrollStep = config.value(key_vScrollStep, defaultVScrollStep).toUInt();
    uint32_t hScrollStep = config.value(key_hScrollStep, defaultHScrollStep).toUInt();
    QString background = config.value(key_background, defaultBackground).toString();

    // Setting ui components values
    ui->widthLineEdit->setText(QString::number(sheetWidth));
    ui->vscrollLineEdit->setText(QString::number(vScrollStep));
    ui->hscrollLineEdit->setText(QString::number(hScrollStep));
    ui->backgroundButton->setStyleSheet(appUtils->constructStyleSheet("backgroundButton",
                                                                      background));
}

void BaseSettings::on_saveButton_released()
{
    QVariantMap config;
    config[key_sheetWidth] = ui->widthLineEdit->text().toUInt();
    config[key_vScrollStep] = ui->vscrollLineEdit->text().toUInt();
    config[key_hScrollStep] = ui->hscrollLineEdit->text().toUInt();
    config[key_background] = lastPickedBackground;

    appUtils->saveConfig(config);

    emit settingsWereSaved(config);
    this->close();
}


void BaseSettings::on_backgroundButton_released()
{
    QColor selectedColor(appUtils->getConfigValue(key_background).toString());
    QString selectedColorName = QColorDialog::getColor(selectedColor).name();
    if ((selectedColorName == QStringLiteral("#000000")
         && selectedColorName != lastPickedBackground)
        || selectedColorName.isEmpty())
    {
        lastPickedBackground = appUtils->getConfigValue(key_background).toString();
        return;
    }

    lastPickedBackground = selectedColorName;

    ui->backgroundButton->setStyleSheet(appUtils->constructStyleSheet("backgroundButton",
                                                                      selectedColorName));
}

