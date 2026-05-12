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


    char input[7] = "002500";
    int inputLen = 6;
};


inline void StartTimer(POMODORO_TIMER &timer)
{
    timer.isRunning = true;
    timer.isPaused = false;
    timer.timeleft = timer.hours * 3600 + timer.minutes * 60 + timer.seconds;
}

inline void UpdateTimer(POMODORO_TIMER &timer)
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


inline void HandleInput(POMODORO_TIMER &timer) {
    int ch = GetCharPressed();
    if (ch >= '0' && ch <= '9' && !timer.isRunning && !timer.isPaused) {
        for (int i = 0; i < 5; i++)
            timer.input[i] = timer.input[i + 1];
        timer.input[5] = ch;

        // OUTSIDE the loop
        timer.hours   = (timer.input[0] - '0') * 10 + (timer.input[1] - '0');
        timer.minutes = (timer.input[2] - '0') * 10 + (timer.input[3] - '0');
        timer.seconds = (timer.input[4] - '0') * 10 + (timer.input[5] - '0');
    }
}

inline void ResetTimer(POMODORO_TIMER &timer) {
    timer.isRunning = false;
    timer.isPaused = false;
    timer.isFinished = false;
    timer.timeleft = 0;
    timer.hours = 0;
    timer.minutes = 0;
    timer.seconds = 0;
    for (int i = 0; i < 6; i++) timer.input[i] = '0';
}
#endif // !TIMER_HPP