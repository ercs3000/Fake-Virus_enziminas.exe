#include <windows.h>
#include <gdiplus.h>
#include <ctime>
#include <cstdlib>
#include <cmath>

#pragma comment (lib,"Gdiplus.lib")
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
    Graphics graphics(hdc);
    SolidBrush brush(Color(64, tintColor.GetRed(), tintColor.GetGreen(), tintColor.GetBlue()));
    graphics.FillRectangle(&brush, 0, 0, width, height);
}

void ApplySepia(HDC hdc, int width, int height) {
    ApplyTint(hdc, width, height, Color(64, 112, 66, 20));
}

void DrawGlitches(HDC hdc, int width, int height) {
    Graphics graphics(hdc);
    for (int i = 0; i < 30; ++i) {
        SolidBrush brush(Color(rand() % 256, rand() % 256, rand() % 256, rand() % 256));
        int x = rand() % width;
        int y = rand() % height;
        int w = rand() % 200 + 10;
        int h = rand() % 100 + 10;
        graphics.FillRectangle(&brush, x, y, w, h);
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
    Graphics graphics(hdc);
    Color discoColor = HSVtoRGB(hue, 1.0f, 1.0f, 64); 
    SolidBrush brush(discoColor);
    graphics.FillRectangle(&brush, 0, 0, width, height);
}

// === Deep Fry Effect (Extreme contrast) ===

void ApplyDeepFry(HDC hdc, int width, int height) {
    Graphics graphics(hdc);
    SolidBrush redBrush(Color(90, 255, 80, 0)); 
    SolidBrush yellowBrush(Color(60, 255, 255, 0)); 

    graphics.FillRectangle(&redBrush, 0, 0, width, height);
    graphics.FillRectangle(&yellowBrush, 0, 0, width, height);

    for (int i = 0; i < 20; ++i) {
        int x = rand() % width;
        int y = rand() % height;
        int w = rand() % 100 + 30;
        int h = rand() % 100 + 30;
        SolidBrush flash(Color(rand() % 50 + 200, 255, 255, 255));
        graphics.FillRectangle(&flash, x, y, w, h);
    }
}

// === Flicker Effect (Random black/white flashes) ===

void ApplyFlicker(HDC hdc, int width, int height) {
    Graphics graphics(hdc);
    BYTE intensity = (rand() % 2 == 0) ? 255 : 0; 
    BYTE alpha = rand() % 100 + 50; 
    Color flickerColor(alpha, intensity, intensity, intensity);
    SolidBrush flickerBrush(flickerColor);
    graphics.FillRectangle(&flickerBrush, 0, 0, width, height);
}

// === Screen Shake Effect (Simulate screen vibration) ===

void ApplyScreenShake(HDC hdc, int width, int height) {
    Graphics graphics(hdc);
    int shakeMagnitude = 10; 

    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
    BitBlt(memDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);

    Bitmap bmp(hBitmap, nullptr);

    for (int i = 0; i < 5; ++i) {
        int dx = (rand() % (2 * shakeMagnitude)) - shakeMagnitude;
        int dy = (rand() % (2 * shakeMagnitude)) - shakeMagnitude;

        graphics.DrawImage(&bmp, dx, dy);
        Sleep(10);
    }

    SelectObject(memDC, oldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(memDC);
}

// === Move Screen Effect (Change the position of pixels) ===

void ApplyMoveScreen(HDC hdc, int width, int height) {
    Graphics graphics(hdc);
    
    // Create a memory DC and bitmap to capture the screen
    HDC memDC = CreateCompatibleDC(hdc);
    HBITMAP hBitmap = CreateCompatibleBitmap(hdc, width, height);
    HBITMAP oldBitmap = (HBITMAP)SelectObject(memDC, hBitmap);
    
    // Capture the current screen
    BitBlt(memDC, 0, 0, width, height, hdc, 0, 0, SRCCOPY);
    
    // Randomly select a direction and magnitude for the move
    int moveDirection = rand() % 4; // 0: Left, 1: Right, 2: Up, 3: Down
    int moveMagnitude = 20; // Move magnitude in pixels

    switch (moveDirection) {
        case 0: // Move Left
            BitBlt(hdc, 0, 0, width - moveMagnitude, height, memDC, moveMagnitude, 0, SRCCOPY);
            break;
        case 1: // Move Right
            BitBlt(hdc, moveMagnitude, 0, width - moveMagnitude, height, memDC, 0, 0, SRCCOPY);
            break;
        case 2: // Move Up
            BitBlt(hdc, 0, 0, width, height - moveMagnitude, memDC, 0, moveMagnitude, SRCCOPY);
            break;
        case 3: // Move Down
            BitBlt(hdc, 0, moveMagnitude, width, height - moveMagnitude, memDC, 0, 0, SRCCOPY);
            break;
    }

    // Clean up
    SelectObject(memDC, oldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(memDC);
}

// ==== MAIN LOOP ==== 

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    ULONG_PTR gdiplusToken;
    InitGDIPlus(gdiplusToken);

    HDC hdc = GetDC(nullptr);
    int screenWidth = GetSystemMetrics(SM_CXSCREEN);
    int screenHeight = GetSystemMetrics(SM_CYSCREEN);

    float hue = 0.0f;

    for (int i = 0; i < 300; ++i) {
        int effect = rand() % 8; // 8 total effects
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
                hue += 0.05f;
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
        }
        Sleep(50);
    }

    ReleaseDC(nullptr, hdc);
    ShutdownGDIPlus(gdiplusToken);
    return 0;
}
