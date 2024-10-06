#pragma once

#include <windows.h>

void leftClick();
void doubleClick();
void LMBDown();
void LMBUp();

void rightClick();
void RMBDown();
void RMBUp();

void middleClick();

void moveMouseRelatively(int dx, int dy);

void clickKey(int key);
void pressKey(int key);
void releaseKey(int key);

//GetAsyncKeyState(key);
//SetCursorPos(x, y);