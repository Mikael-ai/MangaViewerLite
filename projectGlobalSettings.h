#ifndef PROJECTGLOBALSETTINGS_H
#define PROJECTGLOBALSETTINGS_H

#include <QObject>

constexpr uint32_t defaultSheetWidth = 800;
constexpr uint32_t defaultScrollStep = 80;
// QString doens't have constexpr constructor
constexpr const char* defaultBackground = "#bb5232";

#endif // PROJECTGLOBALSETTINGS_H
