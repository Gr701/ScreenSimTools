#pragma once 

#include <iostream>
#include <fstream>
#include <memory>
#include <algorithm>
#include <windows.h>
#include <time.h>

#include <d3d11.h>
#include <dxgi1_2.h>

#ifndef GRCOLOR_DEFINED
#define GRCOLOR_DEFINED
struct GrColor 
{
    int r, g, b;
    GrColor();
    GrColor(int r, int g, int b);

    void print();
};
#endif

int checkHrCode(HRESULT hr, const char* processName);

void setupScreenCapture();

void captureScreen();

void stopScreenCapture();

int getColorHEX(int x, int y);

GrColor getColorRGB(int x, int y);

void saveScreenToBMP(const char* path);

void saveScreenToBMPNoAlpha(const char* path);