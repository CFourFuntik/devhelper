/*
	THIS FILE IS A PART OF GTA V SCRIPT HOOK SDK
				http://dev-c.com
			(C) Alexander Blade 2015
*/

#pragma once

#include "inc\natives.h"
#include "inc\types.h"
#include "inc\enums.h"

#include "inc\main.h"

#include "keyboard.h"
#include "utils.h"
#include "object.h"

#include <string>
#include <ctime>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <thread>

using namespace std;

typedef struct {
	string text;
	int r;
	int g;
	int b;
} chatLine;

size_t maxVehNr = 0;
size_t maxPedNr = 0;
size_t maxObjNr = 0;

vector<string> vehicles;
vector<string> peds;
vector<string> objects;

int modKitId = -1;
int lastVehId = -1;

int vehCreateId = -1;
int pedId = -1;
int objId = -1;

Vector3 objPos;

Hash vehHash = 0;
Hash pedHash = 0;
Hash objHash = 0;
string vehName;
string pedName;
string objName;

Camera playerCam;
bool objectEditor = false;
lil::Object *playerObject = NULL;

bool controlPressed = false;
