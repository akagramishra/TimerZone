#pragma once

#ifndef TODO_HPP
#define TODO_HPP

#include "network.hpp"
#include <cstring>
#include <raylib.h>
#include <string>
#include <vector>

struct TODO_ITEM {
  std::string text;
  std::string id; // ID from theen firebase server to identification
  bool completed = false;
};

struct TODO_TSK {
  std::vector<TODO_ITEM> items;
  char input[256] = "";
  int inputLen = 0;
  int cursorPos =0;//added the curosor positon here
  bool typing;
};



//this function causes bugs because it extracts the typed characters before GuiTextBox can get them
inline void HandleInput(TODO_TSK &todotask) {
  if (!todotask.typing) return;

  //getting keyboard input
  int key = GetCharPressed();
  while (key > 0) {
   if (key >= 32 && key <= 126 && todotask.inputLen < 255) {
  // Shift characters right from end down to cursorPos
  for (int i = todotask.inputLen; i > todotask.cursorPos; i--)
    todotask.input[i] = todotask.input[i - 1];
  todotask.input[todotask.cursorPos] = (char)key;
  todotask.cursorPos++;
  todotask.inputLen++;
  todotask.input[todotask.inputLen] = '\0';
}
    key = GetCharPressed();
  }
//static to handle backspace holding for continuous deletion with initial delay and maintina its original state
  static double lastBackspaceTime = 0;
  static bool backspaceHeld = false;
  double currentTime = GetTime();

  bool doBackspace = false;

  if (IsKeyPressed(KEY_BACKSPACE)) {
    // Immediate delete on first press
    doBackspace = true;
    backspaceHeld = false;
    lastBackspaceTime = currentTime;
  } else if (IsKeyDown(KEY_BACKSPACE)) {
    // Hold deletion with delay after initial press (0.5s first delay, then 0.05s repeatition
    double holdDelay = backspaceHeld ? 0.05 : 0.5;
    if (currentTime - lastBackspaceTime >= holdDelay) {
      doBackspace = true;
      backspaceHeld = true;
      lastBackspaceTime = currentTime;
    }
  } else {
    backspaceHeld = false;
  }

  if (doBackspace && todotask.cursorPos > 0) {
    // Delete character and shift left
    for (int i = todotask.cursorPos - 1; i < todotask.inputLen - 1; i++)
      todotask.input[i] = todotask.input[i + 1];
    todotask.input[--todotask.inputLen] = '\0';
    todotask.cursorPos--;
  }

  //Cursor movement appending the position here
  if (IsKeyPressed(KEY_LEFT) && todotask.cursorPos > 0)
    todotask.cursorPos--;
  if (IsKeyPressed(KEY_RIGHT) && todotask.cursorPos < todotask.inputLen)
    todotask.cursorPos++;
  if (IsKeyPressed(KEY_HOME))
    todotask.cursorPos = 0;
  if (IsKeyPressed(KEY_END))
    todotask.cursorPos = todotask.inputLen;
}





inline void AddTask(TODO_TSK &todotask) {
  if (todotask.inputLen > 0) {
    TODO_ITEM newItem;
    newItem.text = std::string(todotask.input);
    newItem.id = SaveTodoToServer(
        newItem.text.c_str()); // save to server and get unique ID that wull be used for deletion later
    todotask.items.push_back(newItem);
    memset(todotask.input, 0, 256);
    todotask.input[0] = '\0';
    todotask.inputLen = 0;
    todotask.cursorPos = 0;
    todotask.typing = false;
  }
}

inline void DeleteTask(TODO_TSK &todo, int index) {
  if (index >= 0 && index < todo.items.size()) {
    DeleteTodoFromServer(todo.items[index].id);
    todo.items.erase(todo.items.begin() + index);
  }
}

#endif