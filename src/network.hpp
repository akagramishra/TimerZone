#pragma once
#include <string>

struct TODO_TSK;

std::string SaveTodoToServer(const char* text);

void LoadTodosFromServer(TODO_TSK &todo);

void DeleteTodoFromServer(const std::string& id);