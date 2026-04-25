#pragma once

#ifndef TODO_HPP
#define TODO_HPP

#include<string>
#include<raylib.h>
#include<vector>


struct TODO_ITEM
{
    std::string text;
    bool completed =false;
};

struct TODO_TSK
{
    std::vector<TODO_ITEM> items;
    char input[256] = "";
    int inputLen = 0;
    bool typing;
};


void HandleInput(TODO_TSK &todotask){
    if(todotask.typing){
        int key = GetCharPressed();
        while(key>0){
            if(key ==8 && todotask.inputLen>0){
                todotask.input[--todotask.inputLen] = '\0';
            }
            else if(key >=32 && key <=126 && todotask.inputLen <255){
                todotask.input[todotask.inputLen++] = (char)key;
                todotask.input[todotask.inputLen] = '\0';
            }
            key = GetCharPressed();
        }
        if (IsKeyPressed(KEY_BACKSPACE) || IsKeyDown(KEY_BACKSPACE) && todotask.inputLen > 0) {
            todotask.input[--todotask.inputLen] = '\0';
        }
    
    }
}


void AddTask(TODO_TSK &todotask){
    if(todotask.inputLen >0){
        TODO_ITEM newItem;
        todotask.typing = false;
        newItem.text = std::string(todotask.input);
        todotask.items.push_back(newItem);
        todotask.input[0] = '\0';
        todotask.inputLen = 0;
        
    }
}





#endif // TODO_HPP