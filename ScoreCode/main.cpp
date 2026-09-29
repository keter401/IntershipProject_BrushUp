//====================================================================================================================
// main.cpp
// 
// 制作者　：　陳泳冲
// 制作日時：　2025/10/03
//====================================================================================================================
#include "main.h"
#include "Scene\SceneManager.h"
#include <thread>
#include <timeapi.h>

#pragma comment(lib, "winmm.lib")

const char* CLASS_NAME = "AppClass";
const char* WINDOW_NAME = "DX11ゲーム";

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

HWND g_Window;
HWND GetWindow() { return g_Window; }

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow)
{
    WNDCLASSEX wcex{};
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = 0;
    wcex.lpfnWndProc = WndProc;
    wcex.hInstance = hInstance;
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = nullptr;
    wcex.lpszClassName = CLASS_NAME;
    RegisterClassEx(&wcex);

    RECT rc = { 0,0,(LONG)SCREEN_WIDTH,(LONG)SCREEN_HEIGHT };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    g_Window = CreateWindowEx(
        0, CLASS_NAME, WINDOW_NAME,
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT,
        rc.right - rc.left, rc.bottom - rc.top,
        nullptr, nullptr, hInstance, nullptr);

    // 自作ウィンドウ描画設定
    SetUserWindow(g_Window);
    SetUseDirect3DVersion(DX_DIRECT3D_11);
    ChangeWindowMode(TRUE);
    SetWindowSizeChangeEnableFlag(FALSE, FALSE);
    SetGraphMode(SCREEN_WIDTH, SCREEN_HEIGHT, 32);
    SetMainWindowText("Downwell");

    if (DxLib_Init() == -1) return -1;           // 初期化（失敗なら終了）
    SetDrawScreen(DX_SCREEN_BACK);               // 裏画面へ描画

    CoInitializeEx(nullptr, COINITBASE_MULTITHREADED);

    DWSceneManager sceneManager;

    sceneManager.Init();

    ShowWindow(g_Window, nCmdShow);
    UpdateWindow(g_Window);

    const DWORD kFrameMS = 1000 / 60;
    timeBeginPeriod(1);
    DWORD last = timeGetTime();

    MSG msg{};
    while (1)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT) break;
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        else
        {
            // 内部処理（入力等）
            ProcessMessage();

            DWORD now = timeGetTime();
            if (now - last >= kFrameMS)
            {
                last += kFrameMS;

                // 毎フレームの描画手順
                ClearDrawScreen();   // 画面クリア(裏)

                sceneManager.Update();
                sceneManager.Draw();     

                ScreenFlip();        // 裏→表へ反映
            }
            else
            {
                Sleep(0);
            }
        }
    }

    timeEndPeriod(1);

    // 終了処理: サウンド解放などが DxLib の関数を呼ぶので、シーンを先に片付けてから DxLib_End
    sceneManager.Uninit();
    DxLib_End();
    CoUninitialize();

    UnregisterClass(CLASS_NAME, wcex.hInstance);
    return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    case WM_KEYDOWN:
        if (wParam == VK_ESCAPE) DestroyWindow(hWnd);
        break;
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
