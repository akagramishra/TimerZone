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
  bool typing = 0;
};

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