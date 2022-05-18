QT       += core gui
QT += core5compat
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11
CONFIG +=console
# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    delaunay.cpp \
    graphicspoint.cpp \
    main.cpp \
    mainwindow.cpp \
    paintboard.cpp \
    switchformat.cpp \
    triangle.cpp

HEADERS += \
    delaunay.h \
    graphicspoint.h \
    mainwindow.h \
    paintboard.h \
    stdxfm.h \
    switchformat.h \
    triangle.h

FORMS += \
    mainwindow.ui

TRANSLATIONS += \
    learn36_zh_CN.ts
CONFIG += lrelease
CONFIG += embed_translations

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc

#DISTFILES += \
#    library/itklibrary.pri \
#    library/libtiff_library.pri \
#    library/opencvlibrary.pri \
#    library/windowskitlibrary.pri
#include (./library/itklibrary.pri)
#include (./library/windowskitlibrary.pri)
#include (./library/libtiff_library.pri)
#include (./library/opencvlibrary.pri)
#include (./library/tinytiff_library.pri)
