#include "bmpProcessing.h"

void save1dArrayToBMP(const char* path, int* buffer, int width, int height)
{
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + width * height * 4;

    unsigned char fileHeader[fileHeaderSize];

    //File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // File size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    //Reserved 1 (not used)
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    //Reserved 2 (not used)
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    //Pixel data offset
    fileHeader[10] = fileHeaderSize + informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader[informationHeaderSize];

    //Header size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    //Image width
    informationHeader[4] = width;
    informationHeader[5] = width >> 8;
    informationHeader[6] = width >> 16;
    informationHeader[7] = width >> 24;
    //Image height
    informationHeader[8] =  height;
    informationHeader[9] =  height >> 8;
    informationHeader[10] = height >> 16;
    informationHeader[11] = height >> 24;
    //Planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    //Bits per pixels (RGB)
    informationHeader[14] = 32;
    informationHeader[15] = 0;
    //Compression (no compression)
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    //Image size (no compression)
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    //X pixels per meter (not specified)
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    //Y pixels per meter (not specified)
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    //Total colors (colors palette not used)
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    //Important colors (Generally ignored)
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    file.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    file.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);
    //file.write(reinterpret_cast<char*>(buffer), width * height * 4);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char r = static_cast<unsigned char>(buffer[(height - y - 1) * width * 3 + x * 3 + 0]);
            unsigned char g = static_cast<unsigned char>(buffer[(height - y - 1) * width * 3 + x * 3 + 1]);
            unsigned char b = static_cast<unsigned char>(buffer[(height - y - 1) * width * 3 + x * 3 + 2]);
    
            unsigned char color[] = {b, g, r, 255};

    
            file.write(reinterpret_cast<char*>(color), 4);
        }
    }

    file.close();

    std::cout << "File created\n";
}

void save1dArrayToBMPNoAlpha(const char* path, int* buffer, int width, int height)
{
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);

    unsigned char bmpPadding[3] = {0, 0, 0};
    const int paddingAmount = (4 - (width * 3) % 4) % 4;

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + width * height * 3;

    unsigned char fileHeader[fileHeaderSize];

    //File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // File size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    //Reserved 1 (not used)
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    //Reserved 2 (not used)
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    //Pixel data offset
    fileHeader[10] = fileHeaderSize + informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader[informationHeaderSize];

    //Header size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    //Image width
    informationHeader[4] = width;
    informationHeader[5] = width >> 8;
    informationHeader[6] = width >> 16;
    informationHeader[7] = width >> 24;
    //Image height
    informationHeader[8] =  height;
    informationHeader[9] =  height >> 8;
    informationHeader[10] = height >> 16;
    informationHeader[11] = height >> 24;
    //Planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    //Bits per pixels (RGB)
    informationHeader[14] = 24;
    informationHeader[15] = 0;
    //Compression (no compression)
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    //Image size (no compression)
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    //X pixels per meter (not specified)
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    //Y pixels per meter (not specified)
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    //Total colors (colors palette not used)
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    //Important colors (Generally ignored)
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    file.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    file.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);
    //file.write(reinterpret_cast<char*>(buffer), width * height * 4);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char r = static_cast<unsigned char>(buffer[(height - y - 1) * width * 3 + x * 3 + 0]);
            unsigned char g = static_cast<unsigned char>(buffer[(height - y - 1) * width * 3 + x * 3 + 1]);
            unsigned char b = static_cast<unsigned char>(buffer[(height - y - 1) * width * 3 + x * 3 + 2]);
    
            unsigned char color[] = {b, g, r};

    
            file.write(reinterpret_cast<char*>(color), 3);
        }
        file.write(reinterpret_cast<char*>(bmpPadding), paddingAmount);
    }

    file.close();

    std::cout << "File created\n";
}

