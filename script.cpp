/*
F4 - save position
*/
#include "script.h"

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


void sendPlayerMessage(const string &message, int r, int g, int b)
{
	chatLines.push_back({ message , r, g, b});
	LOG::write(string("CHAT: ") + message);
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
		sendPlayerMessage("/save /clear", 0, 125, 0);
		sendPlayerMessage("/randcomps /setcomps /model", 0, 125, 0);
		sendPlayerMessage("/randprops /setprops /clearprops", 0, 125, 0);
		return true;
	}
	else if (!cmd.compare("/primcolor"))
	{
		if (params.size() < 3)
		{
			sendPlayerMessage("USAGE: /primcolor [rColor] [gColor] [bColor]", 125, 125, 125);
			return true;
		}
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) {
			sendPlayerMessage("Your must be in vehicle", 255, 0, 0);
			return true;
		}
		lastVehId = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

		int rColor;
		try {
			rColor = stoi(params[0]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: rColor must be numeric", 255, 50, 50);
			return true;
		}
		if (rColor < 0 || rColor > 255)
		{
			sendPlayerMessage("WARNING: invalid rColor value [0-255]", 255, 0, 0);
			return true;
		}

		int gColor;
		try {
			gColor = stoi(params[1]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: gColor must be numeric", 255, 50, 50);
			return true;
		}
		if (gColor < 0 || gColor > 255)
		{
			sendPlayerMessage("WARNING: invalid gColor value [0-255]", 255, 0, 0);
			return true;
		}

		int bColor;
		try {
			bColor = stoi(params[2]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: bColor must be numeric", 255, 50, 50);
			return true;
		}
		if (bColor < 0 || bColor > 255)
		{
			sendPlayerMessage("WARNING: invalid bColor value [0-255]", 255, 0, 0);
			return true;
		}
		VEHICLE::SET_VEHICLE_CUSTOM_PRIMARY_COLOUR(lastVehId, rColor, gColor, bColor);
		sendPlayerMessage("Primary colour changed", 0, 255, 0);
		return true;
	}
	else if (!cmd.compare("/seccolor"))
	{
		if (params.size() < 3)
		{
			sendPlayerMessage("USAGE: /seccolor [rColor] [gColor] [bColor]", 125, 125, 125);
			return true;
		}
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) {
			sendPlayerMessage("Your must be in vehicle", 255, 0, 0);
			return true;
		}
		lastVehId = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());

		int rColor;
		try {
			rColor = stoi(params[0]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: rColor must be numeric", 255, 50, 50);
			return true;
		}
		if (rColor < 0 || rColor > 255)
		{
			sendPlayerMessage("WARNING: invalid rColor value [0-255]", 255, 0, 0);
			return true;
		}

		int gColor;
		try {
			gColor = stoi(params[1]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: gColor must be numeric", 255, 50, 50);
			return true;
		}
		if (gColor < 0 || gColor > 255)
		{
			sendPlayerMessage("WARNING: invalid gColor value [0-255]", 255, 0, 0);
			return true;
		}

		int bColor;
		try {
			bColor = stoi(params[2]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: bColor must be numeric", 255, 50, 50);
			return true;
		}
		if (bColor < 0 || bColor > 255)
		{
			sendPlayerMessage("WARNING: invalid bColor value [0-255]", 255, 0, 0);
			return true;
		}
		VEHICLE::SET_VEHICLE_CUSTOM_SECONDARY_COLOUR(lastVehId, rColor, gColor, bColor);
		sendPlayerMessage("Secondary colour changed", 0, 255, 0);
		return true;
	}
	else if (!cmd.compare("/maxmod"))
	{
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) {
			sendPlayerMessage("Your must be in vehicle", 255, 0, 0);
			return true;
		}
		lastVehId = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		VEHICLE::SET_VEHICLE_MOD_KIT(lastVehId, 0);

		for (int i = 0; i < 17; ++i)
			if (VEHICLE::GET_NUM_VEHICLE_MODS(lastVehId, i))
				VEHICLE::SET_VEHICLE_MOD(lastVehId, i, VEHICLE::GET_NUM_VEHICLE_MODS(lastVehId, i) - 1, true);
		sendPlayerMessage("Your vehicle max tuned by Xzibit :-)", 0, 255, 0);
		return true;
	}
	else if (!cmd.compare("/vehmod"))
	{
		if (params.size() < 2)
		{
			sendPlayerMessage("USAGE: /vehmod [modType] [modIndex]", 125, 125, 125);
			return true;
		}
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) {
			sendPlayerMessage("Your must be in vehicle", 255, 0, 0);
			return true;
		}
		lastVehId = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		VEHICLE::SET_VEHICLE_MOD_KIT(lastVehId, 0);

		int modType;
		try {
			modType = stoi(params[0]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: modType must be numeric", 255, 50, 50);
			return true;
		}

		if (VEHICLE::GET_NUM_VEHICLE_MODS(lastVehId, modType) == 0)
		{
			sendPlayerMessage("WARNING: This mod does not supported by this vehicle", 255, 0, 0);
			return true;
		}

		int modIndex;
		try {
			modIndex = stoi(params[1]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: modIndex must be numeric", 255, 50, 50);
			return true;
		}

		if (modIndex < 0 || modIndex > VEHICLE::GET_NUM_VEHICLE_MODS(lastVehId, modType))
		{
			stringstream buff;
			buff << "WARNING: invalid modIndex value [0-" << VEHICLE::GET_NUM_VEHICLE_MODS(lastVehId, modType) << "]";
			sendPlayerMessage(buff.str(), 255, 0, 0);
			return true;
		}
		VEHICLE::SET_VEHICLE_MOD(lastVehId, modType, modIndex, true);
		sendPlayerMessage("Vehicle mods installed", 0, 255, 0);
		return true;
	}
	else if (!cmd.compare("/modkit"))
	{
		if (!params.size())
		{
			sendPlayerMessage("USAGE: /modkit [id]", 125, 125, 125);
			return true;
		}
		if (!PED::IS_PED_IN_ANY_VEHICLE(PLAYER::PLAYER_PED_ID(), 0)) {
			sendPlayerMessage("Your must be in vehicle", 255, 0, 0);
			return true;
		}
		Vehicle lastVehId = PED::GET_VEHICLE_PED_IS_USING(PLAYER::PLAYER_PED_ID());
		int modKit;
		try {
			modKit = stoi(params[0]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: modKit must be numeric", 255, 50, 50);
			return true;
		}
		if (modKit < 0 || modKit > VEHICLE::GET_NUM_MOD_KITS(lastVehId))
		{
			stringstream buff;
			buff << "WARNING: invalid vehId value [0-" << VEHICLE::GET_NUM_MOD_KITS(lastVehId) << "]";
			sendPlayerMessage(buff.str(), 255, 0, 0);
			return true;
		}
		modKitId = modKit;
		return true;
	}
	else if (!cmd.compare("/vehicle"))
	{
		if (!params.size())
		{
			char buff[255];
			sprintf_s(buff, "USAGE: /vehicle [id(0-%llu)]", maxVehNr);
			sendPlayerMessage(buff, 125, 125, 125);
			return true;
		}
		bool isNum = false;
		int vehId;
		try {
			vehId = stoi(params[0]);
			isNum = true;
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("Id is not numeric.Trying to get hash", 255, 50, 50);
		}
		if (isNum)
		{
			if (vehId < 0 || vehId > maxVehNr)
			{
				char buff[255];
				sprintf_s(buff, "USAGE: /vehicle [id(0-%llu)]", maxVehNr);
				sendPlayerMessage(buff, 125, 125, 125);
				return true;
			}
			vehCreateId = vehId;
		}
		else
		{
			vehHash = GAMEPLAY::GET_HASH_KEY((char*) params[0].c_str());
			if (STREAMING::IS_MODEL_IN_CDIMAGE(vehHash) && STREAMING::IS_MODEL_A_VEHICLE(vehHash))
			{
				vehCreateId = -2;
				vehName = params[0];
			}
			else
				sendPlayerMessage("Invalid vehicle title", 255, 50, 50);
		}
		return true;
	}
	else if (!cmd.compare("/model"))
	{
		if (!params.size())
		{
			char buff[255];
			sprintf_s(buff, "USAGE: /model [id(0-%llu)]", maxPedNr);
			sendPlayerMessage(buff, 125, 125, 125);
			return true;
		}
		int id;
		bool isNum = false;
		try {
			id = stoi(params[0]);
			isNum = true;
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("Trying to get hash", 255, 50, 50);
		}
		if (isNum)
		{
			if (id < 0 || id > maxPedNr)
			{
				char buff[255];
				sprintf_s(buff, "USAGE: /model [id(0-%llu)]", maxPedNr);
				sendPlayerMessage(buff, 125, 125, 125);
				return true;
			}
			pedId = id;
		}
		else
		{
			pedHash = GAMEPLAY::GET_HASH_KEY((char *)params[0].c_str());
			if (STREAMING::IS_MODEL_IN_CDIMAGE(pedHash) && STREAMING::IS_MODEL_VALID(pedHash))
			{
				pedName = params[0];
				pedId = -2;
			}
			else
				sendPlayerMessage("Invalid ped model title", 255, 50, 50);
		}
		return true;
	}
	else if (!cmd.compare("/object"))
	{
		if (!params.size())
		{
			char buff[255];
			sprintf_s(buff, "USAGE: /object [id(0-%llu)]", maxObjNr);
			sendPlayerMessage(buff, 125, 125, 125);
			return true;
		}
		int id;
		bool isNum = false;
		try {
			id = stoi(params[0]);
			isNum = true;
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: id must be numeric", 255, 50, 50);
			return true;
		}
		if (isNum)
		{
			if (id < 0 || id > maxObjNr)
			{
				char buff[255];
				sprintf_s(buff, "USAGE: /object [id(0-%llu)]", maxObjNr);
				sendPlayerMessage(buff, 125, 125, 125);
				return true;
			}
			objId = id;
		}
		else
		{
			objHash = GAMEPLAY::GET_HASH_KEY((char *)params[0].c_str());
			if (STREAMING::IS_MODEL_VALID(objHash))
			{
				objName = params[0];
				objId = -2;
			}
			else
				sendPlayerMessage("Invalid object title", 255, 50, 50);
		}
		objPos = ENTITY::GET_ENTITY_COORDS(PLAYER::PLAYER_PED_ID(), true);
		return true;
	}
	else if (!cmd.compare("/objexport"))
	{
		if (!params.size())
		{
			sendPlayerMessage("USAGE: /objexport [filename]", 125, 125, 125);
			return true;
		}
		if (!lil::Object::_data.size())
		{
			sendPlayerMessage("Nothing to export!", 255, 0, 0);
			return true;
		}
		lil::Object::ExportAll(params[0]);
		return true;
	}
	else if (!cmd.compare("/randcomps"))
	{
		PED::SET_PED_RANDOM_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), FALSE);
		sendPlayerMessage("Random component variation setted", 0, 200, 0);
		return true;
	}
	else if (!cmd.compare("/setcomps"))
	{
		if (params.size() < 4)
		{
			sendPlayerMessage("USAGE: /setcomps [component_id] [drawable_id] [texture_id] [palette_id]", 125, 125, 125);
			return true;
		}
		int componentId, drawableId, textureId, paletteId;
		try {
			componentId = stoi(params[0]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: componentId must be numeric", 255, 50, 50);
			return true;
		}

		try {
			drawableId = stoi(params[1]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: drawableId must be numeric", 255, 50, 50);
			return true;
		}

		try {
			textureId = stoi(params[2]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: textureId must be numeric", 255, 50, 50);
			return true;
		}

		try {
			paletteId = stoi(params[3]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: paletteId must be numeric", 255, 50, 50);
			return true;
		}

		if (PED::IS_PED_COMPONENT_VARIATION_VALID(PLAYER::PLAYER_PED_ID(), componentId, drawableId, textureId))
		{
			PED::SET_PED_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID(), componentId, drawableId, textureId, paletteId);
			sendPlayerMessage("Components changed", 0, 200, 0);
			return true;
		}
		else
		{
			sendPlayerMessage("ERROR: Components invalid", 255, 0, 0);
			return true;
		}

		return true;
	}
	else if (!cmd.compare("/randprops"))
	{
		PED::SET_PED_RANDOM_PROPS(PLAYER::PLAYER_PED_ID());
		sendPlayerMessage("Random properties setted", 0, 200, 0);
		return true;
	}
	else if (!cmd.compare("/clearprops"))
	{
		PED::CLEAR_ALL_PED_PROPS(PLAYER::PLAYER_PED_ID());
		return true;
	}
	else if (!cmd.compare("/setprops"))
	{
		if (params.size() < 4)
		{
			sendPlayerMessage("USAGE: /setprops [component_id] [drawable_id] [texture_id] [palette_id]", 125, 125, 125);
			return true;
		}
		int componentId, drawableId, textureId, paletteId;
		try {
			componentId = stoi(params[0]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: componentId must be numeric", 255, 50, 50);
			return true;
		}

		try {
			drawableId = stoi(params[1]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: drawableId must be numeric", 255, 50, 50);
			return true;
		}

		try {
			textureId = stoi(params[2]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: textureId must be numeric", 255, 50, 50);
			return true;
		}

		try {
			paletteId = stoi(params[3]);
		}
		catch (invalid_argument err)
		{
			sendPlayerMessage("WARNING: paletteId must be numeric", 255, 50, 50);
			return true;
		}

		if (PED::IS_PED_PROP_INDEX_VALID(PLAYER::PLAYER_PED_ID(), componentId, drawableId, textureId))
		{
			PED::SET_PED_PROP_INDEX(PLAYER::PLAYER_PED_ID(), componentId, drawableId, textureId, paletteId);
			sendPlayerMessage("Properties changed", 0, 200, 0);
			return true;
		}
		else
		{
			sendPlayerMessage("ERROR: Components invalid", 255, 0, 0);
			return true;
		}

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
				Vector3 pos = ENTITY::GET_ENTITY_COORDS(playerPed, true);
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
	return false;
}

void specKeyPressed(DWORD key, bool pressed)
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
		case 0x57:
		{
			if (objectEditor)
			{
				lil::Object::RotateCam(lil::CAM_PARAM::LATITUDE, (float)1.0);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case 0x53:
		{
			if (objectEditor)
			{
				lil::Object::RotateCam(lil::CAM_PARAM::LATITUDE, (float)-1.0);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case 0x41:
		{
			if (objectEditor)
			{
				lil::Object::RotateCam(lil::CAM_PARAM::LONGTITUDE, (float)-1.0);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case 0x44:
		{
			if (objectEditor)
			{
				lil::Object::RotateCam(lil::CAM_PARAM::LONGTITUDE, (float)1.0);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case VK_LEFT:
		{
			if (chatWrite && cursorPos > 0) cursorPos--;
			else if (objectEditor)
			{
				lil::Object::RotateCam(lil::CAM_PARAM::LONGTITUDE, (float)-1.0);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case VK_RIGHT:
		{
			if (chatWrite && cursorPos < curChatText.size())
			{
				cursorPos++;
			}
			else if (objectEditor)
			{
				lil::Object::RotateCam(lil::CAM_PARAM::LONGTITUDE, (float)1.0);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case VK_UP:
		{
			if (chatWrite && chatHistory.size())
			{
				if (historyPos < (chatHistory.size() - 1) || historyPos == -1)
				{
					historyPos++;
					curChatText = chatHistory[chatHistory.size() - historyPos - 1];
					cursorPos = (int)curChatText.size();
				}
			}
			else if (objectEditor)
			{
				lil::Object::RotateCam(lil::CAM_PARAM::LATITUDE, (float)1.0);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case VK_DOWN:
		{
			if (chatWrite && chatHistory.size())
			{
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
			}
			else if (objectEditor)
			{
				lil::Object::RotateCam(lil::CAM_PARAM::LATITUDE, (float)-1.0);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case VK_PRIOR:
		{
			if (!objectEditor) break;
			if ((lil::Object::distance - ((controlPressed) ? 1.0f : .01f)) < 0)
				lil::Object::distance = 0.0;
			else 
				lil::Object::distance -= ((controlPressed) ? 1.0f : .01f);
			playerObject->PointCam(playerCam);
			break;
		}
		case VK_NEXT:
		{
			if (!objectEditor) break;
			lil::Object::distance += ((controlPressed) ? 1.0f : .01f);
			playerObject->PointCam(playerCam);
			break;
		}
		case VK_ADD:
		{
			if (!objectEditor) break;
			playerObject->ChangeOneParam(lil::POSITION_Z, ((controlPressed) ? 1.0f : .01f));
			playerObject->PointCam(playerCam);
			break;
		}
		case VK_SUBTRACT:
		{
			if (!objectEditor) break;
			playerObject->ChangeOneParam(lil::POSITION_Z, -((controlPressed) ? 1.0f : .01f));
			playerObject->PointCam(playerCam);
			break;
		}
		case VK_NUMPAD2:
		{
			if (objectEditor)
			{
				Vector3 pos = playerObject->GetPosition();
				pos.x -= ((controlPressed) ? 1.0f : .01f) * cos(lil::Object::longtitude*PI / 180.0);
				pos.y -= ((controlPressed) ? 1.0f : .01f) * sin(lil::Object::longtitude*PI / 180.0);
				playerObject->SetPosition(pos);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case VK_NUMPAD8:
		{
			if (objectEditor)
			{
				Vector3 pos = playerObject->GetPosition();
				pos.x += ((controlPressed) ? 1.0f : .01f) * cos(lil::Object::longtitude*PI / 180.0);
				pos.y += ((controlPressed) ? 1.0f : .01f) * sin(lil::Object::longtitude*PI / 180.0);
				playerObject->SetPosition(pos);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case VK_NUMPAD4:
		{
			if (objectEditor)
			{
				Vector3 pos = playerObject->GetPosition();
				pos.x += ((controlPressed) ? 1.0f : .01f) * cos((lil::Object::longtitude + 90.0)*PI / 180.0);
				pos.y += ((controlPressed) ? 1.0f : .01f) * sin((lil::Object::longtitude + 90.0)*PI / 180.0);
				playerObject->SetPosition(pos);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case VK_NUMPAD6:
		{
			if (objectEditor)
			{
				Vector3 pos = playerObject->GetPosition();
				pos.x -= ((controlPressed) ? 1.0f : .01f) * cos((lil::Object::longtitude + 90.0)*PI / 180.0);
				pos.y -= ((controlPressed) ? 1.0f : .01f) * sin((lil::Object::longtitude + 90.0)*PI / 180.0);
				playerObject->SetPosition(pos);
				playerObject->PointCam(playerCam);
			}
			break;
		}
		case VK_NUMPAD1:
		{
			playerObject->ChangeOneParam(lil::OBJECT_PARAM::ROTATION_Z, ((controlPressed) ? 5.0f : .1f));
			break;
		}
		case VK_NUMPAD3:
		{
			playerObject->ChangeOneParam(lil::OBJECT_PARAM::ROTATION_Z, -((controlPressed) ? 5.0f : .1f));
			break;
		}
		case VK_NUMPAD7:
		{
			playerObject->ChangeOneParam(lil::OBJECT_PARAM::ROTATION_X, ((controlPressed) ? 5.0f : .1f));
			break;
		}
		case VK_NUMPAD9:
		{
			playerObject->ChangeOneParam(lil::OBJECT_PARAM::ROTATION_X, -((controlPressed) ? 5.0f : .1f));
			break;
		}
		case VK_DIVIDE:
		{
			playerObject->ChangeOneParam(lil::OBJECT_PARAM::ROTATION_Y, ((controlPressed) ? 5.0f : .1f));
			break;
		}
		case VK_MULTIPLY:
		{
			playerObject->ChangeOneParam(lil::OBJECT_PARAM::ROTATION_Y, -((controlPressed) ? 5.0f : .1f));
			break;
		}
		case VK_CONTROL:
		{
			controlPressed = !controlPressed;
			break;
		}
	}
}

void main()
{
	if (!CONFIG::get("saveFile", fileName))
	{
		char *pValue;
		size_t len;
		errno_t err = _dupenv_s(&pValue, &len, "USERPROFILE");
		if (err)
			fileName = string("C:\\savedcoords.txt");
		else
			fileName = string(pValue) + "\\Documents\\Rockstar Games\\GTA V\\savedcoords.txt";
	}

	sendPlayerMessage("GTAV DevHelper by Funtik Initialized");
	sendPlayerMessage("Press F6 to open chat textbox");

	auto loadEntities = []()
	{
		if (!CONFIG::loadList("devhelper/vehicles.txt", vehicles))
			sendPlayerMessage("Can not load devhelper/vehicles.txt", 255, 0, 0);
		else
		{
			maxVehNr = vehicles.size() - 1;
			sendPlayerMessage("Vehicles loaded", 0, 255, 0);
		}

		if (!CONFIG::loadList("devhelper/peds.txt", peds))
			sendPlayerMessage("Can not load devhelper/peds.txt", 255, 0, 0);
		else
		{
			maxPedNr = peds.size() - 1;
			sendPlayerMessage("Peds loaded", 0, 255, 0);
		}

		if (!CONFIG::loadList("devhelper/objects.txt", objects))
			sendPlayerMessage("Can not load devhelper/objects.txt", 255, 0, 0);
		else
		{
			maxObjNr = objects.size() - 1;
			sendPlayerMessage("Objects loaded", 0, 255, 0);
		}
	};
	thread(loadEntities).detach();

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
		if (modKitId != -1)
		{
			VEHICLE::SET_VEHICLE_MOD_KIT(lastVehId, modKitId);
			sendPlayerMessage("Your car\'s modkit changed", 0, 255, 0);
			modKitId = -1;
		}
		if (vehCreateId != -1)
		{
			stringstream buff;
			DWORD model = (vehCreateId == -2) ? vehHash : GAMEPLAY::GET_HASH_KEY((char *)vehicles[vehCreateId].c_str());
			buff << "Loading vehicle [" << vehCreateId << "] " << ((vehCreateId == -2) ? vehName : vehicles[vehCreateId]) << ", Hash = 0x" << std::hex << model;
			sendPlayerMessage(buff.str());
			if (STREAMING::IS_MODEL_IN_CDIMAGE(model) && STREAMING::IS_MODEL_A_VEHICLE(model))
			{
				STREAMING::REQUEST_MODEL(model);
				while (!STREAMING::HAS_MODEL_LOADED(model)) WAIT(0);
				Vector3 coords = ENTITY::GET_OFFSET_FROM_ENTITY_IN_WORLD_COORDS(PLAYER::PLAYER_PED_ID(), 0.0, 5.0, 0.0);
				Vehicle veh = VEHICLE::CREATE_VEHICLE(model, coords.x, coords.y, coords.z, 0.0, 1, 1);
				VEHICLE::SET_VEHICLE_ON_GROUND_PROPERLY(veh);

				ENTITY::SET_ENTITY_HEADING(veh, ENTITY::GET_ENTITY_HEADING(PLAYER::PLAYER_PED_ID()));
				PED::SET_PED_INTO_VEHICLE(PLAYER::PLAYER_PED_ID(), veh, -1);

				WAIT(0);
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(model);
				ENTITY::SET_VEHICLE_AS_NO_LONGER_NEEDED(&veh);
				sendPlayerMessage(string("Vehicle ") + ((vehCreateId == -2) ? vehName : vehicles[vehCreateId]) + " created.", 0, 255, 0);
			}
			else
			{
				sendPlayerMessage("Error when creating vehicle", 255, 0, 0);
			}
			vehCreateId = -1;
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
		if (pedId != -1)
		{
			Hash skin = (pedId == -2) ? pedHash : GAMEPLAY::GET_HASH_KEY((char*)peds[pedId].c_str());
			if (STREAMING::IS_MODEL_IN_CDIMAGE(skin) && STREAMING::IS_MODEL_VALID(skin))
			{
				STREAMING::REQUEST_MODEL(skin);
				while (!STREAMING::HAS_MODEL_LOADED(skin))
					WAIT(0);
				PLAYER::SET_PLAYER_MODEL(PLAYER::PLAYER_ID(), skin);
				PED::SET_PED_DEFAULT_COMPONENT_VARIATION(PLAYER::PLAYER_PED_ID());
				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(skin);
				sendPlayerMessage(string("Your model changed to ") + ((pedId == -2) ? pedName : peds[pedId]), 0, 200, 0);
			}
			else
				sendPlayerMessage("Unknown error!", 255, 0, 0);
			pedId = -1;
		}
		if (objectEditor)
		{
			/*for (unsigned i = 0; i < lil::Object::_data.size(); ++i)
			{
				Vector3 pos = lil::Object::_data[i]->GetPosition();
				GRAPHICS::DRAW_DEBUG_TEXT_2D("Object", pos.x, pos.y, pos.z, 255, 255, 255, 255);
			}*/
			Vector3 pos = playerObject->GetPosition();
			Vector3 rot = playerObject->GetRotation();
			drawText("Object parameters:", 0.1f, 0.8f, 255, 255, 255);
			char buff[256];
			sprintf_s(buff, "Position X: %.4f", pos.x);
			drawText(buff, 0.01f, 0.815f, 255, 255, 255);
			sprintf_s(buff, "Position Y: %.4f", pos.y);
			drawText(buff, 0.01f, 0.83f, 255, 255, 255);
			sprintf_s(buff, "Position Z: %.4f", pos.z);
			drawText(buff, 0.01f, 0.845f, 255, 255, 255);

			sprintf_s(buff, "Rotation X: %.4f", rot.x);
			drawText(buff, 0.01f, 0.86f, 255, 255, 255);
			sprintf_s(buff, "Rotation Y: %.4f", rot.y);
			drawText(buff, 0.01f, 0.875f, 255, 255, 255);
			sprintf_s(buff, "Rotation Z: %.4f", rot.z);
			drawText(buff, 0.01f, 0.89f, 255, 255, 255);
		}
		if (objId != -1)
		{
			Hash object = (objId == -2) ? pedHash : GAMEPLAY::GET_HASH_KEY((char*)objects[objId].c_str());
			if (STREAMING::IS_MODEL_VALID(object))
			{
				STREAMING::REQUEST_MODEL(object);
				while (!STREAMING::HAS_MODEL_LOADED(object))
					WAIT(0);
				if (!objectEditor)
				{
					objectEditor = true;
					GRAPHICS::SET_DEBUG_LINES_AND_SPHERES_DRAWING_ACTIVE(true);
					UI::DISPLAY_HUD(false);
					UI::DISPLAY_RADAR(false);
				}
				lil::Object *curObj = new lil::Object(object, objPos.x, objPos.y, objPos.z);
				playerObject = curObj;

				STREAMING::SET_MODEL_AS_NO_LONGER_NEEDED(object);
				ENTITY::SET_ENTITY_VISIBLE(PLAYER::PLAYER_PED_ID(), false);
				PLAYER::SET_PLAYER_CONTROL(PLAYER::PLAYER_ID(), false, 0);
				if(!CAM::DOES_CAM_EXIST(playerCam))
					playerCam = CAM::CREATE_CAMERA(0x19286a9, 0);
				if (CAM::DOES_CAM_EXIST(playerCam)) {
					CAM::SET_CAM_ACTIVE(playerCam, 1);
					CAM::RENDER_SCRIPT_CAMS(true, 0, playerCam, true, 0);
					playerObject->PointCam(playerCam);
				}
				sendPlayerMessage(string("Object created ") + ((objId == -2) ? objName : objects[objId]), 0, 200, 0);
			}
			else
				sendPlayerMessage("Unknown error!", 255, 0, 0);
			objId = -1;
		}
		WAIT(0);
	}
}

void ScriptMain()
{
	srand(GetTickCount());
	main();
}
