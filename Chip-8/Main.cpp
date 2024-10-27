#include <iostream>
#include "Chip8.h"
#include <chrono>
#include <thread>
#include "SDL2/SDL.h"



int WinMain() {
	Chip8 chip8 = Chip8();
    
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        std::cout << "hello\n";
        return 1;
    }
    SDL_Window* window = SDL_CreateWindow("simple window",SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 512, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        std::cout << "coudlnt be created\n";
        SDL_Quit();
        return 1;
    }

    SDL_Renderer* render = SDL_CreateRenderer(window, -1, 0); // what renders the new pixels to screen
    if (render == NULL) {
        std::cout << "coudlnt be created\n";
        SDL_Quit();
        return 1;
    }
    SDL_RenderSetLogicalSize(render, 1024, 512);
    SDL_Texture* sdlTexture = SDL_CreateTexture(render,
        SDL_PIXELFORMAT_ARGB8888,
        SDL_TEXTUREACCESS_STREAMING,
        64, 32);

    uint32_t pixels[2048];
	
	if (chip8.load("3-corax+.ch8")) {
		for (int i = 0; i < 1000; i++)
		{
			chip8.execution_cycle();
            if (chip8.drawFlag) {
                chip8.drawFlag = false;
                for (int i = 0; i < 2048; i++) {
                    uint8_t pixel =chip8.Display[i];
                    pixels[i] = (0x00FFFFFF * pixel) | 0xFF0000000;
                }
                SDL_UpdateTexture(sdlTexture, NULL, pixels, 64 * sizeof(Uint32));

                SDL_RenderClear(render); // clears screen?? or maybe just the renderer
                SDL_RenderCopy(render, sdlTexture, NULL, NULL);
                // Up until now everything was drawn behind the scenes.
                // This will show the new, red contents of the window.
                SDL_RenderPresent(render);//Updates the window
            }
			std::this_thread::sleep_for(std::chrono::microseconds(1000));
		}
	}
    SDL_Delay(5000);
    SDL_DestroyRenderer(render);
    SDL_DestroyTexture(sdlTexture);
    SDL_DestroyWindow(window);
    SDL_Quit();
	
	return 0;
}