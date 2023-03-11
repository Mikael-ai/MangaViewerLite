#ifndef APPUTILS_H
#define APPUTILS_H

#include <QWidget>
#include <QMap>

constexpr uint32_t defaultSheetWidth = 800;
constexpr uint32_t defaultVScrollStep = 60;
constexpr uint32_t defaultHScrollStep = 60;
// QString doesn't have constexpr constructor
constexpr const char* defaultBackground = "#3f424c";

constexpr const char* key_sheetWidth= "sheetWidth";
constexpr const char* key_vScrollStep = "vScrollStep";
constexpr const char* key_hScrollStep = "hScrollStep";
constexpr const char* key_background = "background";


class AppUtils
{
private:
    static AppUtils *p_appUtils;
    QVariantMap m_config;
    QString m_background;

    QString tempDirPath;
    QString settingsDirPath;
private:
    AppUtils();
    ~AppUtils();

public:
    static AppUtils* getInstance();

    QVariantMap getConfig(bool needToReload = false);
    void saveConfig(const QVariantMap &config);

    QVariant getConfigValue(const QString &key) const;

    inline QString getTempDirPath() const { return tempDirPath; };
    inline QString getSettingsDirPath() const { return settingsDirPath; };

    QString getStyleSheet(const QString &widget,
                          const bool isBase = true) const;
    QString constructStyleSheet(const QString &widget,
                                const QString &color = QString()) const;
    QString constructStyleSheet(const QString &widget,
                                const QStringList &colors = QStringList()) const;
    // Really big (at least I think so)
    QString getBigAssScrollAreaStyleSheet(const QString &color) const;

    void checkAppDirs();

    void unzipFile(const QString &filePath) const;
    void cleanTempDir() const;
};


struct ScopedPointerCustomDeleter
{
    static inline void cleanup(QWidget *pointer)
    {
        if (!pointer->isVisible())
        {
            delete pointer;
            pointer = nullptr;
        }
    }
};

inline AppUtils *appUtils = AppUtils::getInstance();

#endif // APPUTILS_H
