#include "screenshot.h"
#include <iostream>
#include <windows.h>
bool mkScreenshot(std::wstring fname) {
    HDC hdcScreen = GetDC(NULL);
    if (!hdcScreen) {
        std::cerr << "Failed to get screen device context" << std::endl;
        return 1;
    }

    // ��ȡ��Ļ�Ŀ��
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // ��������Ļ���ݵ��ڴ��豸������
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    if (!hdcMem) {
        std::cerr << "Failed to create compatible DC" << std::endl;
        ReleaseDC(NULL, hdcScreen);
        return 1;
    }

    // ����һ������Ļ���ݵ�λͼ
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    if (!hBitmap) {
        std::cerr << "Failed to create compatible bitmap" << std::endl;
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 1;
    }

    // ѡ��λͼ���ڴ�DC��
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    // ����Ļ���ݸ��Ƶ��ڴ�DC�е�λͼ
    BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY);

    // ����λͼΪBMP�ļ�
    BITMAPFILEHEADER bfHeader;
    BITMAPINFOHEADER biHeader;
    DWORD dwWritten;

    // ����ļ�ͷ
    bfHeader.bfType = 0x4D42;  // 'BM'
    bfHeader.bfReserved1 = 0;
    bfHeader.bfReserved2 = 0;
    bfHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
    bfHeader.bfSize = bfHeader.bfOffBits + screenWidth * screenHeight * 3; // 24-bit color

    // �����Ϣͷ
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biWidth = screenWidth;
    biHeader.biHeight = -screenHeight;  // Ϊ�˱���Ϊ������ͼ��
    biHeader.biPlanes = 1;
    biHeader.biBitCount = 24;  // 24-bitͼ��
    biHeader.biCompression = BI_RGB;
    biHeader.biSizeImage = 0;
    biHeader.biXPelsPerMeter = 0;
    biHeader.biYPelsPerMeter = 0;
    biHeader.biClrUsed = 0;
    biHeader.biClrImportant = 0;
    fname += L".bmp";
    // �����ļ�
    HANDLE hFile = CreateFile(fname.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hFile == INVALID_HANDLE_VALUE) {
        std::cerr << "Failed to create file" << std::endl;
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return 1;
    }

    // д���ļ�ͷ����Ϣͷ
    WriteFile(hFile, &bfHeader, sizeof(bfHeader), &dwWritten, NULL);
    WriteFile(hFile, &biHeader, sizeof(biHeader), &dwWritten, NULL);

    // д��λͼ����
    BYTE* pPixels = new BYTE[screenWidth * screenHeight * 3];
    GetDIBits(hdcMem, hBitmap, 0, screenHeight, pPixels, (BITMAPINFO*)&biHeader, DIB_RGB_COLORS);
    WriteFile(hFile, pPixels, screenWidth * screenHeight * 3, &dwWritten, NULL);

    // ����
    CloseHandle(hFile);
    delete[] pPixels;

    // �ָ�״̬���ͷ���Դ
    SelectObject(hdcMem, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);

    std::cout << "Screenshot saved as screenshot.bmp" << std::endl;
    return 0;
}