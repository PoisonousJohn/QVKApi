CONFIG += c++11
TEMPLATE = lib

QT += webkit webkitwidgets


HEADERS += VKApi.h \
    VKResponse.h \
    VKRequest.h \
    VKRequestManager.h

SOURCES += VKApi.cpp \
    VKResponse.cpp \
    VKRequest.cpp \
    VKRequestManager.cpp


RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(deployment.pri)
