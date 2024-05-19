#include "Screen.h"

namespace AdhamIsmail {
Screen::Screen() :
		m_renderer(NULL), m_texture(NULL), m_window(NULL), m_buffer1(NULL), m_buffer2(
		NULL) {
}
bool Screen::init() {
	if (SDL_Init(SDL_INIT_VIDEO) < 0) {

		return false;
	}
	m_window = SDL_CreateWindow("Particle Fire Explosion",
	SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height,
			SDL_WINDOW_SHOWN);
	if (m_window == NULL) {
		SDL_Quit();
		return false;
	}

	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_PRESENTVSYNC);
	m_texture = SDL_CreateTexture(m_renderer, SDL_PIXELFORMAT_RGBA8888,
			SDL_TEXTUREACCESS_STATIC, width, height);

	if (m_renderer == NULL) {

		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}
	if (m_texture == NULL) {

		SDL_DestroyRenderer(m_renderer);
		SDL_DestroyWindow(m_window);
		SDL_Quit();
		return false;
	}
	m_buffer1 = new Uint32[width * height];
	m_buffer2 = new Uint32[width * height];

	memset(m_buffer1, 0, width * height * sizeof(Uint32));
	memset(m_buffer2, 0, width * height * sizeof(Uint32));

	return true;
}
bool Screen::processEvents() {
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT) {
			return false;
		}
	}
	return true;
}
void Screen::setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue) {
	if (x < 0 || x >= width || y < 0 || y >= height) {
		return;
	}

	Uint32 color = 0;

	color += red;
	color <<= 8;
	color += green;
	color <<= 8;
	color += blue;
	color <<= 8;
	color += 0xFF;

	//RGBA (Red, Green, Blue, Alpha)

	m_buffer1[(y * width) + x] = color;
}
void Screen::update() {
	SDL_UpdateTexture(m_texture, NULL, m_buffer1, width * sizeof(Uint32));
	SDL_RenderClear(m_renderer);
	SDL_RenderCopy(m_renderer, m_texture, NULL, NULL);
	SDL_RenderPresent(m_renderer);

}

void Screen::boxBlur() {
	//buffer swapping
	Uint32 *swap;
	swap = m_buffer2;
	m_buffer2 = m_buffer1;
	m_buffer1 = swap;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			/*
			 * 0 0 0
			 * 0 1 0
			 * 0 0 0
			 */
			int redTotal = 0;
			int greenTotal = 0;
			int blueTotal = 0;

			for (int row = -1; row <= 1; row++) {
				for (int col = -1; col <= 1; col++) {
					int currX = x + col;
					int currY = y + row;

					if (currX >= 0 && currX < width && currY >= 0
							&& currY < height) {
						Uint32 color = m_buffer2[(currY * width) + currX];

						Uint8 red = color >> 24;
						Uint8 green = color >> 16;
						Uint8 blue = color >> 8;

						redTotal += red;
						greenTotal += green;
						blueTotal += blue;
					}

				}
			}

			Uint8 red = redTotal / 9;
			Uint8 green = greenTotal / 9;
			Uint8 blue = blueTotal / 9;

			setPixel(x,y,red,green,blue);

		}
	}

}
void Screen::close() {
	delete[] m_buffer1;
	delete[] m_buffer2;
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyTexture(m_texture);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
}
}
