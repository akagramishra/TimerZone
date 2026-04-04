#pragma once

#ifndef TIMER_HPP
#define TIMER_HPP

#include <raylib.h>

struct POMODORO_TIMER
{
    int hours = 0;
    int minutes = 25;
    int seconds= 0;
    float timeleft = 0;
    bool isRunning = false;
    bool isPaused = false;
    bool isFinished = false;
};


void StartTimer(POMODORO_TIMER &timer)
{
    timer.isRunning = true;
    timer.isPaused = false;
    timer.timeleft = timer.hours * 3600 + timer.minutes * 60 + timer.seconds;
}

void UpdateTimer(POMODORO_TIMER &timer)
{

    if(timer.isRunning && timer.timeleft>0){
        timer.timeleft -=GetFrameTime();
    }

    if(timer.timeleft<=0 &&timer.isRunning){
        timer.isRunning = false;
        timer.timeleft = 0;
        timer.isFinished = true;
    }
}









#endif // !TIMER_HPP