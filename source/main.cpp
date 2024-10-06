#include <iostream>
#include <windows.h>
#include <time.h>
#include <math.h>

#include "screenCapture.h"
#include "inputSimulation.h"
#include "imageDisplay.h"
#include "bmpProcessing.h"


//DIFFERENT LIBRARY TESTS

int inverseImage[1920][1080][3];
int main()
{
    //TEST for -1 in bmp
    int* button = new int [134 * 35 * 3];
    readBMPTo1dArray("SinglePlayerButton.bmp", button, 134, 35);
    std::cout << "main read:" << std::endl;
    for (int i = 0; i < 134 * 35 * 3; i++) {
        std::cout << button[i] << " ";
    }
    return 0;

    //TEST for delay in screen capture 
    setupScreenCapture();
    captureScreen();
    saveScreenToBMP("testmove1.bmp");
    leftClick();
    leftClick();
    moveMouseRelatively(100, 0);
    captureScreen();
    saveScreenToBMP("testmove2.bmp");
    leftClick();
    leftClick();
    moveMouseRelatively(100, 0);
    captureScreen();
    saveScreenToBMP("testmove3.bmp");
    leftClick();
    leftClick();
    moveMouseRelatively(100, 0);
    captureScreen();
    saveScreenToBMP("testmove4.bmp");
    return 0;

    setupScreenCapture();
    captureScreen();
    saveScreenToBMP("testmove1.bmp");
    return 0;
    //trransparent window
    int* hellicopterImage = new int[256*100*3];
    readBMPTo1dArray("helicopter.bmp", hellicopterImage, 256, 100);
    createWindow(0, 0, 1920, 1080, 0);
    //makeWindowTransparent(255, 0, 255);
    int xx = 0;
    while (true)
    {
        if (!processMessages())
        {
            return 0;
        }
        clearRender(GrColor(255, 0, 255));
        draw1dBuffer(xx, xx, 256, 100, hellicopterImage);
        xx++;
        renderImage();
        Sleep(1000/60);
    }
    return 0;

    

    //read and write arrays
    
    clock_t startTime, endTime;

    std::cout << "start program" << std::endl;

    //int* buffer = new int[1920 * 1080 * 3];
    int* buffer = new int[340 * 22 * 3];

    readBMPTo1dArray("Fight1.bmp", buffer, 340, 22);
    //for (int i = 0; i < 8 * 13 * 3; i++)
    //{
    //    //std::cout << (buffer[i] + 256) % 256 << " ";
    //}
    //readBMPTo1dArray("2.bmp", buffer, 8, 13);
    //int y4 = 4;
    //int x4 = 3;
    //std::cout << buffer[8 * y4 * 3 + x4 * 3 + 0] << " - r" << std::endl;
    //std::cout << buffer[8 * y4 * 3 + x4 * 3 + 1] << " - g" << std::endl;
    //std::cout << buffer[8 * y4 * 3 + x4 * 3 + 2] << " - b" << std::endl;

    save1dArrayToBMPNoAlpha("2.bmp", buffer, 340, 22);

    return 0;

    bool a = false; //clicker fortnite
    while (true)
    {
        if (a)
        {
            clickKey('E');
            Sleep(10);
            LMBDown();
            Sleep(10);
            LMBUp();
        }
        if(GetAsyncKeyState('G'))
        {
            std::cout << "g";
            a = !a;
            Sleep(500);
        }
        Sleep(1000/120);
    }
    return 0;

    
    //readBMPTo1dArray("fortest.bmp", buffer, 1920, 1080);
    //for (int y = 0; y < 1080; y++)
    //{
    //    for (int x = 0; x < 1920; x++)
    //    {
    //        if (x == 100)
    //        {
    //            buffer[y * 1920 * 3 + x * 3 + 0] = 255;
    //            buffer[y * 1920 * 3 + x * 3 + 1] = 255;
    //            buffer[y * 1920 * 3 + x * 3 + 2] = 255;
    //        }
    //        else if (y == 100)
    //        {
    //            buffer[y * 1920 * 3 + x * 3 + 0] = 255;
    //            buffer[y * 1920 * 3 + x * 3 + 1] = 0;
    //            buffer[y * 1920 * 3 + x * 3 + 2] = 0;
    //        }
    //    }
    //}
    //save1dArrayToBMP("screenCopy2.bmp", buffer, 1920, 1080);

    //readBMPTo3dArray("map.bmp", (int*)inverseImage, 1920, 1080);
    //for (int y = 0; y < 1080; y++)
    //{
    //    for (int x = 0; x < 1920; x++)
    //    {
    //        if (x == 100)
    //        {
    //            inverseImage[x][y][0] = 255;
    //            inverseImage[x][y][1] = 255;
    //            inverseImage[x][y][2] = 255;
    //        }
    //        else if (y == 100)
    //        {
    //            inverseImage[x][y][0] = 255;
    //            inverseImage[x][y][1] = 0;
    //            inverseImage[x][y][2] = 0;
    //        }
    //    }
    //}
    //save3dArrayToBMP("srceenCopy.bmp", (int*)inverseImage, 1920, 1080);

    //

    
    setupScreenCapture();
    captureScreen();
    saveScreenToBMP("screenFortest.bmp");
    saveScreenToBMPNoAlpha("screenFortestNoalpha.bmp");
    readBMPTo1dArray("screenFortest.bmp", buffer, 1920, 1080);
    save1dArrayToBMP("alpha.bmp", buffer, 1920, 1080);
    save1dArrayToBMPNoAlpha("noalpha.bmp", buffer, 1920, 1080);
    return 0; 
    createWindow(0, 0, 1920, 1080, 0);
    int x = 0;
    while (true)
    {
        startTime = clock();
        if (!processMessages())
        {
            return 0;
        }
        //render
        clearRender(GrColor(0, 0, 0));
        
        draw1dBuffer(0, 0, 1920, 1080, buffer);
        //draw3dBuffer(0, 0, 1920, 1080, (int*)inverseImage);
        fillRect(x, 10, 50, 50, GrColor(255, 255, 255));
        x++;
        renderImage();
        //
        endTime = clock();
        Sleep(1000/60);
        std::cout << "time - " << (double)(endTime - startTime)/CLOCKS_PER_SEC << " s\n";
    }

    return 0;
}

