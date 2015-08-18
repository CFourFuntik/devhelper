#include "object.h"

namespace lil
{
	std::vector<Object*> Object::_data;
	float Object::latitude = -90.0, Object::longtitude = 0.0, Object::distance = 10.0f;
	Camera Object::lastCam;

	Object::Object(Hash objectHash, bool dynamic)
	{
		_handler = OBJECT::CREATE_OBJECT(objectHash, 0.0, 0.0, 0.0, false, true, dynamic);
		_objectHash = objectHash;
		_position.x = 0.0;
		_position.y = 0.0;
		_position.z = 0.0;
		_rotation.x = 0.0;
		_rotation.y = 0.0;
		_rotation.z = 0.0;
		_data.push_back(this);
	}

	Object::Object(Hash objectHash, Vector3 position, bool dynamic)
	{
		_handler = OBJECT::CREATE_OBJECT(objectHash, position.x, position.y, position.z, false, true, dynamic);
		_objectHash = objectHash;
		_position = position;
		_rotation.x = 0.0;
		_rotation.y = 0.0;
		_rotation.z = 0.0;
		_data.push_back(this);
	}

	Object::Object(Hash objectHash, float posx, float posy, float posz, float rotx, float roty, float rotz, bool dynamic)
	{
		_handler = OBJECT::CREATE_OBJECT(objectHash, posx, posy, posz, false, true, dynamic);
		_objectHash = objectHash;
		_position.x = posx;
		_position.y = posy;
		_position.z = posz;
		_rotation.x = rotx;
		_rotation.y = roty;
		_rotation.z = rotz;
		if(rotx != .0 || roty != .0 || rotz != .0)
			ENTITY::SET_ENTITY_ROTATION(_handler, rotx, roty, rotz, 2, true);
		_data.push_back(this);
	}

	void Object::Delete()
	{
		OBJECT::DELETE_OBJECT(&_handler);
	}

	void Object::SetPosition(float posx, float posy, float posz)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(_handler, posx, posy, posz,
			false, false, false);
		_position.x = posx;
		_position.y = posy;
		_position.z = posz;
	}

	void Object::SetPosition(Vector3 position)
	{
		ENTITY::SET_ENTITY_COORDS_NO_OFFSET(_handler, position.x, position.y, position.z,
			false, false, false);
		ENTITY::SET_ENTITY_ROTATION(_handler, _rotation.x, _rotation.y, _rotation.z, 2, true);
		_position = position;
	}

	void Object::SetRotation(float rotx, float roty, float rotz)
	{
		ENTITY::SET_ENTITY_ROTATION(_handler, rotx, roty, rotz, 2, true);
	}

	void Object::SetRotation(Vector3 rotation)
	{
		ENTITY::SET_ENTITY_ROTATION(_handler, rotation.x, rotation.y, rotation.z, 2, true);
	}

	void Object::ChangeOneParam(OBJECT_PARAM param, float offset)
	{
		switch (param)
		{
			case OBJECT_PARAM::POSITION_X:
			{
				_position.x += offset;
				SetPosition(_position);
				break;
			}
			case OBJECT_PARAM::POSITION_Y:
			{
				_position.y += offset;
				SetPosition(_position);
				break;
			}
			case OBJECT_PARAM::POSITION_Z:
			{
				_position.z += offset;
				SetPosition(_position);
				break;
			}
			case OBJECT_PARAM::ROTATION_X:
			{
				_rotation.x += offset;
				if (_rotation.x > 360.0) _rotation.x = fmod(_rotation.x, 360.0);
				else if (_rotation.x < 0.0) _rotation.x += 360.0;
				SetRotation(_rotation);
				break;
			}
			case OBJECT_PARAM::ROTATION_Y:
			{
				_rotation.y += offset;
				if (_rotation.y > 360.0) _rotation.y = fmod(_rotation.y, 360.0);
				else if (_rotation.y < 0.0) _rotation.y += 360.0;
				SetRotation(_rotation);
				break;
			}
			case OBJECT_PARAM::ROTATION_Z:
			{
				_rotation.z += offset;
				if (_rotation.z > 360.0) _rotation.z = fmod(_rotation.z, 360.0);
				else if (_rotation.z < 0.0) _rotation.z += 360.0;
				SetRotation(_rotation);
				break;
			}
			default:return;
		}
	}

	void Object::PointCam(Camera cam)
	{
		CAM::POINT_CAM_AT_COORD(cam, _position.x, _position.y, _position.z);
		CAM::SET_CAM_COORD(cam,
			_position.x + distance * sin(latitude*PI / 180.0) * cos(longtitude*PI / 180.0),
			_position.y + distance * sin(latitude*PI / 180.0) * sin(longtitude*PI / 180.0),
			_position.z + distance * cos(latitude*PI / 180.0));
		lastCam = cam;
	}

	Vector3 Object::GetPosition()
	{
		return _position;
	}

	Vector3 Object::GetRotation()
	{
		return _rotation;
	}

	std::string Object::ExportObject()
	{
		char buff[512];
		sprintf_s(buff, "Object -> (%lu, %.4f, %.4f, %.4f, %.4f, %.4f, %.4f)\n",
			_objectHash, _position.x, _position.y, _position.z, _rotation.x, _rotation.y, _rotation.z);
		return std::string(buff);
	}

	bool Object::ExportAll(std::string filename)
	{
		std::ofstream out(filename);
		if (!out.is_open()) return false;
		for (unsigned i = 0; i < _data.size(); ++i)
		{
			out << _data[i]->ExportObject();
		}
		out.close();
		return true;
	}
	void Object::RotateCam(CAM_PARAM param, float offset)
	{
		if (param == CAM_PARAM::LONGTITUDE)
		{
			longtitude = (std::fmodf((longtitude + (float)180.0 + offset), (float)360.0)) - (float)180.0;
		}
		else if (param == CAM_PARAM::LATITUDE)
		{
			if ((latitude + offset) > (float)-0.001)
				latitude = (float)-0.001;
			else if ((latitude + offset) < (float)-179.999)
				latitude = (float)-179.999;
			else
				latitude += offset;
		}
	}
}