void save3dArrayToBMP(const char* path, int* buffer, int width, int height)
{
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize +width * height * 4;

    unsigned char fileHeader[fileHeaderSize];

    //File type
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';
    // File size
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;
    //Reserved 1 (not used)
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    //Reserved 2 (not used)
    fileHeader[8] = 0;
    fileHeader[9] = 0;
    //Pixel data offset
    fileHeader[10] = fileHeaderSize + informationHeaderSize;
    fileHeader[11] = 0;
    fileHeader[12] = 0;
    fileHeader[13] = 0;

    unsigned char informationHeader[informationHeaderSize];

    //Header size
    informationHeader[0] = informationHeaderSize;
    informationHeader[1] = 0;
    informationHeader[2] = 0;
    informationHeader[3] = 0;
    //Image width
    informationHeader[4] = width;
    informationHeader[5] = width >> 8;
    informationHeader[6] = width >> 16;
    informationHeader[7] = width >> 24;
    //Image height
    informationHeader[8] =  height;
    informationHeader[9] =  height >> 8;
    informationHeader[10] = height >> 16;
    informationHeader[11] = height >> 24;
    //Planes
    informationHeader[12] = 1;
    informationHeader[13] = 0;
    //Bits per pixels (RGB)
    informationHeader[14] = 32;
    informationHeader[15] = 0;
    //Compression (no compression)
    informationHeader[16] = 0;
    informationHeader[17] = 0;
    informationHeader[18] = 0;
    informationHeader[19] = 0;
    //Image size (no compression)
    informationHeader[20] = 0;
    informationHeader[21] = 0;
    informationHeader[22] = 0;
    informationHeader[23] = 0;
    //X pixels per meter (not specified)
    informationHeader[24] = 0;
    informationHeader[25] = 0;
    informationHeader[26] = 0;
    informationHeader[27] = 0;
    //Y pixels per meter (not specified)
    informationHeader[28] = 0;
    informationHeader[29] = 0;
    informationHeader[30] = 0;
    informationHeader[31] = 0;
    //Total colors (colors palette not used)
    informationHeader[32] = 0;
    informationHeader[33] = 0;
    informationHeader[34] = 0;
    informationHeader[35] = 0;
    //Important colors (Generally ignored)
    informationHeader[36] = 0;
    informationHeader[37] = 0;
    informationHeader[38] = 0;
    informationHeader[39] = 0;

    file.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
    file.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            unsigned char r = static_cast<unsigned char>(buffer[x * height * 3 + (height - y - 1) * 3 + 0]);
            unsigned char g = static_cast<unsigned char>(buffer[x * height * 3 + (height - y - 1) * 3 + 1]);
            unsigned char b = static_cast<unsigned char>(buffer[x * height * 3 + (height - y - 1) * 3 + 2]);
    
            unsigned char color[] = {b, g, r, 255};
    
            file.write(reinterpret_cast<char*>(color), 4);
        }
    }
    file.close();
    std::cout << "File created\n";
}

void readBMPTo1dArray(const char* path, int* outputBuffer, int width, int height)
{
    std::ifstream imageFile(path, std::ios::binary);
    if (!imageFile.is_open()) 
    {
        std::cout << "No file to open\n";
        return;
    }
    imageFile.seekg(0, std::ios::end);
    std::streampos length = imageFile.tellg();
    imageFile.seekg(0, std::ios::beg);
    char* buffer = new char[length];
    imageFile.read(&buffer[0], length);
    imageFile.close();

    //std::cout << "width: " << (int)buffer[18] << "\n";
    //std::cout << "width: " << (int)buffer[19] << "\n";
    //std::cout << "width: " << (int)buffer[20] << "\n";
    //std::cout << "width: " << (int)buffer[21] << "\n";

    for (int j = 0; j < 54; j++)
    {
        std::cout << (int)buffer[j] << " ";
    }
    std::cout << std::endl;
    std::cout << "Info is read" << std::endl;

    int i = 54;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            outputBuffer[(height - y - 1) * width * 3 + x * 3 + 0] = (int)buffer[i + 2];
            outputBuffer[(height - y - 1) * width * 3 + x * 3 + 1] = (int)buffer[i + 1];
            outputBuffer[(height - y - 1) * width * 3 + x * 3 + 2] = (int)buffer[i + 0];
            std::cout << (int)buffer[i + 2] << " " << (int)buffer[i + 1] << " " << (int)buffer[i + 0] << " ";
            i += 3;
        }
        std::cout << std::endl;
    }
    std::cout << "File is read" << std::endl;
}

void readBMPTo3dArray(const char* path, int* outputBuffer, int width, int height)
{
    std::ifstream imageFile(path, std::ios::binary);
    if (!imageFile.is_open())
    {
        std::cout << "No file to open\n";
        return;
    }
    imageFile.seekg(0, std::ios::end);
    std::streampos length = imageFile.tellg();
    imageFile.seekg(0, std::ios::beg);
    char* buffer = new char[length];
    imageFile.read(&buffer[0], length);
    imageFile.close();

    int i = 54;
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            outputBuffer[x * height * 3 + (height - y - 1) * 3 + 0] = (int)buffer[i + 2];
            outputBuffer[x * height * 3 + (height - y - 1) * 3 + 1] = (int)buffer[i + 1];
            outputBuffer[x * height * 3 + (height - y - 1) * 3 + 2] = (int)buffer[i + 0];
            
            //outputBuffer[x * height * 3 + (height - y - 1) * 3 + 0] = (int)buffer[i + 2];
            //outputBuffer[x * height * 3 + (height - y - 1) * 3 + 1] = (int)buffer[i + 1];
            //outputBuffer[x * height * 3 + (height - y - 1) * 3 + 2] = (int)buffer[i + 0];
            i += 3;
        }
    }
}