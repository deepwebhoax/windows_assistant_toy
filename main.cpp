// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com



#ifndef UNICODE
#define UNICODE
#endif

#include <iostream>
#include <windows.h>
#include "headers/avatar.h"



LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    static Avatar* avatar = nullptr;

    switch (uMsg)
    {
    case WM_CREATE:
        avatar = new Avatar(hwnd);
        if (!avatar->Initialize())
        {
            delete avatar;
            return -1;
        }
        return 0;

    case WM_PAINT:
        avatar->Render();
        return 0;

    case WM_DESTROY:
        delete avatar;
        PostQuitMessage(0);
        return 0;

    case WM_KEYDOWN:
        switch (wParam)
        {
        case VK_LEFT:
            avatar->Move(-10.0f, 0.0f);
            break;
        case VK_RIGHT:
            avatar->Move(10.0f, 0.0f);
            break;
        case VK_UP:
            avatar->Move(0.0f, -10.0f);
            break;
        case VK_DOWN:
            avatar->Move(0.0f, 10.0f);
            break;
        case 'A':
            avatar->Animate();
            break;
        }
        return 0;
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow)
{
    const wchar_t CLASS_NAME[] = L"AvatarWindowClass";
    std::cout << "Hello, World!1" << std::endl;
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);
    std::cout << "Hello, World2!" << std::endl;

    HWND hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Avatar Example",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 800, 600,
        nullptr,
        nullptr,
        hInstance,
        nullptr);
    std::cout << "Hello, Worl3d!" << std::endl;

    if (hwnd == nullptr)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);
    std::cout << "Hello, World4!" << std::endl;

    MSG msg = {};
    while (GetMessage(&msg, nullptr, 0, 0))
    {
    std::cout << "Hello, World!5" << std::endl;

        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}