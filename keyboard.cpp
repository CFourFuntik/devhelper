/*
		THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
					http://dev-c.com
				(C) Alexander Blade 2015
*/

#include "keyboard.h"

const int KEYS_SIZE = 255;

struct {
	DWORD time;
	BOOL isWithAlt;
	BOOL wasDownBefore;
	BOOL isUpNow;
} keyStates[KEYS_SIZE];

bool pressing[KEYS_SIZE];

void keyPressed(WCHAR symbol);
void specKeyPressed(DWORD key);

int VKToChar(unsigned int vk, WCHAR *s)
{
	int scan;
	HKL layout = GetKeyboardLayout(0);
	BYTE kbd[256];
	if(!GetKeyboardState(kbd))
		return 0;
	scan = MapVirtualKeyEx(vk, 0, layout);
	return ToUnicodeEx(vk, scan, kbd, s, 2, 0, layout);
}

void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	if (key < KEYS_SIZE)
	{
		keyStates[key].time = GetTickCount();
		keyStates[key].isWithAlt = isWithAlt;
		keyStates[key].wasDownBefore = wasDownBefore;
		keyStates[key].isUpNow = isUpNow;
	}
	if (!keyStates[key].isUpNow)
	{
		if (!pressing[key])
		{
			/*LPTSTR layout;
			GetKeyboardLayoutName(layout);
			HKL trans = LoadKeyboardLayout(layout, KLF_ACTIVATE);

			CHAR symb = MapVirtualKeyEx(key, MAPVK_VK_TO_CHAR, trans);*/
			WCHAR *symb = new WCHAR[2];
			int res = VKToChar(key, symb);
			if(key != VK_RETURN && key != VK_BACK && key != VK_DELETE && res)
				keyPressed(symb[0]);
			else
				specKeyPressed(key);
		}
	}
	else
	{
		if (pressing[key])
			pressing[key] = false;
	}
}

const int NOW_PERIOD = 100, MAX_DOWN = 5000; // ms

bool IsKeyDown(DWORD key)
{
	return (key < KEYS_SIZE) ? ((GetTickCount() < keyStates[key].time + MAX_DOWN) && !keyStates[key].isUpNow) : false;
}

bool IsKeyJustUp(DWORD key, bool exclusive)
{
	bool b = (key < KEYS_SIZE) ? (GetTickCount() < keyStates[key].time + NOW_PERIOD && keyStates[key].isUpNow) : false;
	if (b && exclusive)
		ResetKeyState(key);
	return b;
}

void ResetKeyState(DWORD key)
{
	if (key < KEYS_SIZE)
		memset(&keyStates[key], 0, sizeof(keyStates[0]));
}