#include "Random.h"
#include "Scene.h"
#include "Renderer.h"
#include "Canvas.h"
#include "Camera.h"
#include "Material.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include <iostream>
#include <memory>
#include <SDL.h>

int main(int argc, char* argv[])
{
	std::cout << "Hello World\n";

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("Fart Window", 400, 300);

	Canvas canvas(400, 300, renderer);

	float aspectRatio = canvas.GetSize().x / (float)canvas.GetSize().y;
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 1, 10 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, 20.0f, aspectRatio);

	Scene scene(8, glm::vec3{ 1.0f }, glm::vec3{ 0.5f, 0.7f, 1.0f }); // sky color could be set with the top and bottom color
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


	//auto material = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });

	// create material
	/*auto lambertian = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	auto metal = std::make_shared<Metal>(color3_t{ 1, 1, 1 }, 0.0f);*/

	//std::shared_ptr<Material> pmaterial;

	//pmaterial = std::make_shared<Lambertian>(color3_t{ 0.2f });
	//auto triangle = std::make_unique<Triangle>(glm::vec3{ -1, 0, 0 }, glm::vec3{ 1, 0, 0 }, glm::vec3{ 0, 2, 0 }, std::make_shared<Lambertian>(color3_t{ 1, 0, 0 }));
	//scene.AddObject(std::move(triangle));
	//auto plane = std::make_unique<Plane>(glm::vec3{ 0, -0.2f, 0 }, glm::vec3{ 0, 1, 0 }, pmaterial);
	//scene.AddObject(std::move(plane));

	//// create objects -> add to scene
	//for (int i = 0; i < 10; i++)
	//{
	//	std::shared_ptr<Material> material = rand() % 2 ? std::dynamic_pointer_cast<Material>(lambertian) : std::dynamic_pointer_cast<Material>(metal);
	//	auto sphere = std::make_unique<Sphere>(glm::vec3{ random(-10.5f, 10.5f), random(0.2f, 0.3f), random(-10.5f, 10.5f) }, random(0.3f, 0.6f), material);
	//	scene.AddObject(std::move(sphere));
	//}

	/*for (int i = 0; i < 100; i++)
	{
		auto sphere = std::make_unique<Sphere>(glm::vec3{ random(-10.5f, 10.5f), random(-1.0f, 2.0f), random(-10.5f, 10.5f) }, random(0.4f, 0.7f), material);
		scene.AddObject(std::move(sphere));
	}*/

	canvas.Clear({ 0, 0, 0, 1 });
	scene.Render(canvas, 100);
	canvas.Update();

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_KEYDOWN:
			switch (event.key.keysym.sym)
			{
			case SDLK_ESCAPE:
				quit = true;
				break;
			}
			break;
		case SDL_QUIT:
			quit = true;
			break;
		}

		renderer.PresentCanvas(canvas);
	}

	renderer.Shutdown();

	return 0;
}