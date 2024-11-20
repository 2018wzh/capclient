#include "screenshot.h"
#include "uuidxx.h"
#include <iostream>
#include <windows.h>
#include <turbojpeg.h>
#include "utils.h"
#include "logger.h"
std::string mkScreenshot() {
    
    HDC hdcScreen = GetDC(NULL);
    if (!hdcScreen) {
        Logger::get_instance()->error("Failed to get screen device context");
        return "";
    }

    // ��ȡ��Ļ�Ŀ��
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // ��������Ļ���ݵ��ڴ��豸������
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    if (!hdcMem) {
        Logger::get_instance()->error("Failed to create compatible DC");
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    // ����һ������Ļ���ݵ�λͼ
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    if (!hBitmap) {
        Logger::get_instance()->error("Failed to create compatible bitmap");
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    // ѡ��λͼ���ڴ�DC��
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    // ����Ļ���ݸ��Ƶ��ڴ�DC�е�λͼ
    if (!BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY)) {
        Logger::get_instance()->error("Failed to capture screen content");
        SelectObject(hdcMem, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    // ��ȡλͼ����
    BITMAPINFOHEADER biHeader = {};
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biWidth = screenWidth;
    biHeader.biHeight = -screenHeight; // Ϊ�˱���ͼ������
    biHeader.biPlanes = 1;
    biHeader.biBitCount = 24;  // 24-bitͼ��
    biHeader.biCompression = BI_RGB;

    BYTE* pPixels = new BYTE[screenWidth * screenHeight * 3];
    if (!GetDIBits(hdcMem, hBitmap, 0, screenHeight, pPixels, (BITMAPINFO*)&biHeader, DIB_RGB_COLORS)) {
        Logger::get_instance()->error("Failed to retrieve bitmap data");
        delete[] pPixels;
        SelectObject(hdcMem, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    // ʹ�� libjpeg-turbo ѹ��Ϊ JPEG
    tjhandle jpegCompressor = tjInitCompress();
    if (!jpegCompressor) {
        Logger::get_instance()->error("Failed to initialize libjpeg-turbo compressor");
        delete[] pPixels;
        SelectObject(hdcMem, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    unsigned char* jpegBuffer = nullptr;  // JPEG ͼ�񻺳���
    unsigned long jpegSize = 0;           // JPEG ͼ���С

    if (tjCompress2(jpegCompressor, pPixels, screenWidth, 0, screenHeight, TJPF_RGB, &jpegBuffer, &jpegSize, TJSAMP_444, 90, TJFLAG_FASTDCT) != 0) {
        Logger::get_instance()->error("Failed to compress image: {}", tjGetErrorStr());
        tjDestroy(jpegCompressor);
        delete[] pPixels;
        SelectObject(hdcMem, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    // �� JPEG ���ݽ��� Base64 ����
    std::string base64Encoded = base64Encode(jpegBuffer, jpegSize);

    // ����
    tjFree(jpegBuffer);
    tjDestroy(jpegCompressor);
    delete[] pPixels;
    SelectObject(hdcMem, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);

    Logger::get_instance()->info("Screenshot captured successfully");

    return base64Encoded; // ���� Base64 �������ַ���
}