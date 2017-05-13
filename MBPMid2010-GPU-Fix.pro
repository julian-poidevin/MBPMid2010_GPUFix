#-------------------------------------------------
#
# Project created by QtCreator 2017-02-24T17:37:54
#
#-------------------------------------------------

QT       += core gui xml autoupdatergui
CONFIG += C++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = MBPMid2010-GPU-Fix
TEMPLATE = app

ICON = MBPMid2010-GPU-Fix.icns

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += source/main.cpp\
        source/mainwindow.cpp \
    source/logger.cpp \
    source/autoupdatercore/simplescheduler.cpp \
    source/autoupdatercore/updater.cpp \
    source/autoupdatercore/updater_p.cpp

HEADERS  += source/mainwindow.h \
    source/logger.h \
    source/autoupdatercore/adminauthoriser.h \
    source/autoupdatercore/qautoupdatercore_global.h \
    source/autoupdatercore/simplescheduler_p.h \
    source/autoupdatercore/updater.h \
    source/autoupdatercore/updater_p.h

FORMS    += mainwindow.ui

RESOURCES += \
    mbpmid2010_gpufix.qrc

unix:!mac {
        LIBS += -L$$[QT_INSTALL_LIBS] -licudata
        LIBS += -L$$[QT_INSTALL_LIBS] -licui18n
        LIBS += -L$$[QT_INSTALL_LIBS] -licuuc
}

#add lib dir to rpath
mac: QMAKE_LFLAGS += '-Wl,-rpath,\'$$OUT_PWD/../../../lib\''

