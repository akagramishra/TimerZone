#pragma once
#ifndef APP_HPP
#define APP_HPP

#include "timer.hpp"
#include "todo.hpp"

class Application {
public:
    Application();
    ~Application();

    void Init();
    void RunLoop();
    void Shutdown();

private:
    void Update();
    void Draw();

    POMODORO_TIMER timer;
    TODO_TSK todo;
    
    int windowWidth;
    int windowHeight;
};

#endif // APP_HPP
