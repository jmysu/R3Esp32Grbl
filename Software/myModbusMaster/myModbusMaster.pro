QT += serialbus serialport widgets
requires(qtConfig(combobox))

#qtConfig(modbus-serialport): QT += serialport

TARGET = myModbusMaster
TEMPLATE = app
CONFIG += c++11

SOURCES += main.cpp\
        mainwindow.cpp \
        settingsdialog.cpp \
    mainwindowtable.cpp

HEADERS  += mainwindow.h \
         settingsdialog.h

FORMS    += mainwindow.ui \
         settingsdialog.ui

RESOURCES += \
    master.qrc

target.path = $$[QT_INSTALL_EXAMPLES]/serialbus/modbus/master
INSTALLS += target
