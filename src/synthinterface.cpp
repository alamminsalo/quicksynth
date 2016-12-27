#include "synthinterface.h"
#include "utils.h"

//Initialize singleton as null value
SynthInterface *SynthInterface::s_instance = 0;

SynthInterface *SynthInterface::getInstance(QObject *parent) {
    if (!s_instance) {
        s_instance = new SynthInterface(parent);
    }
    return s_instance;
}

void SynthInterface::setNoteOn(QString note)
{
    emit noteOn(utils::stof(note));
}

SynthInterface::SynthInterface(QObject *parent)
    : QObject(parent)
{
    declareQml();
}

#include <QQmlEngine>
QObject *provider(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Q_UNUSED(engine)
    Q_UNUSED(scriptEngine)

    SynthInterface *eng = SynthInterface::getInstance();
    QQmlEngine::setObjectOwnership(eng, QQmlEngine::CppOwnership);

    return eng;
}

void SynthInterface::declareQml()
{
    //Expose this to qml as a singleton
    qmlRegisterSingletonType<SynthInterface>("com.synth.interface", 1, 0, "Synth", provider);
}
