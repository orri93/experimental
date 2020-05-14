INCLUDEPATH += \
  $$PWD

SOURCES += \
  $$PWD/myplugin.cpp \
  $$PWD/myquickitem.cpp

HEADERS += \
  $$PWD/myplugin.h \
  $$PWD/myquickitem.h

RESOURCES += \
  $$PWD/qml/plugin.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

DISTFILES += \
  $$PWD/qmldir
