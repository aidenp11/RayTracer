#include "Random.h"
#include "Scene.h"
#include "Renderer.h"
#include "Canvas.h"
#include "Camera.h"
#include "Material.h"
#include "Sphere.h"
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
	std::shared_ptr<Camera> camera = std::make_shared<Camera>(glm::vec3{ 0, 0, 1 }, glm::vec3{ 0, 0, 0 }, glm::vec3{ 0, 1, 0 }, 70.0f, aspectRatio);

	Scene scene(8); // sky color could be set with the top and bottom color
	scene.SetCamera(camera);

	//auto material = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });

	// create material
	auto lambertian = std::make_shared<Lambertian>(color3_t{ 0, 0, 1 });
	auto metal = std::make_shared<Metal>(color3_t{ 1, 1, 1 }, 0.0f);

	// create objects -> add to scene
	for (int i = 0; i < 100; i++)
	{
		std::shared_ptr<Material> material = rand() % 2 ? std::dynamic_pointer_cast<Material>(lambertian) : std::dynamic_pointer_cast<Material>(metal);
		auto sphere = std::make_unique<Sphere>(glm::vec3{ random(-10.5f, 10.5f), random(0.2f, 0.3f), random(-10.5f, 10.5f) }, random(0.3f, 0.6f), material);
		scene.AddObject(std::move(sphere));
	}

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