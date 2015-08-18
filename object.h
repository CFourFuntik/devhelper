#pragma once
#include "inc\types.h"
#include "inc\natives.h"
#include <vector>
#include <string>
#include <fstream>

#define PI 3.14159265

namespace lil
{
	typedef enum
	{
		POSITION_X,
		POSITION_Y,
		POSITION_Z,
		ROTATION_X,
		ROTATION_Y,
		ROTATION_Z
	} OBJECT_PARAM;

	typedef enum
	{
		LATITUDE,
		LONGTITUDE
	} CAM_PARAM;

	class Object
	{
	private:
		Vector3 _position;
		Vector3 _rotation;
		Hash _objectHash;
		Entity _handler;
		static Camera lastCam;
	public:
		static std::vector<Object*> _data;
		static float latitude, longtitude, distance;

		Object(Hash objectHash, bool dynamic = false);
		Object(Hash objectHash, Vector3 position, bool dynamic = false);
		Object(Hash objectHash, float posx, float posy, float posz, float rotx = .0, float roty = .0, float rotz = .0, bool dynamic = false);
		void Delete();
		void SetPosition(float posx, float posy, float posz);
		void SetPosition(Vector3 position);
		void SetRotation(float rotx, float roty, float rotz);
		void SetRotation(Vector3 rotation);
		void ChangeOneParam(OBJECT_PARAM param, float offset);
		void PointCam(Camera cam);
		Vector3 GetPosition();
		Vector3 GetRotation();
		std::string ExportObject();
		static bool ExportAll(std::string filename);
		static void RotateCam(CAM_PARAM param, float offset);
	};
}