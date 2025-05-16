#include <windows.h>
#include <gdiplus.h>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <vector>
#include <algorithm>

#pragma comment(lib, "Gdiplus.lib")
#pragma comment(lib, "Msimg32.lib")

using namespace Gdiplus;

// ==== GDI+ INIT / CLEANUP ====

void InitGDIPlus(ULONG_PTR &gdiplusToken) {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

void ShutdownGDIPlus(ULONG_PTR &gdiplusToken) {
    GdiplusShutdown(gdiplusToken);
}

// ==== EFFECTS ====

void ApplyTint(HDC hdc, int width, int height, Color tintColor) {
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP bmp = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBmp = (HBITMAP)SelectObject(memDC, bmp);

    // Fill the memory DC with the tint color
    HBRUSH brush = CreateSolidBrush(RGB(tintColor.GetRed(), tintColor.GetGreen(), tintColor.GetBlue()));
    RECT rect = { 0, 0, width, height };
    FillRect(memDC, &rect, brush);
    DeleteObject(brush);

    // Blend onto the screen using the alpha value from tintColor
    BLENDFUNCTION blend = {};
    blend.BlendOp = AC_SRC_OVER;
    blend.BlendFlags = 0;
    blend.SourceConstantAlpha = tintColor.GetAlpha(); // Opacity (0–255)
    blend.AlphaFormat = 0; // No per-pixel alpha

    AlphaBlend(hdc, 0, 0, width, height, memDC, 0, 0, width, height, blend);

    SelectObject(memDC, oldBmp);
    DeleteObject(bmp);
    DeleteDC(memDC);
}

void ApplySepia(HDC hdc, int width, int height) {
    ApplyTint(hdc, width, height, Color(64, 112, 66, 20)); // Sepia Tone
}
void ApplyContrast(HDC hdc, int width, int height, float contrastFactor = 1.5f) {
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

    // Copy screen to memory
    BitBlt(memDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    BITMAPINFO bmi = {0};
    bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    bmi.bmiHeader.biWidth = width;
    bmi.bmiHeader.biHeight = -height;  // top-down DIB
    bmi.bmiHeader.biPlanes = 1;
    bmi.bmiHeader.biBitCount = 32;
    bmi.bmiHeader.biCompression = BI_RGB;

    std::vector<BYTE> pixels(width * height * 4);
    GetDIBits(memDC, hBitmap, 0, height, pixels.data(), &bmi, DIB_RGB_COLORS);

    for (int i = 0; i < width * height * 4; i += 4) {
        for (int j = 0; j < 3; ++j) {
            float color = pixels[i + j] / 255.0f;
            color -= 0.5f;
            color *= contrastFactor;
            color += 0.5f;
            color *= 255.0f;
            pixels[i + j] = static_cast<BYTE>(std::max(0, std::min(255, static_cast<int>(color))));
        }
    }

    SetDIBits(memDC, hBitmap, 0, height, pixels.data(), &bmi, DIB_RGB_COLORS);

    BitBlt(hdc, 0, 0, width, height, memDC, 0, 0, SRCCOPY);

    SelectObject(memDC, oldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(memDC);
}

void DrawGlitches(HDC hdc, int width, int height) {
    for (int i = 0; i < 30; ++i) {
        HBRUSH brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        int x = rand() % width;
        int y = rand() % height;
        int w = rand() % 200 + 10;
        int h = rand() % 100 + 10;
        RECT rect = {x, y, x + w, y + h};
        FillRect(hdc, &rect, brush);
        DeleteObject(brush);
    }
}

// === Disco Filter ===

Color HSVtoRGB(float h, float s, float v, BYTE alpha = 64) {
    float r, g, b;
    int i = static_cast<int>(h * 6);
    float f = h * 6 - i;
    float p = v * (1 - s);
    float q = v * (1 - f * s);
    float t = v * (1 - (1 - f) * s);

    switch (i % 6) {
        case 0: r = v, g = t, b = p; break;
        case 1: r = q, g = v, b = p; break;
        case 2: r = p, g = v, b = t; break;
        case 3: r = p, g = q, b = v; break;
        case 4: r = t, g = p, b = v; break;
        case 5: r = v, g = p, b = q; break;
    }

    return Color(alpha, static_cast<BYTE>(r * 255), static_cast<BYTE>(g * 255), static_cast<BYTE>(b * 255));
}

void ApplyDisco(HDC hdc, int width, int height, float hue) {
    Color discoColor = HSVtoRGB(hue, 1.0f, 1.0f, 64);
    HBRUSH brush = CreateSolidBrush(RGB(discoColor.GetRed(), discoColor.GetGreen(), discoColor.GetBlue()));
    RECT rect = {0, 0, width, height};
    FillRect(hdc, &rect, brush);
    DeleteObject(brush);
}

// === Deep Fry Effect ===

void ApplyDeepFry(HDC hdc, int width, int height) {
    HBRUSH redBrush = CreateSolidBrush(RGB(255, 80, 0));
    HBRUSH yellowBrush = CreateSolidBrush(RGB(255, 255, 0));
    RECT rect = {0, 0, width, height};
    FillRect(hdc, &rect, redBrush);
    FillRect(hdc, &rect, yellowBrush);
    DeleteObject(redBrush);
    DeleteObject(yellowBrush);

    for (int i = 0; i < 20; ++i) {
        int x = rand() % width;
        int y = rand() % height;
        int w = rand() % 100 + 30;
        int h = rand() % 100 + 30;
        HBRUSH flash = CreateSolidBrush(RGB(255, 255, 255));
        RECT r = {x, y, x + w, y + h};
        FillRect(hdc, &r, flash);
        DeleteObject(flash);
    }
}

// === Flicker Effect ===

void ApplyFlicker(HDC hdc, int width, int height) {
    BYTE intensity = (rand() % 2 == 0) ? 255 : 0;
    HBRUSH flickerBrush = CreateSolidBrush(RGB(intensity, intensity, intensity));
    RECT rect = {0, 0, width, height};
    FillRect(hdc, &rect, flickerBrush);
    DeleteObject(flickerBrush);
}

// === Screen Shake Effect ===

void ApplyScreenShake(HDC hdc, int width, int height) {
    int shakeMagnitude = rand() % 25;
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
    BitBlt(memDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    for (int i = 0; i < 3; ++i) {
        int dx = (rand() % (2 * shakeMagnitude)) - shakeMagnitude;
        int dy = (rand() % (2 * shakeMagnitude)) - shakeMagnitude;
        BitBlt(hdc, dx, dy, width, height, memDC, 0, 0, SRCCOPY);
        Sleep(10);
    }

    SelectObject(memDC, oldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(memDC);
}

// === Move Screen Effect ===

void ApplyMoveScreen(HDC hdc, int width, int height) {
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);

    BitBlt(memDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    int moveDirection = rand() % 4;
    int moveMagnitude = 20;

    switch (moveDirection) {
        case 0:
            BitBlt(hdc, 0, 0, width - moveMagnitude, height, memDC, moveMagnitude, 0, SRCCOPY);
            break;
        case 1:
            BitBlt(hdc, moveMagnitude, 0, width - moveMagnitude, height, memDC, 0, 0, SRCCOPY);
            break;
        case 2:
            BitBlt(hdc, 0, 0, width, height - moveMagnitude, memDC, 0, moveMagnitude, SRCCOPY);
            break;
        case 3:
            BitBlt(hdc, 0, moveMagnitude, width, height - moveMagnitude, memDC, 0, 0, SRCCOPY);
            break;
    }

    SelectObject(memDC, oldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(memDC);
}

// ==== MAIN LOOP ====

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow){
    MessageBox(0, "Run?", "enziminas.exe", MB_OK);
    srand(static_cast<unsigned int>(time(nullptr)));

    ULONG_PTR gdiplusToken;
    InitGDIPlus(gdiplusToken);

    HWND hwnd = GetDesktopWindow();
    HDC hdc = GetDC(hwnd);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    float hue = 0.0f;

    for (int i = 0; i < 300; ++i) {
        int effect = rand() % 9;
        switch (effect) {
            case 0:
                ApplyTint(hdc, screenWidth, screenHeight, Color(64, 0, 255, 255));
                break;
            case 1:
                ApplySepia(hdc, screenWidth, screenHeight);
                break;
            case 2:
                DrawGlitches(hdc, screenWidth, screenHeight);
                break;
            case 3:
                ApplyDisco(hdc, screenWidth, screenHeight, hue);
                hue += 0.1f;
                if (hue > 1.0f) hue -= 1.0f;
                break;
            case 4:
                ApplyDeepFry(hdc, screenWidth, screenHeight);
                break;
            case 5:
                ApplyFlicker(hdc, screenWidth, screenHeight);
                break;
            case 6:
                ApplyScreenShake(hdc, screenWidth, screenHeight);
                break;
            case 7:
                ApplyMoveScreen(hdc, screenWidth, screenHeight);
                break;
            case 8:
                ApplyContrast(hdc, screenWidth, screenHeight);
                break;

        }
        Sleep(25);
    }

    ReleaseDC(hwnd, hdc);
    ShutdownGDIPlus(gdiplusToken);
    return 0;
}
