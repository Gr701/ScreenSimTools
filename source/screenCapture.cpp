#include "screenCapture.h"

//VARIABLES used in both setupScreenCapture and captureScreen 
IDXGIOutputDuplication *outputDuplication;
HRESULT hr;
DXGI_OUTDUPL_FRAME_INFO frameInfo;
IDXGIResource *resource = NULL;
ID3D11Texture2D *acquiredDesktopImage;
ID3D11DeviceContext *deviceContext;
ID3D11Texture2D *texture; 
D3D11_MAPPED_SUBRESOURCE mappedSubresource;
//std::unique_ptr<BYTE> buffer;
BYTE* buffer;
D3D11_TEXTURE2D_DESC desc;
UINT bmpRowPitch;
DXGI_OUTDUPL_DESC outDuplDesc;
UINT rowPitch;
UCHAR* imageBuffer;
long captureSize;

BYTE* sptr;
BYTE* dptr;
//
#ifndef GRCOLOR_IMPLEMENTED
#define GRCOLOR_IMPLEMENTED
GrColor::GrColor() {}
GrColor::GrColor(int r, int g, int b) : r(r), g(g), b(b) {}
void GrColor::print() { std::cout << "r - " << r << " g - " << g << " b - " << b << std::endl; }
#endif

int checkHrCode(HRESULT hr, const char* processName)
{
    if (FAILED(hr))
    {
        std::cerr << std::hex << processName << " - 0x" << hr << std::endl;
    }
    //else
    //{
    //    std::cerr << std::hex << processName << " - 0x" << hr << std::endl;
    //}
    return 0;
}

void setupScreenCapture()
{
    //preparation 

    IDXGIFactory1 *factory;
    hr = CreateDXGIFactory1(__uuidof(IDXGIFactory1), (void**)&factory);
    checkHrCode(hr, "factory");
    
    IDXGIAdapter1 *adapter; //videocard
    hr = factory->EnumAdapters1(0, &adapter);
    checkHrCode(hr, "adapter");

    IDXGIOutput *output;
    hr = adapter->EnumOutputs(0, &output);
    checkHrCode(hr, "output");

    IDXGIOutput1 *output1;
    hr = output->QueryInterface(&output1);
    checkHrCode(hr, "output1");

    ID3D11Device *device;
    D3D_FEATURE_LEVEL featureLevel;
    hr = D3D11CreateDevice(adapter, D3D_DRIVER_TYPE_UNKNOWN, NULL, 0, NULL, 0, D3D11_SDK_VERSION, &device, &featureLevel, NULL);
    checkHrCode(hr, "device");
    //std::cout << "feature level - " << featureLevel << std::endl;
    //device->CheckFeatureSupport(D3D11_FEATURE_O)

    //duplication
    hr = output1->DuplicateOutput(device, &outputDuplication);
    checkHrCode(hr, "duplication");

    while (true)
    {
        hr = outputDuplication->AcquireNextFrame(INFINITE, &frameInfo, &resource);
        if (FAILED(hr))
        {
            std::cerr << "acquire next frame - " << std::hex << "0x" << hr << std::endl;
        }
        if (frameInfo.LastPresentTime.QuadPart)
        {
            //std::cout << "next frame acquired" << std::endl;
            break;
        }
        outputDuplication->ReleaseFrame();
    }
    
    
    //hr = resource->QueryInterface(__uuidof(ID3D11Texture2D), reinterpret_cast<void **>(&acquiredDesktopImage));
    hr = resource->QueryInterface(IID_PPV_ARGS(&acquiredDesktopImage));
    checkHrCode(hr, "acquiredImage");

    //copying to cpu

    //////////?
    acquiredDesktopImage->GetDesc(&desc);

    D3D11_TEXTURE2D_DESC textureDescription;
    RtlZeroMemory(&textureDescription, sizeof(D3D11_TEXTURE2D_DESC));
    textureDescription.Width = desc.Width;
    textureDescription.Height = desc.Height;
    textureDescription.MipLevels = desc.MipLevels;
    textureDescription.ArraySize = desc.ArraySize;
    textureDescription.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
    textureDescription.SampleDesc = desc.SampleDesc;
    textureDescription.Usage = D3D11_USAGE_STAGING;
    textureDescription.BindFlags = 0;
    textureDescription.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
    textureDescription.MiscFlags = 0;

    
    hr = device->CreateTexture2D(&textureDescription, nullptr, &texture);
    checkHrCode(hr, "create texture");

    
    device->GetImmediateContext(&deviceContext);
    deviceContext->CopyResource(texture, acquiredDesktopImage);

    
    hr = deviceContext->Map(texture, 0, D3D11_MAP_READ, 0, &mappedSubresource);
    checkHrCode(hr, "map");

    //getting buffer

    buffer = new BYTE[mappedSubresource.RowPitch * desc.Height];

    
    outputDuplication->GetDesc(&outDuplDesc);

    bmpRowPitch = outDuplDesc.ModeDesc.Width * 4;
    sptr = reinterpret_cast<BYTE*>(mappedSubresource.pData);
    dptr = buffer + mappedSubresource.RowPitch * desc.Height - bmpRowPitch;
    rowPitch = std::min<UINT>(bmpRowPitch, mappedSubresource.RowPitch);

    for (size_t h = 0; h < outDuplDesc.ModeDesc.Height; h++)
    {
        memcpy_s(dptr, bmpRowPitch, sptr, rowPitch);
        sptr += mappedSubresource.RowPitch;
        dptr -= bmpRowPitch;
    }
    
    captureSize = rowPitch * desc.Height;
    imageBuffer = new UCHAR[captureSize];
    //imageBuffer = (UCHAR*)malloc(captureSize);
    memcpy(imageBuffer, buffer, captureSize);

    //cleaning memory
    factory->Release();
    adapter->Release();
    output->Release();
    output1->Release();
    device->Release();

    // OMG ITS OVER (Seems like that)
}

