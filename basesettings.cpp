#include "basesettings.h"
#include "ui_basesettings.h"
#include "projectGlobalSettings.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>
#include <QDir>
#include <QPalette>

BaseSettings::BaseSettings(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BaseSettings)
{
    ui->setupUi(this);
    setWindowTitle(tr("Settings"));

    // setting the background button palette
    backgroundButtonPalette = ui->backgroundButton->palette();
    ui->backgroundButton->setPalette(backgroundButtonPalette);

    getConfig(true);
    setUiValues();
}

BaseSettings::~BaseSettings()
{
    delete ui;
}

QVariantMap BaseSettings::getConfig(bool needToLoad)
{
    if (!needToLoad)
        return m_config;

    const QString pathToJsonConfig = QDir::currentPath() % QStringLiteral("/settings/config.json");
    QFile jsonConfigFile(pathToJsonConfig);
    jsonConfigFile.open(QFile::ReadOnly);

    // Could be done in a single line but ...
    // "(QJsonDocument::fromJson(jsonConfigFile.readAll())).toVariant().toMap()" is unreadable
    QJsonDocument jsonConfig = QJsonDocument::fromJson(jsonConfigFile.readAll());
    m_config = jsonConfig.toVariant().toMap();
    return m_config;
}

void BaseSettings::setUiValues()
{
    // Getting config values
    uint32_t sheetWidth = m_config.value("sheetWidth", defaultSheetWidth).toUInt();
    uint32_t scrollStep = m_config.value("scrollStep", defaultScrollStep).toUInt();
    QString background = m_config.value("background", defaultBackground).toString();

    // Setting ui components values
    ui->widthLineEdit->setText(QString::number(sheetWidth));
    ui->vscrollLineEdit->setText(QString::number(scrollStep));

    backgroundButtonPalette.setColor(QPalette::Button, QColor(background));
    ui->backgroundButton->update();
}

void BaseSettings::on_saveButton_released()
{
    QVariantMap config;
    config["sheetWidth"] = ui->widthLineEdit->text().toUInt();
    config["scrollStep"] = ui->vscrollLineEdit->text().toUInt();
    config["background"] = lastPickedBackground;

    QJsonDocument jsonConfig = QJsonDocument::fromVariant(config);

    const QString pathToJsonConfig = QDir::currentPath() % QStringLiteral("/settings/config.json");
    QFile jsonConfigFile(pathToJsonConfig);
    jsonConfigFile.open(QFile::WriteOnly);
    jsonConfigFile.write(jsonConfig.toJson());

    this->m_config = config;
    this->hide();

    emit settingsWereSaved(config);
}

