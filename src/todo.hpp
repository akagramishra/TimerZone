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
  std::string id; // unique identifier for server operations from firebase
  bool completed = false;
};

struct TODO_TSK {
  std::vector<TODO_ITEM> items;
  char input[256] = "";
  int inputLen = 0;
  bool typing;
};

// inline does not allow multiple definitions across translation units, so we
// can define these functions in the header without linker errors.

inline void HandleInput(TODO_TSK &todotask) {
  if (todotask.typing) {
    int key = GetCharPressed();
    while (key > 0) {
      if (key == 8 && todotask.inputLen > 0) {
        todotask.input[--todotask.inputLen] = '\0';
      } else if (key >= 32 && key <= 126 && todotask.inputLen < 255) {
        todotask.input[todotask.inputLen++] = (char)key;
        todotask.input[todotask.inputLen] = '\0';
      }
      key = GetCharPressed();
    }
    if ((IsKeyPressed(KEY_BACKSPACE) || IsKeyDown(KEY_BACKSPACE)) &&
        todotask.inputLen > 0) {
      todotask.input[--todotask.inputLen] = '\0';
    }
  }
}

inline void AddTask(TODO_TSK &todotask) {
  if (todotask.inputLen > 0) {
    TODO_ITEM newItem;
    newItem.text = std::string(todotask.input);
    newItem.id = SaveTodoToServer(
        newItem.text.c_str()); // save to server and get unique ID
    todotask.items.push_back(newItem);
    SaveTodoToServer(newItem.text.c_str()); // save to FirebaseDB
    memset(todotask.input, 0, 256);
    todotask.inputLen = 0;
    todotask.typing = false;
  }
}

inline void DeleteTask(TODO_TSK &todo, int index) {
  if (index >= 0 && index < todo.items.size()) {
    DeleteTodoFromServer(todo.items[index].id);
    todo.items.erase(todo.items.begin() + index);
  }
}

#endif // TODO_HPP