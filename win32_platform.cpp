#include <windows.h>
#include "utils.cpp"
#include "platform_common.cpp"

global_variable bool isRunning = true;
global_variable float frame_time_ms = 1000 * 1.f/10; // Limit Framerate

struct RenderState {
    int width, height;
    void* memory;
    BITMAPINFO bitmap_info; 
};

global_variable RenderState render_state;
global_variable Input input = {};
#include "renderer.cpp"
#include "game.cpp"

LRESULT CALLBACK window_callback(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam) {
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
            render_state.width = rect.right - rect.left;
            render_state.height = rect.bottom - rect.top;
            int size = render_state.width * render_state.height * sizeof(unsigned int);

            if (render_state.memory) VirtualFree(render_state.memory, 0, MEM_RELEASE);
            render_state.memory = VirtualAlloc(0, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
            render_state.bitmap_info.bmiHeader.biSize = sizeof(render_state.bitmap_info.bmiHeader);
            render_state.bitmap_info.bmiHeader.biWidth = render_state.width;
            render_state.bitmap_info.bmiHeader.biHeight = render_state.height;
            render_state.bitmap_info.bmiHeader.biPlanes = 1;
            render_state.bitmap_info.bmiHeader.biBitCount = 32;
            render_state.bitmap_info.bmiHeader.biCompression = BI_RGB;
        }

        default:
            result = DefWindowProc(hWnd, Msg, wParam, lParam);
    }

    return result;
}

#define process_key(k, b)\
case k: { \
input.buttons[b].changed = is_down != input.buttons[b].isDown; \
input.buttons[b].isDown = is_down; \
} break;

internal void handle_input(HWND window) {
    for (int i = 0; i < BUTTON_COUNT; i++) {
        input.buttons[i].changed = false;
    }

    MSG message;
    while (PeekMessage(&message, window, 0, 0, PM_REMOVE)) {
        switch (message.message) {
        case WM_KEYUP:
        case WM_KEYDOWN: {
            u32 vk_code = (u32)message.wParam;
            bool is_down = ((message.lParam & 1 << 31) == 0);

            switch (vk_code) {
                process_key(VK_UP, BUTTON_UP);
                process_key(0x57, BUTTON_UP); // W
                process_key(VK_DOWN, BUTTON_DOWN);
                process_key(0x53, BUTTON_DOWN); // S
                process_key(VK_LEFT, BUTTON_LEFT);
                process_key(0x41, BUTTON_LEFT); // A
                process_key(VK_RIGHT, BUTTON_RIGHT);
                process_key(0x44, BUTTON_RIGHT); //D
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
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "Game Window Class";
    window_class.lpfnWndProc = window_callback;

    // Register Class
    RegisterClass(&window_class);

    //Create Window
    HWND window = CreateWindow(window_class.lpszClassName, "My First Game", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 720, 720, 0, 0, hInstance, 0);
    HDC hdc = GetDC(window);

    LARGE_INTEGER frame_start_time;
    LARGE_INTEGER frame_end_time;
    QueryPerformanceCounter(&frame_start_time);

    
    float performance_frequency; // Grab CPU Frequency to track framerate
    {
        LARGE_INTEGER perf_freq;
        QueryPerformanceFrequency(&perf_freq); 
        performance_frequency = float(perf_freq.QuadPart);
    }

    // Game Setup 
    setup_game();

    // Game Loop
    while (isRunning) {
        // Input
        handle_input(window);
        

        // Update
        simulate_game(&input);


        // Render
        render_game();
        StretchDIBits(hdc, 0, 0, render_state.width, render_state.height, 0, 0, render_state.width, render_state.height, render_state.memory, &render_state.bitmap_info, DIB_RGB_COLORS, SRCCOPY);
        
        // Sleep to limit framerate
        QueryPerformanceCounter(&frame_end_time);
        float deltaTime = ((float)(frame_end_time.QuadPart - frame_start_time.QuadPart) / performance_frequency);
        frame_start_time = frame_end_time;
        Sleep(frame_time_ms);
    }
}