QT += qml quick multimedia

CONFIG += c++11

SOURCES += src/main.cpp \ 
    src/synthengine.cpp \
    src/synth.cpp \
    src/waveform.cpp \
    src/synthinterface.cpp

HEADERS += src/synth.h \ 
    src/synthengine.h \
    src/utils.h \
    src/global.h \
    src/waveform.h \
    src/synthinterface.h

RESOURCES += qml.qrc

macx:{
        DEFINES += __MACOSX_CORE__
	INCLUDEPATH += /usr/local/include
	LIBS += -L/usr/local/Cellar/stk/4.5.0/lib -lstk \
		-framework CoreAudio \
		-framework CoreFoundation
}

unix: !macx: {
    DEFINES += __UNIX_JACK__
    LIBS += -lstk
}

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
