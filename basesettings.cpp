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
    setWindowTitle(tr("Settings"));

    setUiValues(appUtils->getConfig(true));

    connect(this, SIGNAL(settingsWereSaved(QVariantMap)),
            mainWindow, SLOT(loadConfigFromVariant(QVariantMap)));
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
    ui->backgroundButton->setStyleSheet(appUtils->getBackgroundStyleSheet(background));
}

void BaseSettings::on_saveButton_released()
{
    QVariantMap config;
    config[key_sheetWidth] = ui->widthLineEdit->text().toUInt();
    config[key_vScrollStep] = ui->vscrollLineEdit->text().toUInt();
    config[key_hScrollStep] = ui->hscrollLineEdit->text().toUInt();
    config[key_background] = appUtils->getBackground();

    appUtils->saveConfig(config);

    emit settingsWereSaved(config);
    this->close();
}


void BaseSettings::on_backgroundButton_released()
{
    const QString selectedColor = QColorDialog::getColor(QColor(appUtils->getBackground())).name();
    if (selectedColor == QStringLiteral("#000000"))
        return;

    appUtils->setBackground(selectedColor);
    ui->backgroundButton->setStyleSheet(appUtils->getBackgroundStyleSheet(selectedColor));
}

