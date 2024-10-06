#pragma once

#include <iostream>
#include <windows.h>
#include <algorithm> 
#include <cmath>


#ifndef GRCOLOR_DEFINED
#define GRCOLOR_DEFINED
//#define IMPLEMENT_GRCOLOR
struct GrColor {
    int r, g, b;
    GrColor();
    GrColor(int r, int g, int b);
    void print();
}; 
#endif

struct GrPoint {
    int x, y;
    GrPoint();
    GrPoint(int x, int y);
    GrPoint operator+(const GrPoint & rhs);
    GrPoint & operator+=(const GrPoint & rhs);
    GrPoint operator-(const GrPoint & rhs);
    GrPoint operator*(int c);
};

struct GrRect {
    int x, y, width, height;
    GrRect();
    GrRect(int x, int y, int width, int height);
};

void createWindow(int x, int y, int width, int height, DWORD style);

void makeWindowTransparent(int r, int g, int b);

void closeWindow();

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

bool processMessages(); //should be called in cycle, returns false if window is closed

void renderImage();

void clearRender(const GrColor& color);

void drawPixel(int x, int y, const GrColor& color);

void drawPixel(GrPoint point, const GrColor& color);

void drawLine(GrPoint point1, GrPoint point2, const GrColor& color);

void drawLine(GrPoint point1, GrPoint point2, const GrColor& color, int thickness);

void drawRect(const GrRect& rect, const GrColor& color);

void fillRect(int x, int y, int width, int height, const GrColor& color);

void fillRect(const GrRect& rect, const GrColor& color);

void drawCircle(GrPoint center, int radius, const GrColor& color);

void fillCircle(GrPoint center, int radius, const GrColor& color);

void draw1dBuffer(int x, int y, int width, int height, int* buffer);

void draw3dBuffer(int x, int y, int width, int height, int* buffer);
