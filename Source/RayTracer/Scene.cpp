#include "Scene.h"
#include "Canvas.h"
#include "MathUtils.h"

void Scene::Render(Canvas& canvas)
{
	glm::ivec2 canvasSize = canvas.GetSize();
	// cast ray for each point (pixel) on the canvas
	for (int y = 0; y < canvasSize.y; y++)
	{
		for (int x = 0; x < canvasSize.x; x++)
		{
			// create vec2 pixel from canvas x,y
			glm::vec2 pixel = glm::vec2{ canvasSize.x, canvasSize.y };
				// get normalized (0 - 1) point coordinates from pixel
			glm::vec2 point = pixel / glm::vec2(canvasSize);
				// flip y
				point.y = 1.0f - point.y;

			// create ray from camera
			ray_t ray = m_camera->GetRay(point);

				// cast ray into scene
				// set color value from trace
				color3_t color = Trace(ray);

			// draw color to canvas point (pixel)
			canvas.DrawPoint(pixel, color4_t(color, 1));
		}
	}
}
color3_t Scene::Trace(const ray_t& ray)
{
	glm::vec3 direction = glm::normalize(ray.direction);

	// set scene sky color
	float t = (direction.y + 1) * 0.5f; // direction.y (-1 <-> 1) => (0 <-> 1)
	color3_t color = lerp(m_bottomColor, m_topColor, t);

	return color;
}