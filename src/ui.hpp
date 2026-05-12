#pragma once
#ifndef UI_HPP
#define UI_HPP

#include "timer.hpp"
#include "todo.hpp"
#include <raylib.h>

namespace UI {
// Colors Used Extern for entire file usage
extern const Color BgColor;
extern const Color PanelColor;
extern const Color PrimaryAccent;
extern const Color SecondaryAccent;
extern const Color TextColor;
extern const Color TextMutedColor;

// Assets
extern Font TitleFont;
extern Font DefaultFont;
extern Sound StartSound;
extern Sound EndSound;
extern Texture2D Icon;

void LoadAssets();
void UnloadAssets();

void DrawModernButton(Rectangle bounds, const char *text, Color baseColor,
                      Color hoverColor, bool &isClicked);

void DrawTimerPanel(POMODORO_TIMER &timer, Rectangle bounds);
void DrawTodoPanel(TODO_TSK &todo, Rectangle bounds);

void DrawAppBackground();
void DrawHeader();
} // namespace UI

#endif // UI_HPP
