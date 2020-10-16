#ifndef GLOBALSTATE_H
#define GLOBALSTATE_H

#define FREE 0
#define BUSY 1

class GlobalState
{
private:
    static int globalState;
    static int isTimerRunning;
public:
    void setGlobalState(int);
    int getGlobalState() const;
    void setTimerState(int);
    int getTimerState() const;

};

#endif // GLOBALSTATE_H
