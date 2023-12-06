#include "Random.h"
#include "Scene.h"
#include "Renderer.h"
#include "Canvas.h"
#include "Camera.h"
#include "Material.h"
#include "Sphere.h"
#include "Plane.h"
#include "Triangle.h"
#include "Mesh.h"
#include <iostream>
#include <memory>
#include <SDL.h>

int main(int argc, char* argv[])
{
	std::cout << "Hello World\n";

	const int width = 400;
	const int height = 300;
	const int samples = 1;
	const int depth = 5;

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("Fart Window", width, height);

	Canvas canvas(width, height, renderer);
	Scene scene(glm::vec3{ 1.0f }, glm::vec3{ 0.5f, 0.7f, 1.0f }); // sky color could be set with the top and bottom color

	scene.InitScene01(scene, canvas);

	canvas.Clear({ 0, 0, 0, 1 });
	scene.Render(canvas, samples, depth);
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