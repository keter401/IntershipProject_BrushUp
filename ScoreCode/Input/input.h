#pragma once

#define MAX_KEY 256
#define MAX_PADS 16

class DWInput
{
private:
    // --- Keyboard ---
    char  OldKeyState[MAX_KEY]{};
    char  KeyState[MAX_KEY]{};

    // --- Joypad ---
    int  PrevPadButtons = 0;
    int  PadButtons = 0;

public:
    DWInput() {};

	void Init();
	void Uninit();
	void Update();

    bool GetLeftBottom();
    bool GetRightBottom();
    bool GetActionBottom();
    bool GetActionBottomTrigger();
    bool GetActionBottomUp();

    // --- Keyboard ---
	bool GetKeyPress(const int keyCode);
	bool GetKeyTrigger(const int keyCode);
	bool GetKeyUp(const int keyCode);

    // --- Joypad ---
    bool GetPadPress(const int buttonMask);
    bool GetPadTrigger(const int buttonMask);
    bool GetPadUp(const int buttonMask);
};