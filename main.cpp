#include<raylib.h>
#include<deque>
#include<cstring>

#define RAYGUI_IMPLEMENTATION
#include "Res/includes/raygui.hpp"
#include "timer.hpp"
#include "todo.hpp"


int main()
{

    

SetConfigFlags(FLAG_MSAA_4X_HINT|FLAG_WINDOW_RESIZABLE) ;
    int WINDOW_WIDTH = 1200;
    int WINDOW_HEIGHT = 800;
    
InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "TIMERZONE");
InitAudioDevice();
SetWindowMinSize(WINDOW_WIDTH,WINDOW_HEIGHT);
SetTargetFPS(45);

Font Title_FONT = LoadFontEx("Res/fonts/Regular.ttf", 50, 0, 0);
Font Default_FONT = GetFontDefault();

Sound Strt_Sound = LoadSound("Res/Sound/strtbeep.wav");
Sound End_Sound = LoadSound("Res/Sound/endbeep.wav");

SetTextureFilter(Title_FONT.texture, TEXTURE_FILTER_BILINEAR);


Color BACKGRND_COLOR = {23, 24, 43, 255};
Color TIMER_BOX_CLR = {56, 63, 92, 250};
Color BUTTON_ONE_CLR = {0, 228, 48, 255};
Color BUTTON_SCD_CLR = {74, 108, 117, 250};
Color BUTTON_TRD_CLR = {55, 60, 82, 250};

Color Action_Buttons[] = {BUTTON_ONE_CLR, BUTTON_SCD_CLR, BUTTON_TRD_CLR};


POMODORO_TIMER timer;
TODO_TSK todo;

