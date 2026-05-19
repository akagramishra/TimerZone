#pragma once

#ifndef TIMER_HPP
#define TIMER_HPP

#include "todo.hpp"
#include <raylib.h>
#include <iostream>

struct POMODORO_TIMER
{
    int hours = 0;
    int minutes = 25;
    int seconds = 0;
    float timeleft = 1500;
    bool isRunning = false;

    char input[7] = "002500";
    int inputLen = 6;

};

inline void setTime(POMODORO_TIMER &timer)
{
        timer.timeleft = timer.hours * 3600 + timer.minutes * 60 + timer.seconds;
}

inline void ResetTimer(POMODORO_TIMER &timer)
{
    timer.isRunning = false;
    setTime(timer);
}

inline void UpdateTimer(POMODORO_TIMER &timer)
{

    if (timer.isRunning && timer.timeleft > 0)
    {
        timer.timeleft -= GetFrameTime();
    }

    if (timer.timeleft <= 0 && timer.isRunning)
    {
        ResetTimer(timer);
    }
}

inline void HandleInput(POMODORO_TIMER &timer, TODO_TSK &todo)
{
    if (!todo.typing)
    {
        int ch = GetCharPressed();
        if (ch >= '0' && ch <= '9' && !timer.isRunning)
        {
            for (int i = 0; i < 5; i++)
                timer.input[i] = timer.input[i + 1];
            timer.input[5] = ch;

            timer.hours = (timer.input[0] - '0') * 10 + (timer.input[1] - '0');
            timer.minutes = (timer.input[2] - '0') * 10 + (timer.input[3] - '0');
            timer.seconds = (timer.input[4] - '0') * 10 + (timer.input[5] - '0');
            setTime(timer);
        }
        if (IsKeyPressed(KEY_BACKSPACE) && !timer.isRunning)
        {
            for (int i = 5; i > 0; i--)
                timer.input[i] = timer.input[i-1];
            timer.input[0] = '0';

            timer.hours = (timer.input[0] - '0') * 10 + (timer.input[1] - '0');
            timer.minutes = (timer.input[2] - '0') * 10 + (timer.input[3] - '0');
            timer.seconds = (timer.input[4] - '0') * 10 + (timer.input[5] - '0');
            setTime(timer);
        }
    }
}

#endif // !TIMER_HPP