CONFIG += c++11
TEMPLATE = app

QT += qml quick widgets webkit webkitwidgets


HEADERS += VKApi.h

SOURCES += main.cpp \
            VKApi.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