while(!WindowShouldClose())
{
    //<----UPDATE THINGS HERE------->
    int ScreenWidth = GetScreenWidth();
    float boxW = GetScreenWidth() * 0.5f;
    float boxH = GetScreenHeight() * 0.4f;
    float boxX = (GetScreenWidth() - boxW) / 4;
    float boxY = (GetScreenHeight() - boxH) / 4;

    //TODO List conts
    float todoX = boxX;
    float todoY = boxY + boxH + 100;  // below buttons
    float todoW = boxW;

    //Timer conts
    int hour = (int)timer.timeleft / 3600;
    int minute = ((int)timer.timeleft % 3600) / 60;
    int second = (int)timer.timeleft % 60;

    //Progress Bar
    
    float totalTime = (timer.hours * 3600) + (timer.minutes * 60) + timer.seconds;
    float progress = 1.0f - (timer.timeleft / totalTime);

    float barX = boxX + 20;
    float barY = boxY + boxH - 30;
    float barW = boxW - 40;
    float barH = 10;

    char timeText[20];
    char inputText_TODO[256];
if (timer.isRunning || timer.isPaused) {
    sprintf(timeText, "%02d:%02d:%02d", hour, minute, second);
} else {
    sprintf(timeText, "%02d:%02d:%02d", timer.hours, timer.minutes, timer.seconds);
}

    Rectangle Button_ONE = {boxX, boxY + boxH + 20, boxW * 0.3f, 50};
    Rectangle Button_TWO = {boxX + boxW * 0.35f, boxY + boxH + 20, boxW * 0.3f, 50};
    Rectangle Button_THREE = {boxX + boxW * 0.7f, boxY + boxH + 20, boxW * 0.3f, 50};

    Rectangle timerBox = {boxX, boxY, boxW, boxH};
    Rectangle timer_Progress = {boxX-10, boxY+boxH+13, boxW + 20, boxH*0.20f};


    //<-----INPUTS HERE------>
    if(CheckCollisionPointRec(GetMousePosition(), Button_ONE) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        StartTimer(timer);
        BUTTON_ONE_CLR = YELLOW;
        PlaySound(Strt_Sound);
    }

   if (CheckCollisionPointRec(GetMousePosition(), Button_TWO) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    if (timer.isRunning) {
        timer.isRunning = false;
        timer.isPaused = true;
    } else if (timer.isPaused) {
        timer.isRunning = true;
        timer.isPaused = false;
    }
    PlaySound(Strt_Sound);
}

     if(CheckCollisionPointRec(GetMousePosition(), Button_THREE) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        ResetTimer(timer);
        PlaySound(End_Sound);
    }


    if (IsKeyPressed(KEY_ENTER) && todo.typing) {
    AddTask(todo);
    }

// Click to focus input
if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    todo.typing = CheckCollisionPointRec(GetMousePosition(), {todoX, todoY + 40, todoW - 90, 40});
}

     HandleInput(todo);
    HandleInput(timer);

    UpdateTimer(timer);
    

    //<----Draw HERE INIT----->
    BeginDrawing();
    ClearBackground(BACKGRND_COLOR);
    

    //Tit for my Intrfce
    DrawTextEx(Title_FONT, "POMODORO TIMER", {(ScreenWidth - MeasureText("POMODORO TIMER", 50))/2, 10}, 50, 3, WHITE);

    //TimeR encloser
    DrawRectangleRoundedLines(timerBox,0.3f,10, TIMER_BOX_CLR);


    //Timer otpt
    Vector2 textsize = MeasureTextEx(Default_FONT ,timeText, 120,3);
    Vector2 timer_pos = {boxX+(boxW/4), boxY + boxH/4};
    DrawTextEx(Default_FONT, timeText, timer_pos, 60, 7, WHITE);



    // Background
    DrawRectangleRounded({barX, barY, barW, barH}, 0.5f, 10, DARKGRAY);

    // Progress
    DrawRectangleRounded({barX, barY, barW * progress, barH}, 0.5f, 10, GREEN);


    //Button Bar
    DrawRectangleRoundedLines(timer_Progress,0.4f, 20, TIMER_BOX_CLR);


    //Buttons

    

    DrawRectangleRounded(Button_ONE, 0.8f, 20, Action_Buttons[0]);
    DrawRectangleRoundedLinesEx(Button_ONE, 0.8f, 20, 3,Fade(BLACK, 220));
    int t1W = MeasureText("START", 30);
    DrawText("START", Button_ONE.x + (Button_ONE.width - t1W) / 2, Button_ONE.y + (Button_ONE.height - 30) / 2, 30, BLACK);

    DrawRectangleRounded(Button_TWO, 0.8f, 20, Action_Buttons[1]);
    DrawRectangleRoundedLinesEx(Button_TWO, 0.8f, 20, 3,Fade(BLACK, 220));
    int t2W = MeasureText("|| PAUSE", 30);
    DrawText("|| PAUSE", Button_TWO.x + (Button_TWO.width - t2W) / 2, Button_TWO.y + (Button_TWO.height - 30) / 2, 30, BLACK);


    DrawRectangleRounded(Button_THREE, 0.8f, 20, Action_Buttons[2]);
    DrawRectangleRoundedLinesEx(Button_THREE, 0.8f, 20, 3,Fade(BLACK, 220));
    int t3W = MeasureText(">> SKIP", 30);
    DrawText(">> RESET", Button_THREE.x + (Button_THREE.width - t3W) / 2, Button_THREE.y + (Button_THREE.height - 30) / 2, 30, BLACK);


    //Todolist
    // Todo section title
DrawTextEx(Title_FONT, "TODO LIST", {todoX, todoY}, 30, 3, WHITE);

// Input box + Add button
GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, ColorToInt(WHITE));
GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, ColorToInt(TIMER_BOX_CLR));
GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, ColorToInt(TIMER_BOX_CLR));

if (GuiTextBox({todoX, todoY + 40, todoW - 90, 40}, todo.input, 256, todo.typing)) {
    todo.typing = !todo.typing;
}

// Add button
GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, ColorToInt({0, 228, 48, 255}));
GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, ColorToInt(BLACK));
if (GuiButton({todoX + todoW - 80, todoY + 40, 80, 40}, "ADD")) {
    AddTask(todo);
}

// Todo items with checkboxes
for (size_t i = 0; i < todo.items.size(); i++) {
    Color textClr = todo.items[i].completed ? GRAY : WHITE;
    GuiCheckBox({todoX, todoY + 95 + (float)(i * 35), 20, 20}, 
                todo.items[i].text.c_str(), 
                &todo.items[i].completed);
}

    


    EndDrawing();

}

UnloadFont(Title_FONT);
UnloadSound(Strt_Sound);
UnloadSound(End_Sound);

CloseWindow();



}