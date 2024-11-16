#include "screenshot.h"
#include <iostream>
#include <windows.h>
bool mkScreenshot(std::wstring fname) {
    HDC hdcScreen = GetDC(NULL);
    if (!hdcScreen) {
        std::cerr << "Failed to get screen device context" << std::endl;
        return 1;
    }

    // 获取屏幕的宽高
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 创建与屏幕兼容的内存设备上下文
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    if (!hdcMem) {
        std::cerr << "Failed to create compatible DC" << std::endl;
        ReleaseDC(NULL, hdcScreen);
        return 1;
    }

    // 创建一个与屏幕兼容的位图
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    if (!hBitmap) {
        std::cerr << "Failed to create compatible bitmap" << std::endl;
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 1;
    }

    // 选择位图到内存DC中
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    // 将屏幕内容复制到内存DC中的位图
    BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);

    // 保存位图为BMP文件
    BITMAPFILEHEADER bfHeader;
    BITMAPINFOHEADER biHeader;
    DWORD dwWritten;

    // 填充文件头
    bfHeader.bfType = 0x4D42;  // 'BM'
    bfHeader.bfReserved1 = 0;
    bfHeader.bfReserved2 = 0;
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bfHeader.bfSize = bfHeader.bfOffBits + screenWidth * screenHeight * 3; // 24-bit color

    // 填充信息头
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biWidth = screenWidth;
    biHeader.biHeight = -screenHeight;  // 为了保存为倒立的图像
    biHeader.biPlanes = 1;
    biHeader.biBitCount = 24;  // 24-bit图像
    biHeader.biCompression = BI_RGB;
    biHeader.biSizeImage = 0;
    biHeader.biXPelsPerMeter = 0;
    biHeader.biYPelsPerMeter = 0;
    biHeader.biClrUsed = 0;
    biHeader.biClrImportant = 0;
    fname += L".bmp";
    // 创建文件
    HANDLE hFile = CreateFile(fname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create file" << std::endl;
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 1;
    }

    // 写入文件头和信息头
    WriteFile(hFile, &bfHeader, sizeof(bfHeader), &dwWritten, NULL);
    WriteFile(hFile, &biHeader, sizeof(biHeader), &dwWritten, NULL);

    // 写入位图数据
    BYTE* pPixels = new BYTE[screenWidth * screenHeight * 3];
    GetDIBits(hdcMem, hBitmap, 0, screenHeight, pPixels, (BITMAPINFO*)&biHeader, DIB_RGB_COLORS);
    WriteFile(hFile, pPixels, screenWidth * screenHeight * 3, &dwWritten, NULL);

    // 清理
    CloseHandle(hFile);
    delete[] pPixels;

    // 恢复状态并释放资源
    SelectObject(hdcMem, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);

    std::cout << "Screenshot saved as screenshot.bmp" << std::endl;
    return 0;
}