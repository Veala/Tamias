#include "globalstate.h"

int GlobalState::globalState = FREE;
int GlobalState::isTimerRunning = 0;

void GlobalState::setGlobalState(int gs)
{
    globalState = gs;
}

int GlobalState::getGlobalState() const
{
    return globalState;
}

void GlobalState::setTimerState(int t)
{
    isTimerRunning = t;
}

int GlobalState::getTimerState() const
{
    return isTimerRunning;
}
