#include "seasontimer.h"
#include <iostream>

SeasonTimer::SeasonTimer(float seasonDuration):
    seasonDuration(seasonDuration)
{
}

SeasonTimer::~SeasonTimer()
{
}

void SeasonTimer::startTimer()
{
    std::cout << "Timer started..." << std::endl;
    timer.start(seasonDuration, this);
}

void SeasonTimer::timerEvent(QTimerEvent *)
{
    std::cout << "The season changed!" << std::endl;
    emit seasonChange();
}
