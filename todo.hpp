#pragma once

#ifndef TODO_HPP
#define TODO_HPP

#include<string>
#include<raylib.h>
#include<vector>


struct TODO_ITEM
{
    std::string text;
    bool completed;
};

struct TODO_LIST
{
    std::vector<TODO_ITEM> items;
    char input[256] = "";
    int inputLen = 0;
    bool typing = false;
};


void AddTask(TODO_LIST &todo) {
    if(todo.inputLen>0){
        TODO_ITEM item;
        item.text = todo.input;
        todo.items.push_back(item);
        //Reset input text box
        memset(todo.input, 0, 256);
        todo.inputLen = 0;
    }
}

void HandleInputTodo(TODO_LIST &todo) {
   int ch = GetCharPressed();
   if(ch>= 32 && todo.inputLen < 255) {
       todo.input[todo.inputLen] = ch;
       todo.inputLen++;
   }
   if(IsKeyPressed(KEY_BACKSPACE) && todo.inputLen > 0) {
    todo.input[todo.inputLen] = '\0';   
    todo.inputLen--;
   }
    if(IsKeyPressed(KEY_ENTER)) {
         AddTask(todo);
    }
}







#endif // TODO_HPP