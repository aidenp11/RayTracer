#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/color_space.hpp>

using color3_t = glm::vec3;
using color4_t = glm::vec4;
using rgba_t = uint32_t;

inline color4_t RGBAToColor(const rgba_t& rgba) 
{
    float r = glm::clamp(static_cast<float>((rgba >> 24) & 0xFF) / 255.0f, 0.0f, 1.0f);
    float g = glm::clamp(static_cast<float>((rgba >> 16) & 0xFF) / 255.0f, 0.0f, 1.0f);
    float b = glm::clamp(static_cast<float>((rgba >> 8) & 0xFF) / 255.0f, 0.0f, 1.0f);
    float a = glm::clamp(static_cast<float>(rgba & 0xFF) / 255.0f, 0.0f, 1.0f);
    return color4_t(r, g, b, a);
}

inline rgba_t ColorToRGBA(const color4_t& color) 
{
    uint8_t r = static_cast<uint8_t>(glm::clamp(color.r * 255.0f, 0.0f, 255.0f));
    uint8_t g = static_cast<uint8_t>(glm::clamp(color.g * 255.0f, 0.0f, 255.0f));
    uint8_t b = static_cast<uint8_t>(glm::clamp(color.b * 255.0f, 0.0f, 255.0f));
    uint8_t a = static_cast<uint8_t>(glm::clamp(color.a * 255.0f, 0.0f, 255.0f));

    return (static_cast<uint32_t>(r) << 24) | (static_cast<uint32_t>(g) << 16) | (static_cast<uint32_t>(b) << 8) | static_cast<uint32_t>(a);
}
