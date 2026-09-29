#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>

#define NOMINMAX
#include <windows.h>
#include <assert.h>
#include <functional>

// 描画は DxLib 経由で行うため、D3D11 / DirectXMath / DirectXTex への直接依存は持たない
#include <DxLib.h>
#include <vector>
#include <string>
#include <list>
#include "Framework\vector2.h"
#include "Input\input.h"

#pragma comment (lib, "winmm.lib")

constexpr int SCREEN_WIDTH  = 1600;
constexpr int SCREEN_HEIGHT = 900;

HWND GetWindow();
