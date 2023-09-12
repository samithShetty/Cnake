#include <windows.h>
#include "utils.cpp"
#include "platform_common.cpp"

global_variable bool isRunning = true;
global_variable float FrameTimeMillis = 1000 * 1.f/10; // Limit Framerate to 10 FPS

struct RenderState {
    int width, height;
    void* memory;
    BITMAPINFO bitmapInfo; 
};

global_variable RenderState renderState;
global_variable Input input = {};
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK windowCallback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
    LRESULT result = 0;

    switch (Msg) {
        // Window Closed
        case WM_CLOSE:
        case WM_DESTROY: {
            isRunning = false;
        } break;

        // Window Resized
        case WM_SIZE: {
            RECT rect;
            GetClientRect(hWnd, &rect);
            renderState.width = rect.right - rect.left;
            renderState.height = rect.bottom - rect.top;
            int size = renderState.width * renderState.height * sizeof(unsigned int);

            if (renderState.memory) VirtualFree(renderState.memory, 0, MEM_RELEASE);
            renderState.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            renderState.bitmapInfo.bmiHeader.biSize = sizeof(renderState.bitmapInfo.bmiHeader);
            renderState.bitmapInfo.bmiHeader.biWidth = renderState.width;
            renderState.bitmapInfo.bmiHeader.biHeight = renderState.height;
            renderState.bitmapInfo.bmiHeader.biPlanes = 1;
            renderState.bitmapInfo.bmiHeader.biBitCount = 32;
            renderState.bitmapInfo.bmiHeader.biCompression = BI_RGB;

            if (grid) {
                grid->style.setOffset(grid->width, grid->height);
            }
        }

        default:
            result = DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return result;
}

#define processKey(k, b)\
case k: { \
input.buttons[b].changed = isDown != input.buttons[b].isDown; \
input.buttons[b].isDown = isDown; \
} break;

internal void handleInput(HWND window) {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        input.buttons[i].changed = false;
    }

    MSG message;
    while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
        switch (message.message) {
        case WM_KEYUP:
        case WM_KEYDOWN: {
            u32 vkCode = (u32)message.wParam;
            bool isDown = ((message.lParam & 1 << 31) == 0);

            switch (vkCode) {
                processKey(VK_UP, BUTTON_UP);
                processKey(0x57, BUTTON_UP); // W
                processKey(VK_DOWN, BUTTON_DOWN);
                processKey(0x53, BUTTON_DOWN); // S
                processKey(VK_LEFT, BUTTON_LEFT);
                processKey(0x41, BUTTON_LEFT); // A
                processKey(VK_RIGHT, BUTTON_RIGHT);
                processKey(0x44, BUTTON_RIGHT); //D
            }
        } break;

        default:
            TranslateMessage(&message);
            DispatchMessage(&message);
        }
    }
}

int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
    
    // Create Window Class
    WNDCLASS WindowClass = {};
    WindowClass.style = CS_HREDRAW | CS_VREDRAW;
    WindowClass.lpszClassName = "Game Window Class";
    WindowClass.lpfnWndProc = windowCallback;

    // Register Class
    RegisterClass(&WindowClass);

    //Create Window
    HWND window = CreateWindow(WindowClass.lpszClassName, "Cnake", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 720, 720, 0, 0, hInstance, 0);
    HDC hdc = GetDC(window);

    LARGE_INTEGER frameStartTime;
    LARGE_INTEGER frameEndTime;
    QueryPerformanceCounter(&frameStartTime);

    // Grab CPU Frequency to track framerate
    float performanceFrequency; 
    LARGE_INTEGER _perfFreq;
    QueryPerformanceFrequency(&_perfFreq); 
    performanceFrequency = float(_perfFreq.QuadPart);

    // Game Setup 
    setupGame();

    // Game Loop
    while (isRunning) {
        handleInput(window);

        simulateGame(&input);

        renderGame();
        StretchDIBits(hdc, 0, 0, renderState.width, renderState.height, 0, 0, renderState.width, renderState.height, renderState.memory, &renderState.bitmapInfo, DIB_RGB_COLORS, SRCCOPY);
        
        // Sleep to limit framerate
        QueryPerformanceCounter(&frameEndTime);
        float deltaTime = ((float)(frameEndTime.QuadPart - frameStartTime.QuadPart) / performanceFrequency);
        frameStartTime = frameEndTime;
        Sleep(FrameTimeMillis-deltaTime);
    }
}