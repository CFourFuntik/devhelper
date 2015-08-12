/*
F4 - save position
*/
#include "script.h"
#include "models.h"

vector<chatLine> chatLines;
vector<string> chatHistory;

VOID drawText(string text, float posx, float posy, int r, int g, int b)
{
	UI::_SET_TEXT_ENTRY("STRING");
	UI::_ADD_TEXT_COMPONENT_STRING((LPSTR)(text.c_str()));
	UI::SET_TEXT_FONT(0);
	UI::SET_TEXT_SCALE(0.3f, 0.3f);
	UI::SET_TEXT_COLOUR(r, g, b, 255);
	UI::SET_TEXT_WRAP(0.0, 1.0);
	UI::SET_TEXT_CENTRE(0);
	UI::SET_TEXT_DROPSHADOW(0, 0, 0, 0, 0);
	UI::SET_TEXT_EDGE(0, 0, 0, 0, 0);
	UI::SET_TEXT_LEADING(1);
	UI::_DRAW_TEXT(posx, posy);
}

string curChatText("");
bool chatEnabled = true, chatWrite = false;
string fileName;
int cursorPos = 0, historyPos = -1;

//skin change
bool needSkinChange = false;
DWORD skin = 0;
int skinId;

void sendPlayerMessage(const string &message, int r = 255, int g = 255, int b = 255)
{
	chatLines.push_back({ message , r, g, b});
	//temporary
	if (chatLines.size() > 10)
		chatLines.erase(chatLines.begin(), chatLines.begin() + 1);
}

void keyPressed(WCHAR symbol)
{
	if (!chatWrite) return;
	curChatText.insert(curChatText.begin() + cursorPos, (CHAR)symbol);
	cursorPos++;
}

bool commandProcessor(string command)
{
	vector<string> params = split(command, ' ');
	string cmd = params[0];
	params.erase(params.begin(), params.begin() + 1);

	if (!cmd.compare("/help"))
	{
		sendPlayerMessage("/save /clear /startfire /stopfire", 0, 125, 0);
		return true;
	}
	else if (!cmd.compare("/randcomps"))
	{
		PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), FALSE);
		sendPlayerMessage("Random component variation setted", 0, 200, 0);
		return true;
	}
	else if (!cmd.compare("/model"))
	{
		if (!params.size())
		{
			sendPlayerMessage("USAGE: /model [id(0-696)]", 125, 125, 125);
			return true;
		}
		int id;
		try {
			id = stoi(params[0]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: id must be numeric", 255, 50, 50);
			return true;
		}
		if (id < 0 || id >= MAX_MODELS)
		{
			sendPlayerMessage("USAGE: /model [id(0-696)]", 125, 125, 125);
			return true;
		}
		skin = GAMEPLAY::GET_HASH_KEY((char *)models[id]);
		needSkinChange = true;
		skinId = id;
		return true;
	}
	else if (!cmd.compare("/save"))
	{
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		if (ENTITY::DOES_ENTITY_EXIST(playerPed)) {
			if (PED::IS_PED_IN_ANY_VEHICLE(playerPed, 0)) {
				Vehicle veh = PED::GET_VEHICLE_PED_IS_USING(playerPed);
				Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, false);
				float rot = ENTITY::GET_ENTITY_HEADING(playerPed);
				ofstream fout(fileName.c_str(), ifstream::app);
				string comment;
				if (params.size())
					comment = " //";
				for (int i = 0; i < params.size(); ++i) 
					comment += (i == (params.size() - 1) ? params[i] : (params[i] + " "));
				fout << "Vehicle: " << pos.x << ", " << pos.y << ", " 
					<< pos.z << ", " << rot << comment << endl;
				fout.close();
				sendPlayerMessage("Your vehicle position was saved", 0, 125, 0);
			}
			else {
				Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, false);
				float rot = ENTITY::GET_ENTITY_HEADING(playerPed);
				ofstream fout(fileName.c_str(), ifstream::app);
				string comment;
				if (params.size())
					comment = " //";
				for (int i = 0; i < params.size(); ++i)
					comment += (i == (params.size() - 1) ? params[i] : (params[i] + " "));
				fout << "Ped: " << pos.x << ", " << pos.y << ", " 
					<< pos.z << ", " << rot << comment << endl;
				fout.close();
				sendPlayerMessage("Your onfoot position was saved", 0, 125, 0);
			}
		}
		return true;
	}
	else if (!cmd.compare("/clear"))
	{
		chatLines.clear();
		return true;
	}
	else if (!cmd.compare("/startfire"))
	{
		FIRE::START_ENTITY_FIRE(PLAYER::PLAYER_PED_ID());
		return true;
	}
	else if (!cmd.compare("/stopfire"))
	{
		FIRE::STOP_ENTITY_FIRE(PLAYER::PLAYER_PED_ID());
		return true;
	}
	return false;
}

