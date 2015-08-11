/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#include "..\..\inc\natives.h"
#include "..\..\inc\types.h"
#include "..\..\inc\enums.h"

#include "..\..\inc\main.h"

#include "keyboard.h"
#include "utils.h"

#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>

using namespace std;

typedef struct {
	string text;
	int r;
	int g;
	int b;
} chatLine;

void ScriptMain();