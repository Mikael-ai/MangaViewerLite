#include "appUtils.h"

#include <QJsonDocument>
//#include <private/qzipwriter_p.h>
#include <private/qzipreader_p.h>
#include <QDir>

AppUtils* AppUtils::p_appUtils = nullptr;

AppUtils::AppUtils()
{
    tempDirPath = QDir::currentPath() % QString("/temp");
    settingsDirPath = QDir::currentPath() % QString("/settings");
    checkAppDirs();
}

AppUtils::~AppUtils()
{
    cleanTempDir();
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

    const QString pathToJsonConfig = settingsDirPath % QString("/config.json");
    QFile jsonConfigFile(pathToJsonConfig);
    jsonConfigFile.open(QFile::WriteOnly);
    jsonConfigFile.write(jsonConfig.toJson());

    m_config = config;
}

void AppUtils::checkAppDirs() const
{
    QDir temp(tempDirPath);
    QDir settings(settingsDirPath);
    if (!temp.exists())
        temp.mkdir(tempDirPath);
    if (!settings.exists())
        settings.mkdir(settingsDirPath);
}

void AppUtils::unzipFile(const QString &filePath) const
{
    QZipReader zipReader(filePath);
    if (!zipReader.exists())
        return;

    zipReader.extractAll(tempDirPath);
}

void AppUtils::cleanTempDir() const
{
    QDir temp(tempDirPath);
    temp.setFilter( QDir::NoDotAndDotDot | QDir::Files );
    foreach (const auto &filename, temp.entryList())
    {
        temp.remove(filename);
    }
}
