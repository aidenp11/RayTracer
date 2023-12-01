#pragma once
template <typename T>
inline T lerp(const T& a, const T& b, float t)
{
	return static_cast<T>(a * (1 - t) + (t * b));
}