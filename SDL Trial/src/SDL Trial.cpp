#include <iostream>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Screen.h"
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "Particle.h"
#include "Swarm.h"
using namespace std;
using namespace AdhamIsmail;

int main() {

	srand(time(NULL));

	Screen screen;

	if (screen.init() == false) {
		cout << "Error StartUp SDL" << endl;
	}

	Swarm swarm;
	while (true) {
		//update particles

		int elapsed = SDL_GetTicks();

		swarm.update(elapsed);

		unsigned char red = (unsigned char) (1 + sin(elapsed * 0.0001) * 128);
		unsigned char green = (unsigned char) (1 + sin(elapsed * 0.0002) * 128);
		unsigned char blue = (unsigned char) (1 + sin(elapsed * 0.0003) * 128);
		//draw particles

		const Particle *const pParticles = swarm.getParticles();

		for (int i = 0; i < Swarm::NPARTICLES; i++) {
			Particle particle = pParticles[i];

			int x = (particle.m_x + 1) * Screen::width / 2;
			int y = particle.m_y * Screen::width / 2 + Screen::height/2;

			screen.setPixel(x, y, red, green, blue);
		}
		screen.boxBlur();

		//check for events

		screen.update();

		if (!screen.processEvents()) {
			break;
		}

	}
	screen.close();

	return 0;
}
