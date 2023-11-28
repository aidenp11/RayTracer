#pragma once
#include <glm/glm.hpp>

typedef glm::vec3 color3_t;
typedef glm::vec4 color4_t;
typedef glm::uint32 rgba_t;

inline color4_t RGBAToColor(const rgba_t& rgba) {}
inline rgba_t ColorToRGBA(const color4_t& color) {}
