QT += qml quick multimedia

CONFIG += c++11

DEFINES += __MACOSX_CORE__

SOURCES += src/main.cpp 

HEADERS += src/synth.h 

RESOURCES += qml.qrc

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/Cellar/stk/4.5.0/lib -lstk \
        -framework CoreAudio \
        -framework CoreFoundation

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
