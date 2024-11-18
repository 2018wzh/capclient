#include "screenshot.h"
#include "uuidxx.h"
#include <iostream>
#include <windows.h>
#include <turbojpeg.h>
#include "utils.h"
std::string mkScreenshot() {
    // ��ȡϵͳ��ʱĿ¼
    char tempPath[MAX_PATH];
    if (!GetTempPathA(MAX_PATH, tempPath)) {
        std::cerr << "Failed to get temp path" << std::endl;
        return ""; // ���ؿ��ַ�����ʾʧ��
    }

    // ����UUID���Ƴ�������
    uuidxx::uuid id = uuidxx::uuid::Generate();
    std::string idstr = id.ToString();
    idstr.erase(std::remove(idstr.begin(), idstr.end(), '{'), idstr.end());
    idstr.erase(std::remove(idstr.begin(), idstr.end(), '}'), idstr.end());
    std::string fname = std::string(tempPath) + idstr + ".jpg";

    HDC hdcScreen = GetDC(NULL);
    if (!hdcScreen) {
        std::cerr << "Failed to get screen device context" << std::endl;
        return "";
    }

    // ��ȡ��Ļ�Ŀ��
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // ��������Ļ���ݵ��ڴ��豸������
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    if (!hdcMem) {
        std::cerr << "Failed to create compatible DC" << std::endl;
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    // ����һ������Ļ���ݵ�λͼ
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    if (!hBitmap) {
        std::cerr << "Failed to create compatible bitmap" << std::endl;
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    // ѡ��λͼ���ڴ�DC��
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    // ����Ļ���ݸ��Ƶ��ڴ�DC�е�λͼ
    if (!BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY)) {
        std::cerr << "Failed to capture screen content" << std::endl;
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
        std::cerr << "Failed to retrieve bitmap data" << std::endl;
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
        std::cerr << "Failed to initialize libjpeg-turbo compressor" << std::endl;
        delete[] pPixels;
        SelectObject(hdcMem, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    unsigned char* jpegBuffer = nullptr;  // JPEG ͼ�񻺳���
    unsigned long jpegSize = 0;           // JPEG ͼ���С

    if (tjCompress2(jpegCompressor, pPixels, screenWidth, 0, screenHeight, TJPF_RGB, &jpegBuffer, &jpegSize, TJSAMP_444, jpegQuality, TJFLAG_FASTDCT) != 0) {
        std::cerr << "Failed to compress image: " << tjGetErrorStr() << std::endl;
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

    return base64Encoded; // ���� Base64 �������ַ���
}