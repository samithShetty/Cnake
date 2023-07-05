#include <windows.h>

LRESULT CALLBACK window_callback( HWND hWnd, UINT Msg, WPARAM wParam,LPARAM lParam) {
    return DefWindowProc(hWnd,Msg, wParam, lParam);
}

int WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nShowCmd) {
    // Create Window Class
    WNDCLASS window_class = {};
    window_class.style = CS_HREDRAW | CS_VREDRAW;
    window_class.lpszClassName = "Game Window Class";
    window_class.lpfnWndProc = window_callback;

    // Register Class
    //RegisterClass(&window_class);
}