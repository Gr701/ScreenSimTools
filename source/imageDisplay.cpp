#include "imageDisplay.h"

HINSTANCE hInstance;
HWND hWnd;
WNDCLASSA wndClass;
const char* className = "over way"; 

HBITMAP hBitmapToDraw;
HDC deviceContextToDraw;
BITMAPINFO bitmapInfo;

int clientWidth, clientHeight;

BYTE* displayBuffer;

#ifdef IMPLEMENT_GRCOLOR
GrColor::GrColor(): r(0), g(0), b(0) {}
GrColor::GrColor(int r, int g, int b): r(r), g(g), b(b) {}
void GrColor::print() { std::cout << "r - " << r << " g - " << g << " b - " << b << std::endl; }
#endif

GrPoint::GrPoint(): x(0), y(0) {}
GrPoint::GrPoint(int x, int y): x(x), y(y) {}
GrPoint GrPoint::operator+(const GrPoint & rhs) {
    return GrPoint(this->x + rhs.x, this->y + rhs.y);
}
GrPoint & GrPoint::operator+=(const GrPoint & rhs) {
    this->x += rhs.x;
    this->y += rhs.y;
    return *this;
}
GrPoint GrPoint::operator-(const GrPoint & rhs) {
    return GrPoint(this->x - rhs.x, this->y - rhs.y);
}
GrPoint GrPoint::operator*(int c) {
    return GrPoint(this->x * c, this->y * c);
}

GrRect::GrRect(): x(0), y(0), width(0), height(0) {}
GrRect::GrRect(int x, int y, int w, int h): x(x), y(y), width(w), height(h) {}

void renderImage() {
    SetDIBitsToDevice(deviceContextToDraw, 0, 0, clientWidth, clientHeight, 0, 0, 0, clientHeight, displayBuffer, &bitmapInfo, DIB_RGB_COLORS);
}

void clearRender(const GrColor& color) {
    for (int i = 0; i < clientWidth * clientHeight * 4; i += 4) {
        displayBuffer[i + 0] =  color.b;
        displayBuffer[i + 1] =  color.g;
        displayBuffer[i + 2] =  color.r;
        displayBuffer[i + 3] =  1;
    }
}

void drawPixel(int x, int y, const GrColor& color) {
    int pixelOffset = y * bitmapInfo.bmiHeader.biWidth * 4 + x * 4;
    displayBuffer[pixelOffset] = color.b;
    displayBuffer[pixelOffset + 1] = color.g;
    displayBuffer[pixelOffset + 2] = color.r;
}

void drawPixel(GrPoint point, const GrColor& color) {
    drawPixel(point.x, point.y, color);
}

void drawLine(GrPoint point1, GrPoint point2, const GrColor& color, int thickness) {
    drawLine(point1, point2, color);
    int dx = point2.x - point1.x;
    int dy = point2.y - point1.y;
    bool isChangeX;
    GrPoint step(
        (dx > 0) ? 1 : -1,
        (dy > 0) ? 1 : -1
    );
    GrPoint change;
    if (abs(dx) < abs(dy)) {
        change.x = step.x;
        isChangeX = true;
    } else {
        change.y = step.y;
        isChangeX = false;
    } 
    for (int i = 1; i <= thickness / 2; i++) {
        drawLine(point1 + change * i, point2 + change * i, color);
    } 
    for (int i = 1; i <= (thickness - 1) / 2; i++) {
        drawLine(point1 - change * i, point2 - change * i, color);
    }
}

void drawLine(GrPoint point1, GrPoint point2, const GrColor& color) {
    int dx = point2.x - point1.x;
    int dy = point2.y - point1.y;
    int stepX = (dx > 0) ? 1 : -1;
    int stepY = (dy > 0) ? 1 : -1;

    int x = point1.x;
    int y = point1.y;
    int xerr = 0, yerr = 0;
    drawPixel(point1, color);
    while (x != point2.x || y != point2.y) {
        xerr += dx;
        yerr += dy;
        if (2 * abs(xerr) > abs(dy)) {
            x += stepX;
            xerr -= abs(dy) * stepX;
        }
        if (2 * abs(yerr) > abs(dx)) {
            y += stepY;
            yerr -= abs(dx) * stepY;
        }
        drawPixel(x, y, color);
    }
}

