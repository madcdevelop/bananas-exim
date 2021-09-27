#ifndef CORE_ENGINE_SERIALIZE_H
#define CORE_ENGINE_SERIALIZE_H

#include "Common.h"
#include "../Graphics/Scene.h"

namespace CoreEngine
{

bool SerializeToXML(const std::string& filePath, GraphicsEngine::Scene* scene);
void OutputItemXML(std::fstream& output, int32 indents, const std::string& name, const real32 value, const std::string& attributes);
void OutputAttributesXML(std::fstream& output, const std::string& attributes);


}

#endif

/* 
class Camera {
    vec3 Position
    vec3 Front
    vec3 Up
    vec3 Right

    real32 Yaw
    real32 Pitch
    real32 Fov
    real32 MovementSpeed
    real32 Sensitivity
};

XML

<camera id="0" version="1">
    <count>1</count>
    <item id="1" version="1">
        <camera.position type="vec3">
            <camera.front.x>-1.203</camera.front.x>
            <camera.front.y>-1.203</camera.front.y>
            <camera.front.z>-1.203</camera.front.z>
        </camera.position>
        <camera.front type="vec3">
            <camera.front.x>-1.203</camera.front.x>
            <camera.front.y>-1.203</camera.front.y>
            <camera.front.z>-1.203</camera.front.z>
        </camera.front>
        <camera.up type="vec3">
            <camera.up.x>-1.203</camera.up.x>
            <camera.up.y>-1.203</camera.up.y>
            <camera.up.z>-1.203</camera.up.z>
        </camera.up>
        <camera.right type="vec3">
            <camera.right.x>-1.203</camera.right.x>
            <camera.right.y>-1.203</camera.right.y>
            <camera.right.z>-1.203</camera.right.z>
        </camera.right>
</camera>

***********
**EXAMPLE**
***********

struct person
{
	string name;
	int dob;
	string email;
};
 
using people = vector<person>;


*******
**XML**
*******
- item is good for an array of items. (Model, Mesh, etc)

<people class_id="0" tracking_level="0" version="1">
	<count>2</count>
	<item_version>1</item_version>
	<item class_id="1" tracking_level="0" version="1">
		<person.name>Martin Vorbrodt</person.name>
		<person.dob>19800830</person.dob>
		<person.email>martin@vorbrodt.blog</person.email>
	</item>
	<item>
		<person.name>Dorota Vorbrodt</person.name>
		<person.dob>19810127</person.dob>
		<person.email>dorota@vorbrodt.blog</person.email>
	</item>
</people>

*/