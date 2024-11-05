#include <iostream>
#include "Chip8.h"
#include <chrono>
#include <thread>
#include "SDL2/SDL.h"



int WinMain() {
	Chip8 chip8 = Chip8();
    uint8_t keymap[16] = {
    SDLK_x,
    SDLK_1,
    SDLK_2,
    SDLK_3,
    SDLK_q,
    SDLK_w,
    SDLK_e,
    SDLK_a,
    SDLK_s,
    SDLK_d,
    SDLK_z,
    SDLK_c,
    SDLK_4,
    SDLK_r,
    SDLK_f,
    SDLK_v,
    };
    
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
   
    bool ifTrue = true;
    const int timerFrequency = 1000 / 60;

	
	if (chip8.load("6-keypad.ch8")) {
        auto lastTimerUpdate = std::chrono::high_resolution_clock::now();
		while(ifTrue){
             SDL_Event e;
			chip8.execution_cycle();

            auto now = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastTimerUpdate).count() >= timerFrequency) {
                chip8.updateTimers();
                lastTimerUpdate = now;

            }

            while (SDL_PollEvent(&e) != 0) {
                if (e.type == SDL_QUIT) {
                    SDL_Quit();
                    ifTrue = false;
                    break;
                }
                 
                if (e.type == SDL_KEYDOWN) {
                    if (e.key.keysym.sym == SDLK_ESCAPE) {
                        SDL_Quit();
                        ifTrue = false;
                        break;
                    }
                       
                        
                    
                    for (int i = 0; i < 16; i++){
                        if (e.key.keysym.sym == keymap[i]) {
                            chip8.key[i] = 1;}}
                }// Ending of the SDL_KeyDown if statement

                if (e.type == SDL_KEYUP) {
                    for (int i = 0; i < 16; i++){
                        if (e.key.keysym.sym == keymap[i]) {
                            chip8.key[i] = 0; 
}
                    }
                }//End of the SDL_KeyUp if Statement

            }

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
        }// End of the DrawFlag if statement
			std::this_thread::sleep_for(std::chrono::microseconds(3000));
		}//End of the Execution Loop
	}//End the loaded if statement
    SDL_Delay(5000);
    SDL_DestroyRenderer(render);
    SDL_DestroyTexture(sdlTexture);
    SDL_DestroyWindow(window);
    SDL_Quit();
	
	return 0;
}