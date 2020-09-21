QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/about/aboutwidget.cpp \
    src/common/view.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/common/util.cpp \
    src/watermark/normalwatermarkwidget.cpp

HEADERS += \
    src/about/aboutwidget.h \
    src/common/view.h \
    src/mainwindow.h \
    src/common/util.h \
    src/watermark/normalwatermarkwidget.h

INCLUDEPATH += src \
    src/common \
    src/watermark \
    src/about

FORMS += \
    src/about/aboutwidget.ui \
    src/mainwindow.ui \
    src/watermark/normalwatermarkwidget.ui

TRANSLATIONS += \
    res/QHelper_zh_CN.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res/res.qrc
