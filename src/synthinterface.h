#ifndef SYNTHINTERFACE_H
#define SYNTHINTERFACE_H

#include <QObject>

class SynthInterface: public QObject
{
    Q_OBJECT

    static SynthInterface *s_instance;

    void declareQml();
    explicit SynthInterface(QObject *parent = 0);

public:
    static SynthInterface *getInstance(QObject *parent = 0);

public slots:
    void setNoteOn(QString note);

signals:
    void noteOn(float key);
    void noteOff();
    void pitch(float pitch);
    void squareAmount(float amount);
    void detuneAmount(float amount);
    void unisonCount(unsigned int count);
};

#endif // SYNTHINTERFACE_H
