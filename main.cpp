#include<raylib.h>
#include<deque>
#include<cstdio>

#include "timer.hpp"


int main()
{
    

SetConfigFlags(FLAG_MSAA_4X_HINT|FLAG_WINDOW_RESIZABLE) ;
    int WINDOW_WIDTH = 1200;
    int WINDOW_HEIGHT = 800;
    
InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Monetrafy-WINDOWS TIMER");
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

while(!WindowShouldClose())
{
    //<----UPDATE THINGS HERE------->
    int ScreenWidth = GetScreenWidth();
    float boxW = GetScreenWidth() * 0.5f;
    float boxH = GetScreenHeight() * 0.4f;
    float boxX = (GetScreenWidth() - boxW) / 4;
    float boxY = (GetScreenHeight() - boxH) / 4;

    //Timer conts
    int hour = (int)timer.timeleft / 3600;
    int minute = ((int)timer.timeleft % 3600) / 60;
    int second = (int)timer.timeleft % 60;

    char timeText[20];
    sprintf(timeText, "%02d:%02d:%02d", hour, minute, second);

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

     if(CheckCollisionPointRec(GetMousePosition(), Button_TWO) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        timer.isRunning = !timer.isRunning;
        timer.isPaused = !timer.isPaused;
        BUTTON_SCD_CLR.a = 120;
        PlaySound(Strt_Sound);
    }

     if(CheckCollisionPointRec(GetMousePosition(), Button_THREE) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        timer.isRunning = false;
        timer.timeleft = 0;
        BUTTON_TRD_CLR.a = 120;
        PlaySound(End_Sound);
    }



    UpdateTimer(timer);

    //<----Draw HERE INIT----->
    BeginDrawing();
    ClearBackground(BACKGRND_COLOR);
    

    //Tit for my Intrfce
    DrawTextEx(Title_FONT, "POMODORO TIMER", {(ScreenWidth - MeasureText("POMODORO TIMER", 50))/2, 10}, 50, 3, WHITE);

    //TimeR encloser
    DrawRectangleRoundedLines(timerBox,0.3f,10, TIMER_BOX_CLR);


    //Timer otpt
    Vector2 textsize = MeasureTextEx(Title_FONT ,timeText, 60,3);
    DrawTextEx(Title_FONT, timeText, {(GetScreenWidth() - textsize.x) / 2, boxY + boxH / 2 - 30}, 60, 3, WHITE);


    //Progress bar
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



    EndDrawing();

}

UnloadFont(Title_FONT);
UnloadSound(Strt_Sound);
UnloadSound(End_Sound);

CloseWindow();



}