void specKeyPressed(DWORD key)
{
	switch (key)
	{
		case VK_F6:
		{
			chatWrite = !chatWrite;
			PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), !chatWrite, 0);
			break;
		}
		case VK_F7:
		{
			chatEnabled = !chatEnabled;
			break;
		}
		case VK_BACK:
		{
			if (!chatWrite) break;
			if (cursorPos > 0)
			{
				curChatText.erase(curChatText.begin() + (cursorPos - 1), curChatText.begin() + cursorPos);
				cursorPos--;
			}
			break;
		}
		case VK_DELETE:
		{
			if (!chatWrite) break;
			if (cursorPos < curChatText.size())
			{
				curChatText.erase(curChatText.begin() + cursorPos, curChatText.begin() + cursorPos + 1);
			}
			break;
		}
		case VK_RETURN:
		{
			if (!chatWrite) break;
			if (curChatText.size())
			{
				if (curChatText[0] == '/')
				{
					if (!commandProcessor(curChatText))
						sendPlayerMessage("Unknown command", 125, 0, 0);
				}
				else
					sendPlayerMessage(SOCIALCLUB::_SC_GET_NICKNAME() + string(": ") + curChatText, 255, 255, 255);
				chatHistory.push_back(curChatText);
				historyPos = -1;
				curChatText = string("");
				cursorPos = 0;
			}
			chatWrite = false;
			PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), !chatWrite, 0);
			break;
		}
		case VK_LEFT:
		{
			if (!chatWrite) break;
			if (cursorPos > 0) cursorPos--;
			break;
		}
		case VK_RIGHT:
		{
			if (!chatWrite) break;
			if (cursorPos < curChatText.size())
			{
				cursorPos++;
			}
			break;
		}
		case VK_UP:
		{
			if (!chatWrite || !chatHistory.size()) break;
			if (historyPos < (chatHistory.size() - 1) || historyPos == -1)
			{
				historyPos++;
				curChatText = chatHistory[chatHistory.size() - historyPos - 1];
				cursorPos = (int)curChatText.size();
			}
			break;
		}
		case VK_DOWN:
		{
			if (!chatWrite || !chatHistory.size()) break;
			if (historyPos > 0)
			{
				historyPos--;
				curChatText = chatHistory[chatHistory.size() - historyPos - 1];
				cursorPos = (int)curChatText.size();
			}
			else if (historyPos == 0)
			{
				historyPos = -1;
				curChatText = "";
				cursorPos = 0;
			}
			break;
		}
	}
}

void main()
{
	char *pValue;
	size_t len;
	errno_t err = _dupenv_s(&pValue, &len, "USERPROFILE");
	if (err)
		fileName = string("C:\\savedcoords.txt");
	else 
		fileName = string(pValue) + "\\Documents\\Rockstar Games\\GTA V\\savedcoords.txt";

	sendPlayerMessage("GTAV DevHelper by Funtik Initialized");
	sendPlayerMessage("Press F6 to open chat textbox");
	while (true)
	{
		Player player = PLAYER::PLAYER_ID();
		Ped playerPed = PLAYER::PLAYER_PED_ID();
		if (ENTITY::DOES_ENTITY_EXIST(playerPed))
		{
			Hash model = ENTITY::GET_ENTITY_MODEL(playerPed);
			if (ENTITY::IS_ENTITY_DEAD(playerPed) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
			{
				if (model != GAMEPLAY::GET_HASH_KEY("player_zero") &&
					model != GAMEPLAY::GET_HASH_KEY("player_one") &&
					model != GAMEPLAY::GET_HASH_KEY("player_two"))
				{
					model = GAMEPLAY::GET_HASH_KEY("player_zero");
					STREAMING::REQUEST_MODEL(model);
					while (!STREAMING::HAS_MODEL_LOADED(model))
						WAIT(0);
					PLAYER::SET_PLAYER_MODEL(player, model);
					PED::SET_PED_DEFAULT_COMPONENT_VARIATION(playerPed);
					STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
					while (ENTITY::IS_ENTITY_DEAD(PLAYER::PLAYER_PED_ID()) || PLAYER::IS_PLAYER_BEING_ARRESTED(player, TRUE))
						WAIT(0);
				}
			}
		}
		if (chatEnabled)
		{
			GRAPHICS::DRAW_RECT(0.0f, 0.0f, 0.9f, 0.645f, 0, 0, 0, 200);
			size_t first = (chatLines.size() > 10) ? (chatLines.size() - 10) : 0;
			for (size_t i = first, c = 0; i < chatLines.size(); ++i, ++c)
			{
				drawText(chatLines[i].text, 0.0f, ((float)c)*(0.03f), chatLines[i].r, chatLines[i].g, chatLines[i].b);
			}
			
		}
		if (chatWrite)
		{
			string chatText(curChatText);
			chatText.insert(chatText.begin() + cursorPos, '_');
			GRAPHICS::DRAW_RECT(0.0f, 0.340f, 0.9f, 0.040f, 125, 125, 125, 255);
			drawText("> " + chatText, 0.0f, 0.330f, 255, 255, 255);
			UI::SET_PAUSE_MENU_ACTIVE(false);
		}
		if (needSkinChange)
		{
			if (STREAMING::IS_MODEL_IN_CDIMAGE(skin) && STREAMING::IS_MODEL_VALID(skin))
			{
				STREAMING::REQUEST_MODEL(skin);
				while (!STREAMING::HAS_MODEL_LOADED(skin))
					WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), skin);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(skin);
				sendPlayerMessage(string("Your model changed to ") + models[skinId], 0, 200, 0);
			}
			else
				sendPlayerMessage("Unknown error!", 200, 0, 0);
			needSkinChange = false;
		}
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
