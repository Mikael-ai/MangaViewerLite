#ifndef APPUTILS_H
#define APPUTILS_H

#include <QWidget>


constexpr uint32_t defaultSheetWidth = 800;
constexpr uint32_t defaultVScrollStep = 60;
constexpr uint32_t defaultHScrollStep = 60;
// QString doesn't have constexpr constructor
constexpr const char* defaultBackground = "#bb5232";

constexpr const char* key_sheetWidth= "sheetWidth";
constexpr const char* key_vScrollStep = "vScrollStep";
constexpr const char* key_hScrollStep = "hScrollStep";
constexpr const char* key_background = "background";


class AppUtils
{
private:
    static AppUtils *p_appUtils;
    QVariantMap m_config;

private:
    AppUtils();
    ~AppUtils();

public:
    static AppUtils* getInstance();
    QVariantMap getConfig(bool needToReload = false);
    void saveConfig(QVariantMap config);
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
