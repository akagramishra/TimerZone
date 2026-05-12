#include "ui.hpp"
#include <cmath>
#include <cstdio>

#define RAYGUI_IMPLEMENTATION
#include "../Res/includes/raygui.hpp"

namespace UI {
const Color BgColor = {24, 26, 32, 255};              // #181A20
const Color PanelColor = {32, 35, 42, 255};           // #20232A
const Color PanelBorderColor = {50, 54, 66, 255};     // #323642
const Color PrimaryAccent = {118, 224, 94, 255};      // #76E05E Green
const Color SecondaryAccent = {0, 229, 255, 255};     // #00E5FF Cyan
const Color TimerBoxColor = {29, 32, 40, 255};        // #1D2028
const Color TimerBorderColor = {44, 49, 60, 255};     // #2C313C
const Color InactiveSegmentColor = {58, 63, 76, 255}; // #3A3F4C
const Color TextColor = {224, 224, 224, 255};         // #E0E0E0
const Color TextMutedColor = {139, 146, 165, 255};    // #8B92A5
const Color ButtonDarkColor = {42, 48, 60, 255};      // #2A303C

Font TitleFont;
Font DefaultFont;
Sound StartSound;
Sound EndSound;
Texture2D Icon;

void LoadAssets() {
  TitleFont = LoadFontEx("Res/fonts/Regular.ttf", 50, 0, 0);
  DefaultFont = GetFontDefault();
  StartSound = LoadSound("Res/Sound/strtbeep.wav");
  EndSound = LoadSound("Res/Sound/endbeep.wav");

  Image iconImg = LoadImage("Res/images/icon.png");
  Icon = LoadTextureFromImage(iconImg);
  SetWindowIcon(iconImg);
  UnloadImage(iconImg);

  SetTextureFilter(TitleFont.texture, TEXTURE_FILTER_BILINEAR);

  // Global styling setup (taaki sab clean lage)
  GuiSetStyle(DEFAULT, BACKGROUND_COLOR, ColorToInt(BgColor));
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
}

void UnloadAssets() {
  UnloadFont(TitleFont);
  UnloadSound(StartSound);
  UnloadSound(EndSound);
  UnloadTexture(Icon);
}

void DrawModernButton(Rectangle bounds, const char *text, Color baseColor,
                      Color hoverColor, Color textColor, bool &isClicked) {
  bool isHovered = CheckCollisionPointRec(GetMousePosition(), bounds);
  Color currentColor = isHovered ? hoverColor : baseColor;

  // Primary accent pe halka sa glow effect
  if (baseColor.r == PrimaryAccent.r && baseColor.g == PrimaryAccent.g) {
    Rectangle shadow = {bounds.x - 3, bounds.y - 3, bounds.width + 6,
                        bounds.height + 6};
    DrawRectangleRounded(shadow, 1.0f, 20,
                         {baseColor.r, baseColor.g, baseColor.b, 60});
  }

  DrawRectangleRounded(bounds, 1.0f, 20, currentColor);

  int textW = MeasureText(text, 20);
  DrawText(text, bounds.x + (bounds.width - textW) / 2,
           bounds.y + (bounds.height - 20) / 2, 20, textColor);

  if (isHovered && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    isClicked = true;
  } else {
    isClicked = false;
  }
}

void DrawTimerPanel(POMODORO_TIMER &timer, Rectangle bounds) {
  // Panels ke outer borders
  Rectangle borderRec = {bounds.x - 2, bounds.y - 2, bounds.width + 4,
                         bounds.height + 4};
  DrawRectangleRounded(borderRec, 0.05f, 10, PanelBorderColor);
  DrawRectangleRounded(bounds, 0.05f, 10, PanelColor);

  // Andar wala timer box setup
  Rectangle timerBoxBorder = {bounds.x + 38, bounds.y + 38, bounds.width - 76,
                              bounds.height * 0.45f + 4};
  DrawRectangleRounded(timerBoxBorder, 0.1f, 10, TimerBorderColor);
  Rectangle timerBox = {bounds.x + 40, bounds.y + 40, bounds.width - 80,
                        bounds.height * 0.45f};
  DrawRectangleRounded(timerBox, 0.1f, 10, TimerBoxColor);

  // Time calculations (hours, mins, secs nikalna)
  int hour = (int)timer.timeleft / 3600;
  int minute = ((int)timer.timeleft % 3600) / 60;
  int second = (int)timer.timeleft % 60;

  int dispHour = (timer.isRunning || timer.isPaused) ? hour : timer.hours;
  int dispMinute = (timer.isRunning || timer.isPaused) ? minute : timer.minutes;
  int dispSecond = (timer.isRunning || timer.isPaused) ? second : timer.seconds;

  const char *timeText;
  if (dispHour > 0) {
    timeText = TextFormat("%02d:%02d:%02d", dispHour, dispMinute, dispSecond);
  } else {
    timeText = TextFormat("%02d:%02d", dispMinute, dispSecond); // E.g., "24:59"
  }

  // Timer text draw karna
  int fontSize = 90;
  Vector2 textsize = MeasureTextEx(TitleFont, timeText, fontSize, 5);

  Vector2 timer_pos = {timerBox.x + (timerBox.width / 2 - textsize.x / 2),
                       timerBox.y + (timerBox.height - fontSize) / 2};

  // Neon glow effect add kar diya
  DrawTextEx(TitleFont, timeText, {timer_pos.x, timer_pos.y + 2}, fontSize, 5,
             {SecondaryAccent.r, SecondaryAccent.g, SecondaryAccent.b, 60});
  DrawTextEx(TitleFont, timeText, timer_pos, fontSize, 5, SecondaryAccent);

  // Niche wala segmented progress bar
  float totalTime = (timer.hours * 3600) + (timer.minutes * 60) + timer.seconds;
  float progress = 0.0f;
  if (totalTime > 0)
    progress = 1.0f - (timer.timeleft / totalTime);
  if (progress < 0.0f)
    progress = 0.0f;
  if (progress > 1.0f)
    progress = 1.0f;

  float barW = bounds.width - 80;
  float barH = 34;
  float barX = bounds.x + 40;
  float barY = timerBox.y + timerBox.height + 20;

  Rectangle pBarBorder = {barX - 2, barY - 2, barW + 4, barH + 4};
  DrawRectangleRounded(pBarBorder, 0.2f, 10, TimerBorderColor);
  Rectangle pBarBox = {barX, barY, barW, barH};
  DrawRectangleRounded(pBarBox, 0.2f, 10, TimerBoxColor);

  int numSegments = 10;
  float spacing = 6.0f;
  float segmentW = (barW - 16 - (numSegments - 1) * spacing) / numSegments;
  float segmentH = barH - 8;

  int activeSegments = (int)(progress * numSegments);

  for (int i = 0; i < numSegments; i++) {
    float segX = barX + 8 + i * (segmentW + spacing);
    float segY = barY + 4;
    Color segColor =
        (i < activeSegments) ? SecondaryAccent : InactiveSegmentColor;

    // Active progress segments pe halka glow
    if (i < activeSegments) {
      DrawRectangleRounded(
          {segX - 1, segY - 1, segmentW + 2, segmentH + 2}, 0.2f, 10,
          {SecondaryAccent.r, SecondaryAccent.g, SecondaryAccent.b, 100});
    }
    DrawRectangleRounded({segX, segY, segmentW, segmentH}, 0.2f, 10, segColor);
  }

  // Niche ke action buttons (START, PAUSE, SKIP)
  float btnSpacing = 15.0f;
  float btnW = (bounds.width - 80 - (2 * btnSpacing)) / 3;
  float btnH = 50;
  float btnY = barY + barH + 30;

  Rectangle btn1 = {bounds.x + 40, btnY, btnW, btnH};
  Rectangle btn2 = {btn1.x + btnW + btnSpacing, btnY, btnW, btnH};
  Rectangle btn3 = {btn2.x + btnW + btnSpacing, btnY, btnW, btnH};

  bool clicked1, clicked2, clicked3;
  DrawModernButton(btn1, "▶ START", PrimaryAccent, {140, 255, 110, 255},
                   {20, 40, 20, 255}, clicked1);
  DrawModernButton(btn2, "|| PAUSE", ButtonDarkColor, {60, 68, 80, 255},
                   SecondaryAccent, clicked2);
  DrawModernButton(btn3, ">> SKIP", ButtonDarkColor, {60, 68, 80, 255},
                   TextMutedColor, clicked3);

  if (clicked1) {
    StartTimer(timer);
    PlaySound(StartSound);
  }
  if (clicked2) {
    if (timer.isRunning) {
      timer.isRunning = false;
      timer.isPaused = true;
    } else if (timer.isPaused) {
      timer.isRunning = true;
      timer.isPaused = false;
    }
    PlaySound(StartSound);
  }
  if (clicked3) {
    ResetTimer(timer);
    PlaySound(EndSound);
  }
}

void DrawTodoPanel(TODO_TSK &todo, Rectangle bounds) {
  Rectangle borderRec = {bounds.x - 2, bounds.y - 2, bounds.width + 4,
                         bounds.height + 4};
  DrawRectangleRounded(borderRec, 0.05f, 10, PanelBorderColor);
  DrawRectangleRounded(bounds, 0.05f, 10, PanelColor);

  Rectangle headerBg = {bounds.x, bounds.y, bounds.width, 40};
  DrawRectangleRounded(headerBg, 0.05f, 10, ButtonDarkColor);
  DrawRectangleRec({bounds.x, bounds.y + 20, bounds.width, 20},
                   ButtonDarkColor);
  DrawText("TODAY'S LOG / TODO", bounds.x + 20, bounds.y + 12, 18, TextColor);

  // Input textbox setup
  GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(TextColor));
  GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(ButtonDarkColor));
  GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(TimerBorderColor));
  GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

  Rectangle inputRec = {bounds.x + 20, bounds.y + 60, bounds.width - 120, 45};

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    todo.typing = CheckCollisionPointRec(GetMousePosition(), inputRec);
  }
  if (GuiTextBox(inputRec, todo.input, 256, todo.typing)) {
    todo.typing = !todo.typing;
  }

  bool addClicked;
  DrawModernButton({bounds.x + bounds.width - 90, bounds.y + 60, 70, 45}, "ADD",
                   ButtonDarkColor, {60, 68, 80, 255}, SecondaryAccent,
                   addClicked);
  if (addClicked || (IsKeyPressed(KEY_ENTER) && todo.typing)) {
    AddTask(todo);
  }

  // Todo items ki list render karna
  float itemY = bounds.y + 130;
  for (size_t i = 0; i < todo.items.size(); i++) {
    if (itemY > bounds.y + bounds.height - 50)
      break;

    Color textClr = todo.items[i].completed ? TextMutedColor : TextColor;

    Rectangle checkRec = {bounds.x + 20, itemY, 26, 26};
    GuiCheckBox(checkRec, "", &todo.items[i].completed);

    int textY = itemY + 3;
    DrawText(todo.items[i].text.c_str(), bounds.x + 60, textY, 20, textClr);

    if (todo.items[i].completed) {
      int tw = MeasureText(todo.items[i].text.c_str(), 20);
      DrawLine(bounds.x + 60, textY + 10, bounds.x + 60 + tw, textY + 10,
               TextMutedColor);
    }

    bool delClicked;
    DrawModernButton({bounds.x + bounds.width - 55, itemY, 35, 26}, "X",
                     ButtonDarkColor, {60, 68, 80, 255}, TextMutedColor,
                     delClicked);
    if (delClicked) {
      DeleteTask(todo, i);
      break; // break to avoid iterator issues
    }

    itemY += 45;
  }
}

void DrawAppBackground() { ClearBackground(BgColor); }

void DrawHeader() {
  int ScreenWidth = GetScreenWidth();
  DrawText("C++ / RAYLIB POMODORO TIMER v1.0", 40, 20, 20, TextMutedColor);
}
} // namespace UI