void drawRect(const GrRect& rect, const GrColor& color) {
    for (int x = rect.x; x < rect.x + rect.width; x++) {
        drawPixel(x, rect.y, color);
        drawPixel(x, rect.y + rect.height, color);
    }
    for (int y = rect.y; y < rect.y + rect.height; y++) {
        drawPixel(rect.x, y, color);
        drawPixel(rect.x + rect.width, y, color);
    }
}

void fillRect(int ix, int iy, int width, int height, const GrColor& color) {
    for (int y = iy; y < iy + height; y++) {
        for (int x = ix; x < ix + width; x++) {
            drawPixel(x, y, color);
        }
    }
}

void fillRect(const GrRect& rect, const GrColor& color) {
    fillRect(rect.x, rect.y, rect.width, rect.height, color);
}

void drawCircle(GrPoint center, int radius, const GrColor& color) {
    double step = 360. / (4 + (radius - 1) * 8);
    for (double angle = 1; angle < 360; angle += step) {
        int x = center.x + radius * std::cos(angle * 3.14 / 180);
        int y = center.y + radius * std::sin(angle * 3.14 / 180);
        drawPixel(x, y, color);
    }
}

void fillCircle(GrPoint center, int radius, const GrColor& color) {
    for (int y = center.y - radius; y < center.y + radius; y++) {
        for (int x = center.x - radius; x < center.x + radius; x++) {
            if (pow(center.x - x, 2) + pow(center.y - y, 2) <= pow(radius, 2)) {
                drawPixel(x, y, color);
            }
        }
    }     
} 

void draw1dBuffer(int ix, int iy, int width, int height, int* buffer) {
    int xLimit = std::min(std::abs((int)bitmapInfo.bmiHeader.biWidth), ix + width);
    int yLimit = std::min(std::abs((int)bitmapInfo.bmiHeader.biHeight), iy + height);
    
    for (int y = iy; y < yLimit; y++) {
        for (int x = ix; x < xLimit; x++) {
            int pixelOffset = y * bitmapInfo.bmiHeader.biWidth * 4 + x * 4;
            int pixelOffsetB = (y - iy) * width * 3 + (x - ix) * 3;
            displayBuffer[pixelOffset]     = buffer[pixelOffsetB + 2];
            displayBuffer[pixelOffset + 1] = buffer[pixelOffsetB + 1];
            displayBuffer[pixelOffset + 2] = buffer[pixelOffsetB];
        }
    }
}

void draw3dBuffer(int ix, int iy, int width, int height, int* buffer) {
    int xLimit = std::min(std::abs((int)bitmapInfo.bmiHeader.biWidth), ix + width);
    int yLimit = std::min(std::abs((int)bitmapInfo.bmiHeader.biHeight), iy + height);
    
    for (int y = iy; y < yLimit; y++) {
        for (int x = ix; x < xLimit; x++) {
            int pixelOffset = y * bitmapInfo.bmiHeader.biWidth * 4 + x * 4;
            int pixelOffsetB = (x - ix) * height * 3 + (y - iy) * 3;
            displayBuffer[pixelOffset]     = buffer[pixelOffsetB + 2];
            displayBuffer[pixelOffset + 1] = buffer[pixelOffsetB + 1];
            displayBuffer[pixelOffset + 2] = buffer[pixelOffsetB];
        }
    }
}

