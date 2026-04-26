#pragma once


struct TODO_TSK;

void SaveTodoToServer(const char* text);

void LoadTodosFromServer(TODO_TSK &todo);

void DeleteTodoFromServer(const std::string& id);