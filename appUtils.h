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

    inline QString getTempDirPath() const { return tempDirPath; };
    inline QString getSettingsDirPath() const { return settingsDirPath; };

    inline QString getBackground() const { return m_background; }
    inline void setBackground(const QString &background) { m_background = background; }

    QString getBackgroundStyleSheet(const QString &background) const;
    QString getBigAssScrollAreaStyleSheet() const;

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