LRESULT CALLBACK windowProcedure(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
        case WM_CREATE: {
            deviceContextToDraw = GetDC(hWnd);
            //hBitmapToDraw = (HBITMAP)LoadImageA(hInstance, "test.bmp", IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
            //
            RECT rect; 
            GetClientRect(hWnd, &rect);
            clientWidth = rect.right - rect.left;
            clientHeight = rect.bottom - rect.top;
            //
            bitmapInfo.bmiHeader.biSize = sizeof(bitmapInfo.bmiHeader);
            bitmapInfo.bmiHeader.biWidth = clientWidth;
            // Negative biHeight makes top left as the coordinate system origin. Otherwise it's bottom left.
            bitmapInfo.bmiHeader.biHeight = -clientHeight;
            bitmapInfo.bmiHeader.biPlanes = 1;
            bitmapInfo.bmiHeader.biBitCount = 32;
            bitmapInfo.bmiHeader.biCompression = BI_RGB;
            //
            displayBuffer = new BYTE[clientWidth * clientHeight * 4];

            //save1dArrayToBmp("test5.bmp", displayBuffer, 100, 100);
            break;
        }
        //работает но непонятно как
        //case WM_PAINT:
        //{
        //    PAINTSTRUCT paintStruct;
        //    BITMAP bitmap;
        //
        //    HDC hdc = BeginPaint(hWnd, &paintStruct);
        //    HDC hdcMem = CreateCompatibleDC(hdc);
        //
        //    HGDIOBJ hBitmapOld = SelectObject(hdcMem, hBitmapToDraw);
        //
        //    GetObjectA(hBitmapToDraw, sizeof(bitmap), &bitmap);
        //    BitBlt(hdc, 100, 100, 100, 100, hdcMem, 200, 200, SRCCOPY);
        //
        //    SelectObject(hdcMem, hBitmapOld);
        //
        //    DeleteDC(hdcMem);
        //
        //    EndPaint(hWnd, &paintStruct);
        //    break;
        //}
        case WM_CLOSE:
            DestroyWindow(hWnd);
            break;
        case WM_DESTROY:
            DeleteObject(hBitmapToDraw);
            PostQuitMessage(0);
            return 0;
        
    }
    return DefWindowProcA(hWnd, uMsg, wParam, lParam);
}

//style = 0 for default flags
void createWindow(int x, int y, int width, int height, DWORD style) {
    
    hInstance = GetModuleHandleA(nullptr);

    WNDCLASSA wndClass = {};
    wndClass.lpszClassName = className;
    wndClass.hInstance = hInstance;
    wndClass.lpfnWndProc = windowProcedure;

    RegisterClassA(&wndClass);

    //https://learn.microsoft.com/en-us/windows/win32/winmsg/window-styles
    if (style == 0) {
        style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX  | WS_MAXIMIZEBOX;
    }
    
    hWnd = CreateWindowExA(0, className, "title", style, x, y, width, height, NULL, NULL, hInstance, NULL);
    
    //RECT rect = {0, 0, 1920, 1080};
    //HRGN hRegion = CreateRectRgnIndirect(&rect);
    //SetWindowRgn(hWnd, hRegion, FALSE);
    ShowWindow(hWnd, SW_SHOW);
}

void closeWindow() {
    UnregisterClassA(className, hInstance);
}

bool processMessages() {
    MSG msg = {};
    while (PeekMessageA(&msg, nullptr, 0u, 0u, PM_REMOVE)) {
        switch (msg.message)  {
            case WM_QUIT:
                return false;
        }
        TranslateMessage(&msg);
        DispatchMessageA(&msg);
    }
    return true;
}

void makeWindowTransparent(int r, int g, int b) {
    // Change window type to layered (https://stackoverflow.com/a/3970218/3357935)
    SetWindowLongA(hWnd, GWL_EXSTYLE, GetWindowLongA(hWnd, GWL_EXSTYLE) | WS_EX_LAYERED); //WS_EX_TRANSPARENT?
    SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
    SetLayeredWindowAttributes(hWnd, RGB(r, g, b), 0, LWA_COLORKEY);

    //SetWindowLongA(hWnd, GWL_STYLE, GetWindowLongA(hWnd, GWL_STYLE) & ~(WS_CAPTION | WS_THICKFRAME));
    //SetWindowLongA(hWnd, GWL_EXSTYLE, GetWindowLongA(hWnd, GWL_EXSTYLE) & ~(WS_EX_DLGMODALFRAME | WS_EX_WINDOWEDGE | WS_EX_CLIENTEDGE | WS_EX_STATICEDGE));
}


