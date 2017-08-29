#pragma once

#include "../math/vector3.h"

namespace igad
{
	struct RaycastHit
	{
		RaycastHit() : 
			is_hit(false),
			distance(false),
			hit(Vector3())
		{
		
		}
		
		bool is_hit = false;
		float distance = 0;
		Vector3 hit = Vector3();
	};
}