QT       += core gui
QT       += gui-private

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

VERSION = 1.0
QMAKE_TARGET_COMPANY = Mikael Airapetyan
QMAKE_TARGET_PRODUCT = Manga Viewer Lite
QMAKE_TARGET_DESCRIPTION = Simple manga viewer
QMAKE_TARGET_COPYRIGHT = Mikael Airapetyan

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES *= QT_USE_QSTRINGBUILDER

SOURCES += \
    appUtils.cpp \
    basescrollbar.cpp \
    basesettings.cpp \
    main.cpp \
    mangaviewer.cpp

HEADERS += \
    appUtils.h \
    basescrollbar.h \
    basesettings.h \
    mangaviewer.h

FORMS += \
    basesettings.ui \
    mangaviewer.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    styles.qrc

RC_FILE = mangaViewer.rc

