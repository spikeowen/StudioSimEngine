#pragma once
#include "BasicIncludes.h"

namespace QuackEngine {

	struct BoundingBox
	{
		Vector3 center;
		Vector3 size;
		BoundingBox()
		{
			center.x = 0;
			center.y = 0;
			center.z = 0;
			size.x = 1;
			size.y = 1;
			size.z = 1;
		}
		BoundingBox(Vector3 position, Vector3 size)
		{
			center = position;
			this->size = size;
		}
	};

	struct BoundingSphere
	{
		BoundingSphere()
		{
			center.x = 0;
			center.y = 0;
			center.z = 0;
			radius = 0;
		}

		BoundingSphere(Vector3 position, float radius)
		{
			center = position;
			this->radius = radius;
		}

		Vector3 center;
		float radius;
	};
}