#pragma once

#include "Object.h"

namespace CommonUtils
{

	//Generates a unique colour based on scalar parameter in the range of 0-1.
	// - Works off HSV color format, in this case Saturation and Value are 100% and Hue is dependant on 'scalar'
	Vector4 GenColour(float scalar, float alpha);

	//Generates a planet
	Object* Build_Planet_SphereObject(
		const std::string& name,
		const Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	Object* Build_atmosphere_SphereObject(
		const std::string& name,
		const Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a ball
	Object* Build_Ball_SphereObject(
		const std::string& name,
		const Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a default Sphere object with the parameters specified.
	Object* BuildSphereObject(
		const std::string& name,
		const Vector3& pos,
		float radius,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a default Cuboid object with the parameters specified
	Object* BuildCuboidObject(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	Object* Build_Quad_Object(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a raptor
	Object* Build_Raptor_Object(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a box
	Object* Build_Box_CuboidObject(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool collidable = true,				//requires physics_enabled = true
		bool dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));

	//Generates a box
	Object* Build_target_CuboidObject(
		const std::string& name,
		const Vector3& pos,
		const Vector3& scale,
		bool  physics_enabled = false,
		float inverse_mass = 0.0f,			//requires physics_enabled = true
		bool  collidable = true,				//requires physics_enabled = true
		bool  dragable = true,
		const Vector4& color = Vector4(1.0f, 1.0f, 1.0f, 1.0f));
};