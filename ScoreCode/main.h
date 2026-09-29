#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>

#include <d3d11.h>
#pragma comment (lib, "d3d11.lib")


#include <DirectXMath.h>
using namespace DirectX;

#include "DirectXTex.h"

#include <DxLib.h>
#include <vector>
#include <string>
#include <list>
#include "Framework\vector2.h"
#include "Input\input.h"

#if _DEBUG
#pragma comment(lib, "DirectXTex_Debug.lib")
#else
#pragma comment(lib, "DirectXTex_Release.lib")
#endif

#pragma comment (lib, "winmm.lib")


#define SCREEN_WIDTH	(1600)
#define SCREEN_HEIGHT	(900)


HWND GetWindow();

void Invoke(std::function<void()> Function, int Time);

