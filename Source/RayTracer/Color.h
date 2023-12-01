#pragma once
#include <glm/glm.hpp>

typedef glm::vec3 color3_t;
typedef glm::vec4 color4_t;
typedef glm::uint32 rgba_t;

inline color4_t RGBAToColor(const rgba_t& rgba) 
{
    glm::vec4 color;
    color.r = static_cast<float>((rgba >> 24) & 0xFF) / 255.0f;
    color.g = static_cast<float>((rgba >> 16) & 0xFF) / 255.0f;
    color.b = static_cast<float>((rgba >> 8) & 0xFF) / 255.0f;
    color.a = static_cast<float>(rgba & 0xFF) / 255.0f;
    return color;
}

inline rgba_t ColorToRGBA(const color4_t& color) 
{
    glm::uint32 r = static_cast<glm::uint32>(color.r * 255.0f);
    glm::uint32 g = static_cast<glm::uint32>(color.g * 255.0f);
    glm::uint32 b = static_cast<glm::uint32>(color.b * 255.0f);
    glm::uint32 a = static_cast<glm::uint32>(color.a * 255.0f);

    return (r << 24) | (g << 16) | (b << 8) | a;
}
