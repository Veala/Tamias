#-------------------------------------------------
#
# Project created by QtCreator 2018-07-09T19:45:57
#
#-------------------------------------------------

QT       += core gui uitools designer
CONFIG   += console

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets network printsupport

TARGET = tgca
TEMPLATE = app

win32:INCLUDEPATH += "C:/VXIPNP/WinNT/Include"
#win32:LIBS += "c:/Program Files (x86)/IVI Foundation/VISA/WinNT/Lib_x64/msc/visa64.lib"
win32:LIBS += "c:/VXIPNP/WinNT/lib/msc/visa32.lib"
#for 32x mingw and Qt
#win32:LIBS += "c:/Program Files (x86)/IVI Foundation/VISA/WinNT/lib/msc/visa32.lib"

RC_FILE = tgca.rc

SOURCES += main.cpp\
    basedevice.cpp \
    deviceselectionform.cpp \
    mainwindow.cpp \
    connection.cpp \
    device.cpp \
    funclib.cpp \
    registers.cpp \
    animatedspinnericon.cpp \
    socketdriver.cpp \
    baseterasicdevice.cpp \
    lsctable.cpp \
    lscgatewaythread.cpp \
    globalstate.cpp \
    inputdialog.cpp \
    tests/abstracttest.cpp \
    tests/memtest.cpp \
    tests/spipart.cpp \
    tests/spelsamperage.cpp \
    settings.cpp \
    tests/pausetest.cpp \
    tests/lscmessagetest.cpp \
    lscformat.cpp \
    writedevicememory.cpp

HEADERS  += mainwindow.h \
    basedevice.h \
    connection.h \
    device.h \
    deviceselectionform.h \
    funclib.h \
    data_structs.h \
    registers.h \
    picts.h \
    animatedspinnericon.h \
    socketdriver.h \
    baseterasicdevice.h \
    lsctable.h \
    lscgatewaythread.h \
    globalstate.h \
    comboboxnoscroll.h \
    inputdialog.h \
    tests/abstracttest.h \
    tests/memtest.h \
    tests/spipart.h \
    tests/spelsamperage.h \
    settings.h \
    tests/pausetest.h \
    tests/lscmessagetest.h \
    extern_hsc.h \
    lscformat.h \
    regNSK.h \
    writedevicememory.h

FORMS    += mainwindow.ui \
    connection.ui \
    basedevice.ui \
    deviceselectionform.ui \
    inputdialog.ui \
    ../default/settings_mem_test.ui \
    ../default/settings_spi_part.ui \
    ../default/stats_mem_test.ui \
    ../default/stats_spi_part.ui \
    ../default/settings_spelsamperage_test.ui \
    ../default/stats_spelsamperage_test.ui \
    settings.ui \
    ../default/settings_pause.ui \
    ../default/stats_pause.ui \
    ../default/settings_lsc_message_test.ui \
    ../default/stats_lsc_message_test.ui

RESOURCES += \
    tgca.qrc
