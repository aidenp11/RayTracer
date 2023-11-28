#pragma once
#include <glm/glm.hpp>


struct ray_t
{
	ray_t() = default;
	ray_t(const glm::vec3& origin, const glm::vec3& direction) :
		origin{ origin },
		direction{ direction }
	{}

	//<return point> GetPoint(float distance) const { <return point> }

	//glm::vec3 GetPoint(float distance) const { return GetPoint(distance); }

	glm::vec3 At(float t) const { return origin + (direction * t); }

	glm::vec3 origin{ 0 };
	glm::vec3 direction{ 0 };
};