#include "Scene.h"
#include "Canvas.h"
#include "MathUtils.h"
#include "Random.h"
#include "Mesh.h"
#include "Sphere.h"
#include "Plane.h"
#include <iostream>
#include <iomanip>

void Scene::Render(Canvas& canvas, int numSamples, int depth)
{
	// cast ray for each point (pixel) on the canvas
	for (int y = 0; y < canvas.GetSize().y; y++)
	{
		for (int x = 0; x < canvas.GetSize().x; x++)
		{
			// create vec2 pixel from canvas x,y
			glm::vec2 pixel = glm::vec2{ x, y };

			color3_t color{ 0 };
				// get normalized (0 - 1) point coordinates from pixel

			for (int i = 0; i < numSamples; i++)
			{
				glm::vec2 point = (pixel + glm::vec2{ random01(), random01() }) / glm::vec2(canvas.GetSize());
				// flip y
				point.y = 1.0f - point.y;

				// create ray from camera
				ray_t ray = m_camera->GetRay(point);

				// cast ray into scene
				// set color value from trace
				raycastHit_t raycastHit;
				color += Trace(ray, 0, 100, raycastHit, depth);
			}
				//color3_t color = Trace(ray);
			color /= numSamples;
			// draw color to canvas point (pixel)
			canvas.DrawPoint(pixel, color4_t(color, 1));
		}
		std::cout << std::setprecision(2) << std::setw(5) << (((float)y / (float)canvas.GetSize().y) * 100) << "%\n";
	}
}
//color3_t Scene::Trace(const ray_t& ray)
//{
//	glm::vec3 direction = glm::normalize(ray.direction);
//
//	// set scene sky color
//	float t = (direction.y + 1) * 0.5f; // direction.y (-1 <-> 1) => (0 <-> 1)
//	color3_t color = lerp(m_bottomColor, m_topColor, t);
//
//	return color;
//}

color3_t Scene::Trace(const ray_t& ray, float minDistance, float maxDistance, raycastHit_t& raycastHit, int depth)
{
	bool rayHit = false;
	float closestDistance = maxDistance;

	// check if scene objects are hit by the ray
	for (const auto &object : m_objects)
	{
		// when checking objects don't include objects farther than closest hit (starts at max distance)
		if (object->Hit(ray, minDistance, closestDistance, raycastHit))
		{
			rayHit = true;
			// set closest distance to the raycast hit distance (only hit objects closer than closest distance)
			closestDistance = raycastHit.distance;
		}
	}

	// if ray hit object, scatter (bounce) ray and check for next hit
	if (rayHit)
	{
		ray_t scattered;
		color3_t color;

		if (depth > 0 && raycastHit.material->Scatter(ray, raycastHit, color, scattered))
		{
			//return raycastHit.normal;
			return color * Trace(scattered, minDistance, maxDistance, raycastHit, depth - 1);
		}
		else
		{
			return raycastHit.material->GetEmissive();
		}
	}

	// if ray not hit, return scene sky color
	glm::vec3 direction = glm::normalize(ray.direction);
	float t = (direction.y + 1) * 0.5f; // direction.y (-1 <-> 1) => (0 <-> 1)
	color3_t color = lerp(m_bottomColor, m_topColor, t);

	return color;
}

