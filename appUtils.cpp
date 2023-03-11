#include "appUtils.h"

#include <QJsonDocument>
#include <private/qzipreader_p.h>
#include <QDir>

AppUtils* AppUtils::p_appUtils = nullptr;

AppUtils::AppUtils()
{
    getConfig(true);

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

void AppUtils::saveConfig(const QVariantMap &config)
{
    QJsonDocument jsonConfig = QJsonDocument::fromVariant(config);

    const QString pathToJsonConfig = settingsDirPath % QString("/config.json");
    QFile jsonConfigFile(pathToJsonConfig);
    jsonConfigFile.open(QFile::WriteOnly);
    jsonConfigFile.write(jsonConfig.toJson());

    m_config = config;
}

QVariant AppUtils::getConfigValue(const QString &key) const
{
    return m_config.value(key);
}

QString AppUtils::getStyleSheet(const QString &widget,
                                const bool isBase) const
{
    const QString suffix = (isBase)
                           ? "StyleSheetBase"
                           : "StyleSheet";

    const QString filePath = QString(":/style/resources/%1%2.txt").arg(widget,
                                                                       suffix);
    QString styleSheet;
    QFile styleSheetFile(filePath);
    if (styleSheetFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream in(&styleSheetFile);
        while (!in.atEnd())
            styleSheet += in.readLine();
    }
    return styleSheet;
}

QString AppUtils::constructStyleSheet(const QString &widget,
                                      const QString &color) const
{
    if (color.isEmpty())
        return getStyleSheet(widget);

    return getStyleSheet(widget, false).arg(color);
}

QString AppUtils::constructStyleSheet(const QString &widget,
                                      const QStringList &colors) const
{
    if (colors.isEmpty())
        return getStyleSheet(widget);

    QString styleSheet = getStyleSheet(widget, false);
    for (auto nextColor = colors.cbegin(); nextColor < colors.cend(); ++nextColor)
    {
        styleSheet = styleSheet.arg(*nextColor);
    }

    return styleSheet;
}

QString AppUtils::getBigAssScrollAreaStyleSheet(const QString &color) const
{
    QStringList colors;
    int hue, saturation, value;
    QColor baseColor(color);
    baseColor.getHsv(&hue, &saturation, &value);

    // Here is where the magic numbers part starts :)
    QColor colorA, colorB, colorC, colorD;
    // Minimum value for each ScrollBar part = 30 out of 255, otherwise it's too dark
    value = (value < 30) ? 30 : value;

    // P.S. If the color is dark - it picks something brighter and vice versa

    // Scroll line color
    colorA.setHsl(hue,
                  saturation,
                  (value * 1.25 < 255) ? value * 1.25 : value * 0.75);
    // Slider color
    colorB.setHsl(hue,
                  saturation,
                  (value * 1.60 < 255) ? value * 1.60 : value * 0.40);
    // Hover color for slider and buttons
    colorC.setHsl((hue * 1.05 < 400) ? hue * 1.05 : hue * 0.95,
                  (saturation * 1.35 < 255) ? saturation * 1.35 : saturation * 0.65,
                  (value * 1.70 < 255) ? value * 1.70 : value * 0.20);
    // Buttons color
    colorD.setHsl(hue,
                  (saturation * 1.15 < 255) ? saturation * 1.15 : saturation * 0.85,
                  (value * 1.60 < 255) ? value * 1.60 : value * 0.40);

    colors << colorA.name()
           << colorB.name()
           << colorC.name()
           << colorD.name();

    return constructStyleSheet("scrollArea", colors);
}

void AppUtils::checkAppDirs()
{
    QDir temp(tempDirPath);
    QDir settings(settingsDirPath);
    if (!temp.exists())
        temp.mkdir(tempDirPath);
    if (!settings.exists())
    {
        QVariantMap config;
        config[key_sheetWidth] = defaultSheetWidth;
        config[key_hScrollStep] = defaultHScrollStep;
        config[key_vScrollStep] = defaultVScrollStep;
        config[key_background] = defaultBackground;

        settings.mkdir(settingsDirPath);
        saveConfig(config);
    }
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
