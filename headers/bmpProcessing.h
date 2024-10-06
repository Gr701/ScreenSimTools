#pragma once

#include <fstream>
#include <iostream>

void save1dArrayToBMP(const char* path, int* buffer, int width, int height);

void save1dArrayToBMPNoAlpha(const char* path, int* buffer, int width, int height);

void save3dArrayToBMP(const char* path, int* buffer, int width, int height); // (int*)array, when sending as an argument

void readBMPTo1dArray(const char* path, int* buffer, int width, int height);

void readBMPTo3dArray(const char* path, int* buffer, int width, int height);