#pragma once
#include <glm/glm.hpp>

template <typename T>
inline T lerp(const T& a, const T& b, float t)
{
	return static_cast<T>(a * (1 - t) + (t * b));
}

inline glm::vec3 cross(const glm::vec3& v1, const glm::vec3& v2) {
	return glm::cross(v1, v2);
}