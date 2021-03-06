QT += serialport charts quick core qml

CONFIG += c++11

VERSION = 1.0.0
QMAKE_TARGET_COMPANY = Geirmundur Orri Sigurdsson
QMAKE_TARGET_PRODUCT = Qt Experimental Sandbox
QMAKE_TARGET_DESCRIPTION = Qt Experimental Sandbox
QMAKE_TARGET_COPYRIGHT = Copyright (C) Geirmundur Orri Sigurdsson 2020
QMAKE_CXXFLAGS += /FS

TARGET       = QtExperimentalSandbox

INCLUDEPATH += $$PWD

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Refer to the documentation for the
# deprecated API to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
  qtsandbox.cpp \
  configuration.cpp \
  restriction.cpp \
  accuracy.cpp \
  backend.cpp \
  format.cpp \
  range.cpp \
  ui.cpp

RESOURCES += assets.qrc \
  qml/qtsandbox.qrc

# Debugging files for Release builds
# https://stackoverflow.com/questions/9234337/qt-no-map-pdb-files-generated-for-windows-release-builds
QMAKE_LFLAGS_RELEASE +=/MAP
QMAKE_CFLAGS_RELEASE += /Zi
QMAKE_LFLAGS_RELEASE +=/debug /opt:ref

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = $${PWD}

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH = $${PWD}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
  configuration.h \
  restriction.h \
  accuracy.h \
  backend.h \
  format.h \
  range.h \
  types.h \
  ui.h

DISTFILES += \
  configuration.ini
