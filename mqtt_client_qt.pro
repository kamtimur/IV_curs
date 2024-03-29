#-------------------------------------------------
#
# Project created by QtCreator 2019-04-01T18:17:15
#
#-------------------------------------------------

QT       += core gui websockets
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = mqtt_client_qt
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++17 console

SOURCES += \
        main.cpp \
    QPainterWidget.cpp \
    MainWidget.cpp \
    MqttClientWrapper.cpp \
    Generator.cpp \
    DeviceWidget.cpp \
    WebSocketIODevice.cpp

HEADERS += \
    QPainterWidget.h \
    MainWidget.h \
    MqttClientWrapper.h \
    Generator.h \
    DeviceWidget.h \
    WebSocketIODevice.h


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-qtmqtt-Desktop_Qt_5_12_3_MinGW_64_bit-Debug/lib/ -lQt5Mqtt
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-qtmqtt-Desktop_Qt_5_12_3_MinGW_64_bit-Debug/lib/ -lQt5Mqttd
else:unix: LIBS += -L$$PWD/../build-qtmqtt-Desktop_Qt_5_12_3_MinGW_64_bit-Debug/lib/ -lQt5Mqtt

INCLUDEPATH += $$PWD/../build-qtmqtt-Desktop_Qt_5_12_3_MinGW_64_bit-Debug/include
DEPENDPATH += $$PWD/../build-qtmqtt-Desktop_Qt_5_12_3_MinGW_64_bit-Debug/include
