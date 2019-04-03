#!/bin/sh
PATH=/C/Users/tkn/Desktop/qt_mqtt_project/build-qtmqtt-Desktop_Qt_5_12_0_MinGW_64_bit-Debug/bin:/C/Qt/5.12.0/mingw73_64/bin:$PATH
export PATH
QT_PLUGIN_PATH=/C/Qt/5.12.0/mingw73_64/plugins${QT_PLUGIN_PATH:+:$QT_PLUGIN_PATH}
export QT_PLUGIN_PATH
exec "$@"
