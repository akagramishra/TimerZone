#include "app.hpp"
#include "ui.hpp"
#include <raylib.h>

Application::Application() {
  windowWidth = 1200;
  windowHeight = 800;
}

Application::~Application() {}

void Application::Init() {
  SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_WINDOW_RESIZABLE);
  InitWindow(windowWidth, windowHeight, "TIMERZONE");
  InitAudioDevice();
  SetWindowMinSize(800, 600);
  SetTargetFPS(60);

  UI::LoadAssets();
  LoadTodosFromServer(todo);
}

void Application::RunLoop() {
  while (!WindowShouldClose()) {
    Update();
    Draw();
  }
}

void Application::Update() {

  //Use this function to add taska and updates tot he server of firebase
  //earlier commit was lazy and did it wrong
  HandleInput(todo);
  
  
  HandleInput(timer,todo);
  UpdateTimer(timer);
}

void Application::Draw() {
  BeginDrawing();
  UI::DrawAppBackground();
  UI::DrawHeader();

  float w = GetScreenWidth();
  float h = GetScreenHeight();

  // Layout margins
  float margin = 40;
  float headerHeight = 80;

  float panelWidth = (w - (margin * 3)) / 2;
  float timerPanelHeight = 350; // Fixed reasonable height for timer
  float todoPanelHeight = h - headerHeight - margin * 2;

  if (panelWidth < 350) {
    // Fallback to vertical layout for narrow windows
    panelWidth = w - margin * 2;
    Rectangle timerBounds = {margin, headerHeight, panelWidth,
                             timerPanelHeight};
    UI::DrawTimerPanel(timer, timerBounds);

    Rectangle todoBounds = {margin, headerHeight + timerPanelHeight + margin,
                            panelWidth,
                            h - (headerHeight + timerPanelHeight + margin * 2)};
    UI::DrawTodoPanel(todo, todoBounds);
  } else {
    // Horizontal layout for wider windows
    Rectangle timerBounds = {margin, headerHeight, panelWidth,
                             timerPanelHeight};
    UI::DrawTimerPanel(timer, timerBounds);

    Rectangle todoBounds = {margin * 2 + panelWidth, headerHeight, panelWidth,
                            todoPanelHeight};
    UI::DrawTodoPanel(todo, todoBounds);
  }

  EndDrawing();
}

void Application::Shutdown() {
  UI::UnloadAssets();
  CloseAudioDevice();
  CloseWindow();
}
