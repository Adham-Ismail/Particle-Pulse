
#ifndef SCREEN_H_
#define SCREEN_H_
#include <SDL2/SDL.h>
#include <iostream>
namespace AdhamIsmail {

class Screen {
public:
	const static int width = 800;
	const static int height = 600;
private:
	SDL_Renderer *m_renderer;
	SDL_Texture *m_texture;
	SDL_Window *m_window;
	Uint32 *m_buffer1;
	Uint32 *m_buffer2;


public:
	Screen();
	bool init();
	void close();
	bool processEvents();
	void setPixel(int x, int y, Uint8 red, Uint8 green, Uint8 blue);
	void update();
	void boxBlur();
};

}

#endif
