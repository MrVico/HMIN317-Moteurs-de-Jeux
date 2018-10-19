#ifndef SEASONTIMER_H
#define SEASONTIMER_H

#include <QBasicTimer>
#include <QTimerEvent>
#include "mainwidget.h"

class SeasonTimer: public QOpenGLWidget
{
     Q_OBJECT
public:
    explicit SeasonTimer(float seasonDuration);
    ~SeasonTimer();
    void startTimer();

signals:
    void seasonChange();

protected:
    void timerEvent(QTimerEvent *e) override;

private:
    QBasicTimer timer;
    float seasonDuration;
};

#endif // SEASONTIMER_H