void Scene::InitScene01(Scene& scene, const Canvas& canvas)
{
	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 1, 10 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, 20.0f, aspectRatio);

	auto lambertian = std::make_shared<Lambertian>(color3_t{ 0.4f });
	
	auto plane = std::make_unique<Plane>(glm::vec3{ 0, -0.2f, 0 }, glm::vec3{ 0, 1, 0 }, lambertian);
	scene.AddObject(std::move(plane));

	scene.SetCamera(camera);

	for (int x = -10; x < 10; x++)
	{
		for (int z = -10; z < 10; z++)
		{

			std::shared_ptr<Material> material;

			// create random material
			float r = random01();
			if (r < 0.3f)		material = std::make_shared<Lambertian>(glm::rgbColor(glm::vec3{ random(0, 360), 1.0f, 1.0f }));
			else if (r < 0.6f)	material = std::make_shared<Metal>(color3_t{ random(0.5f, 1.0f) }, random(0, 0.5f));
			else if (r < 0.9f)	material = std::make_shared<Dielectric>(color3_t{ 1.0f }, random(1.1f, 2));
			else				material = std::make_shared<Emissive>(glm::rgbColor(glm::vec3{ random(0, 360), 1.0f, 1.0f }), 5.0f);

			// set random sphere radius
			float radius = random(0.2f, 0.3f);
			// create sphere using random radius and material
			auto sphere = std::make_unique<Sphere>(glm::vec3{ x + random(-0.5f, 0.5f), radius, z + random(-0.5f, 0.5f) }, radius, material);
			// add sphere to the scene
			scene.AddObject(std::move(sphere));
		}
	}
}

void Scene::InitScene02(Scene& scene, const Canvas& canvas)
{
	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 1, 10 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, 20.0f, aspectRatio);

	scene.SetCamera(camera);

	auto mesh = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 0, 0, 1 }));
	mesh->Load("models/cube.obj", glm::vec3{ 0, 0.2f, 0 }, glm::vec3{ 0, 45, 0 });
	scene.AddObject(std::move(mesh));
}

void Scene::InitScene03(Scene& scene, const Canvas& canvas)
{
	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 1, 10 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, 20.0f, aspectRatio);

	scene.SetCamera(camera);

	auto floor = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 1, 1 }));
	floor->Load("models/quaad.obj", glm::vec3{ 0, 0.3, 7 }, glm::vec3{ -95, 0, 0 }, glm::vec3{ 1.1, 2, 1 });
	scene.AddObject(std::move(floor));

	auto ceiling = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 1, 1 }));
	ceiling->Load("models/quaad.obj", glm::vec3{ 0, 1.1, 7 }, glm::vec3{ 85, 0, 0 }, glm::vec3{ 1.1, 3.2, 1 });
	scene.AddObject(std::move(ceiling));

	auto backwall = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 1, 1 }));
	backwall->Load("models/quaad.obj", glm::vec3{ 0, 0.5, 6 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 2, 1.55, 1 });
	scene.AddObject(std::move(backwall));

	auto leftwall = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 1, 0, 0 }));
	leftwall->Load("models/quaad.obj", glm::vec3{ -0.53, 0.4, 7 }, glm::vec3{ 0, 90, 0 }, glm::vec3{ 2, 1.55, 1 });
	scene.AddObject(std::move(leftwall));

	auto rightwall = std::make_unique<Mesh>(std::make_shared<Lambertian>(color3_t{ 0, 1, 0 }));
	rightwall->Load("models/quaad.obj", glm::vec3{ 0.53, 0.4, 7 }, glm::vec3{ 0, -90, 0 }, glm::vec3{ 2, 1.55, 1 });
	scene.AddObject(std::move(rightwall));

	auto light = std::make_unique<Mesh>(std::make_shared<Emissive>(color3_t{ 1, 1, 1 }, 4));
	light->Load("models/quaad.obj", glm::vec3{ 0, 1.1, 7 }, glm::vec3{ 85, 0, 0 }, glm::vec3{ 0.3, 0.3, 0.3 });
	scene.AddObject(std::move(light));

	auto box = std::make_unique<Mesh>(std::make_shared<Dielectric>(color3_t{ 1 }, 1.5f));
	box->Load("models/cube.obj", glm::vec3{ -0.2, 0.3, 6.6 }, glm::vec3{ 0, 30, 0 }, glm::vec3{ 0.3, 0.7, 0.3 });
	scene.AddObject(std::move(box));

	auto ball = std::make_unique<Sphere>(glm::vec3{ 0.2, 0.46, 7.2}, 0.15, std::make_shared<Metal>(color3_t{ 0.5 }, 0.1f));
	scene.AddObject(std::move(ball));
}

