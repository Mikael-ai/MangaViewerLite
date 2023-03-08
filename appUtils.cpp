#include "appUtils.h"
#include <QJsonDocument>
#include <QDir>

AppUtils* AppUtils::p_appUtils = nullptr;

AppUtils::AppUtils()
{

}

AppUtils *AppUtils::getInstance()
{
    if (!p_appUtils)
        p_appUtils = new AppUtils();

    return p_appUtils;
}

QVariantMap AppUtils::getConfig(bool needToReload)
{
    if (!needToReload)
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

void AppUtils::saveConfig(QVariantMap config)
{
    QJsonDocument jsonConfig = QJsonDocument::fromVariant(config);

    const QString pathToJsonConfig = QDir::currentPath() % QStringLiteral("/settings/config.json");
    QFile jsonConfigFile(pathToJsonConfig);
    jsonConfigFile.open(QFile::WriteOnly);
    jsonConfigFile.write(jsonConfig.toJson());

    m_config = config;
}