void captureScreen()
{
    clock_t startTime = clock();
        
    while (true)
    {
        outputDuplication->ReleaseFrame();
        hr = outputDuplication->AcquireNextFrame(INFINITE, &frameInfo, &resource);
        if (FAILED(hr))
        {
            std::cerr << std::hex << "0x" << hr << std::endl;
            if (hr == 0x887a0026 || hr == 0x887a0001)
            {
                std::cout << "restarting screenCapture" << std::endl;
                Sleep(5000);
                stopScreenCapture();
                setupScreenCapture();
            }

        }
        if (frameInfo.LastPresentTime.QuadPart)
        {
            //std::cout << "frame captured successfully" << std::endl;
            break;
        }
        outputDuplication->ReleaseFrame();
    }

    resource->QueryInterface(IID_PPV_ARGS(&acquiredDesktopImage));

    deviceContext->CopyResource(texture, acquiredDesktopImage);

    deviceContext->Map(texture, 0, D3D11_MAP_READ, 0, &mappedSubresource);

    sptr = reinterpret_cast<BYTE*>(mappedSubresource.pData);
    dptr = buffer + mappedSubresource.RowPitch * desc.Height - bmpRowPitch;

    for (size_t h = 0; h < outDuplDesc.ModeDesc.Height; h++)
    {
        memcpy_s(dptr, bmpRowPitch, sptr, rowPitch);
        sptr += mappedSubresource.RowPitch;
        dptr -= bmpRowPitch;
    }

    memcpy(imageBuffer, buffer, captureSize);

    clock_t endTime = clock();
    //std::cout << "time = " << (double)(endTime - startTime)/CLOCKS_PER_SEC << std::endl;
}

void stopScreenCapture()
{
    outputDuplication->Release();
    resource->Release();
    acquiredDesktopImage->Release();
    deviceContext->Release();
    texture->Release();
    delete[] buffer;
    delete[] imageBuffer;
}

int getColorHEX(int x, int y)
{
    return (int)imageBuffer[(desc.Height - y - 1) * desc.Width * 4 + x * 4] * 256*256 + (int)imageBuffer[(desc.Height - y - 1) * desc.Width * 4 + x * 4 + 1] * 256 + (int)imageBuffer[(desc.Height - y - 1) * desc.Width * 4 + x * 4 + 2];
}

GrColor getColorRGB(int x, int y)
{
    return GrColor
    (
        (int)imageBuffer[(desc.Height - y - 1) * desc.Width * 4 + x * 4 + 2], 
        (int)imageBuffer[(desc.Height - y - 1) * desc.Width * 4 + x * 4 + 1],
        (int)imageBuffer[(desc.Height - y - 1) * desc.Width * 4 + x * 4 + 0]
    );
}

void saveScreenToBMP(const char* path)
{
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + desc.Width * desc.Height * 4;

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
    informationHeader[4] = desc.Width;
    informationHeader[5] = desc.Width >> 8;
    informationHeader[6] = desc.Width >> 16;
    informationHeader[7] = desc.Width >> 24;
    //Image height
    informationHeader[8] = desc.Height;
    informationHeader[9] = desc.Height >> 8;
    informationHeader[10] = desc.Height >> 16;
    informationHeader[11] = desc.Height >> 24;
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
    file.write(reinterpret_cast<char*>(imageBuffer), desc.Width * desc.Height * 4);

    file.close();

    std::cout << "File created\n";
}

void saveScreenToBMPNoAlpha(const char* path)
{
    std::ofstream file;
    file.open(path, std::ios::out | std::ios::binary);

    unsigned char bmpPadding[3] = {0, 0, 0};
    const int paddingAmount = (4 - (desc.Width * 3) % 4) % 4;

    const int fileHeaderSize = 14;
    const int informationHeaderSize = 40;
    const int fileSize = fileHeaderSize + informationHeaderSize + desc.Width * desc.Height * 3;

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
    informationHeader[4] = desc.Width;
    informationHeader[5] = desc.Width >> 8;
    informationHeader[6] = desc.Width >> 16;
    informationHeader[7] = desc.Width >> 24;
    //Image height
    informationHeader[8] = desc.Height;
    informationHeader[9] = desc.Height >> 8;
    informationHeader[10] = desc.Height >> 16;
    informationHeader[11] = desc.Height >> 24;
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

    for (int y = 0; y < desc.Height; y++)
    {
        for (int x = 0; x < desc.Width; x++)
        {
            unsigned char r = static_cast<unsigned char>((int)imageBuffer[y * desc.Width * 4 + x * 4 + 2] % 256);
            unsigned char g = static_cast<unsigned char>((int)imageBuffer[y * desc.Width * 4 + x * 4 + 1] % 256);
            unsigned char b = static_cast<unsigned char>((int)imageBuffer[y * desc.Width * 4 + x * 4 + 0] % 256);
    
            unsigned char color[] = {b, g, r};

            file.write(reinterpret_cast<char*>(color), 3);
        }
        file.write(reinterpret_cast<char*>(bmpPadding), paddingAmount);
    }

    file.close();

    std::cout << "File created\n";
}