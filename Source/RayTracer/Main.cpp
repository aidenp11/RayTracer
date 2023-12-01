#include <iostream>
#include "Renderer.h"
#include "Random.h"
#include "Canvas.h"

int main(int argc, char* argv[])
{
	std::cout << "Hello World\n";

	Renderer renderer;
	renderer.Initialize();
	renderer.CreateWindow("FartRenderer", 400, 300);

	Canvas canvas(400, 300, renderer);

	bool quit = false;
	while (!quit)
	{
		SDL_Event event;
		SDL_PollEvent(&event);
		switch (event.type)
		{
		case SDL_QUIT:
			quit = true;
			break;
		}

		canvas.Clear({ 0, 0, 0, 1 });
		for (int i = 0; i < 1000; i++) canvas.DrawPoint({ random(0, canvas.GetSize().x), random(0, canvas.GetSize().y)}, {random(0, 255), random(0, 255), random(0, 255), 1});
		canvas.Update();

		renderer.PresentCanvas(canvas);
	}

	renderer.Shutdown();

	return 0;
}