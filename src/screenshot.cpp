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

    // 获取屏幕的宽高
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    // 创建与屏幕兼容的内存设备上下文
    HDC hdcMem = CreateCompatibleDC(hdcScreen);
    if (!hdcMem) {
        Logger::get_instance()->error("Failed to create compatible DC");
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    // 创建一个与屏幕兼容的位图
    HBITMAP hBitmap = CreateCompatibleBitmap(hdcScreen, screenWidth, screenHeight);
    if (!hBitmap) {
        Logger::get_instance()->error("Failed to create compatible bitmap");
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    // 选择位图到内存DC中
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    // 将屏幕内容复制到内存DC中的位图
    if (!BitBlt(hdcMem, 0, 0, screenWidth, screenHeight, hdcScreen, 0, 0, SRCCOPY)) {
        Logger::get_instance()->error("Failed to capture screen content");
        SelectObject(hdcMem, hOldBitmap);
        DeleteObject(hBitmap);
        DeleteDC(hdcMem);
        ReleaseDC(NULL, hdcScreen);
        return "";
    }

    // 获取位图数据
    BITMAPINFOHEADER biHeader = {};
    biHeader.biSize = sizeof(BITMAPINFOHEADER);
    biHeader.biWidth = screenWidth;
    biHeader.biHeight = -screenHeight; // 为了保持图像正向
    biHeader.biPlanes = 1;
    biHeader.biBitCount = 24;  // 24-bit图像
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

    // 使用 libjpeg-turbo 压缩为 JPEG
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

    unsigned char* jpegBuffer = nullptr;  // JPEG 图像缓冲区
    unsigned long jpegSize = 0;           // JPEG 图像大小

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

    // 将 JPEG 数据进行 Base64 编码
    std::string base64Encoded = base64Encode(jpegBuffer, jpegSize);

    // 清理
    tjFree(jpegBuffer);
    tjDestroy(jpegCompressor);
    delete[] pPixels;
    SelectObject(hdcMem, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(NULL, hdcScreen);

    Logger::get_instance()->info("Screenshot captured successfully");

    return base64Encoded; // 返回 Base64 编码后的字符串
}