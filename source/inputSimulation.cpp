#include "inputSimulation.h"

void leftClick()
{
    INPUT inputs[2];
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
    inputs[1].mi.time = 0;
    SendInput(2, &inputs[0], sizeof(INPUT));
}

void doubleClick()
{
    leftClick();
    leftClick();
}

void LMBDown()
{
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
    SendInput(1, &input, sizeof(INPUT));
}

void LMBUp()
{
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_LEFTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void rightClick()
{
    INPUT inputs[2];
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    inputs[1].mi.time = 0;
    SendInput(2, &inputs[0], sizeof(INPUT));
}

void RMBDown()
{
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
    SendInput(1, &input, sizeof(INPUT));
}

void RMBUp()
{
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;
    SendInput(1, &input, sizeof(INPUT));
}

void middleClick()
{
    INPUT inputs[2];
    inputs[0].type = INPUT_MOUSE;
    inputs[0].mi.dwFlags = MOUSEEVENTF_MIDDLEDOWN;
    inputs[1].type = INPUT_MOUSE;
    inputs[1].mi.dwFlags = MOUSEEVENTF_MIDDLEUP;
    inputs[1].mi.time = 0;
    SendInput(2, &inputs[0], sizeof(INPUT));
}

void moveMouseRelatively(int dx, int dy)
{
    INPUT input;
    input.type = INPUT_MOUSE;
    input.mi.dwFlags = MOUSEEVENTF_MOVE;
    input.mi.time = 0;
    input.mi.dx = dx;
    input.mi.dy = dy;
    SendInput(1, &input, sizeof(input));
}

void clickKey(int key)
{
    INPUT inputs[2];
    inputs[0].type = INPUT_KEYBOARD;
    inputs[0].ki.dwFlags = KEY_EVENT;
    inputs[0].ki.wVk = key;
    inputs[1].type = INPUT_KEYBOARD;
    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP;
    inputs[1].ki.wVk = key;
    SendInput(2, &inputs[0], sizeof(INPUT));
}

void pressKey(int key)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = 0;
    input.ki.wVk = key;
    SendInput(1, &input, sizeof(INPUT));
}

void releaseKey(int key)
{
    INPUT input;
    input.type = INPUT_KEYBOARD;
    input.ki.dwFlags = KEYEVENTF_KEYUP;
    input.ki.wVk = key;
    SendInput(1, &input, sizeof(INPUT));
}