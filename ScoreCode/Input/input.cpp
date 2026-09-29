#include "main.h"
#include "Input/input.h"
#include "DxLib.h"

void DWInput::Init()
{
    std::memset(OldKeyState, 0, sizeof(OldKeyState));
    std::memset(KeyState, 0, sizeof(KeyState));
    PrevPadButtons = 0;
    PadButtons = 0;
}

void DWInput::Uninit() {}

void DWInput::Update()
{
    // --- Keyboard ---
    std::memcpy(OldKeyState, KeyState, MAX_KEY);
    GetHitKeyStateAll(KeyState);

    // --- Joypad ---
    PrevPadButtons = PadButtons;
    PadButtons = GetJoypadInputState(DX_INPUT_PAD1);
}

bool DWInput::GetRightBottom()
{
    return GetKeyPress(KEY_INPUT_RIGHT) || GetPadPress(PAD_INPUT_RIGHT);
}

bool DWInput::GetLeftBottom()
{
    return GetKeyPress(KEY_INPUT_LEFT) || GetPadPress(PAD_INPUT_LEFT);
}

bool DWInput::GetActionBottom()
{
    return GetKeyPress(KEY_INPUT_SPACE) || GetKeyPress(KEY_INPUT_RSHIFT) || GetPadPress(PAD_INPUT_A);
}

bool DWInput::GetActionBottomTrigger()
{
    return GetKeyTrigger(KEY_INPUT_SPACE) || GetKeyTrigger(KEY_INPUT_RSHIFT) || GetPadTrigger(PAD_INPUT_A);
}

bool DWInput::GetActionBottomUp()
{
    return GetKeyUp(KEY_INPUT_SPACE) || GetKeyUp(KEY_INPUT_RSHIFT) || GetPadUp(PAD_INPUT_A);
}

// ------------------ Keyboard ------------------
bool DWInput::GetKeyPress(const int keyCode)
{
	return KeyState[keyCode] != 0;
}

bool DWInput::GetKeyTrigger(const int keyCode)
{
	return ((KeyState[keyCode] != 0) && !(OldKeyState[keyCode] != 0));
}

bool DWInput::GetKeyUp(const int keyCode)
{
	// 今フレーム離されていて、前フレームは押されていた
	return ((KeyState[keyCode] == 0) && (OldKeyState[keyCode] != 0));
}

// ------------------ Joypad ------------------
bool DWInput::GetPadPress(const int buttonMask)
{
    return (PadButtons & buttonMask) != 0;
}

bool DWInput::GetPadTrigger(const int buttonMask)
{
    return ((PadButtons & buttonMask) != 0) && ((PrevPadButtons & buttonMask) == 0);
}

bool DWInput::GetPadUp(const int buttonMask)
{
    return ((PadButtons & buttonMask) == 0) && ((PrevPadButtons & buttonMask) != 0);
}