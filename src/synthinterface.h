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
    void noteOn(int key);
    void noteOff(int key);
    void pitch(float pitch);
    void squareAmount(float amount);
    void detuneAmount(float amount);
    void unisonCount(unsigned int count);

    // ADSR
    void attack(float attack);
    void decay(float decay);
    void sustain(float sustain);
    void release(float release);
};

#endif // SYNTHINTERFACE_H
