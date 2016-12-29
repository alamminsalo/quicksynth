#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "synthengine.h"
#include "synthinterface.h"
#include <QThread>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    /// \brief Setup synth engine
    SynthEngine *synthengine = SynthEngine::getInstance();

    /// \brief Setup interface
    SynthInterface *interf = SynthInterface::getInstance();
    synthengine->setInterface(interf);

    /// \brief Setup threading
    QThread *t = new QThread();
    synthengine->moveToThread(t);
    t->connect(t, SIGNAL(started()), synthengine, SLOT(run()));
    t->connect(t, SIGNAL(destroyed(QObject*)), synthengine, SLOT(deleteLater()));
    t->start(QThread::TimeCriticalPriority);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QLatin1String("qrc:/main.qml")));

    bool rc = app.exec();

    t->terminate();
    t->deleteLater();

    return rc;
}
