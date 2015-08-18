/*
		THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
					http://dev-c.com
				(C) Alexander Blade 2015
*/

#include "keyboard.h"

bool pressing[255];

void keyPressed(WCHAR symbol);
void specKeyPressed(DWORD key, bool pressed = true);

int VKToChar(unsigned int vk, WCHAR *s)
{
	int scan;
	HKL layout = GetKeyboardLayout(0);
	BYTE kbd[256];
	if(!GetKeyboardState(kbd))
		return 0;
	scan = MapVirtualKeyEx(vk, 0, layout);
	return ToUnicode(vk, scan, kbd, s, 2, 0);
}

void OnKeyboardMessage(DWORD key, WORD repeats, BYTE scanCode, BOOL isExtended, BOOL isWithAlt, BOOL wasDownBefore, BOOL isUpNow)
{
	if (!isUpNow)
	{
		if (!pressing[key])
		{
			WCHAR *symb = new WCHAR[2];
			int res = VKToChar(key, symb);
			if(key != VK_RETURN && key != VK_BACK && key != VK_DELETE && res)
				keyPressed(symb[0]);
			specKeyPressed(key);
		}
	}
	else
	{
		if (pressing[key])
		{
			specKeyPressed(key, false);
			pressing[key] = false;
		}
	}
}