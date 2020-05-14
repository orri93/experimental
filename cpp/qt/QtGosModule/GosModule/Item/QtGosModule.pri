INCLUDEPATH += \
  $$PWD

SOURCES += \
  $$PWD/model/interval.cpp \
  $$PWD/validation/float.cpp

HEADERS += \
  $$PWD/model/interval.h \
  $$PWD/validation/float.h

RESOURCES += \
    $$PWD/qml/gosmodule.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH = 

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

DISTFILES += \
  $$PWD/qmldir