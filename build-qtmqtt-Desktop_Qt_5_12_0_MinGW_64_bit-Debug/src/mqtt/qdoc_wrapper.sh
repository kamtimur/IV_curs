#!/bin/sh
QT_VERSION=5.12.3
export QT_VERSION
QT_VER=5.12
export QT_VER
QT_VERSION_TAG=5123
export QT_VERSION_TAG
QT_INSTALL_DOCS=C:/Qt/Docs/Qt-5.12.0
export QT_INSTALL_DOCS
BUILDDIR=C:/Users/tkn/Desktop/qt_mqtt_project/build-qtmqtt-Desktop_Qt_5_12_0_MinGW_64_bit-Debug/src/mqtt
export BUILDDIR
exec 'C:\Qt\5.12.0\mingw73_64\bin\qdoc.exe' "$@"
