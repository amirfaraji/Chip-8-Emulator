#include <iostream>
#include <unistd.h>
#include "emulator.hpp"
#include "SDL2/SDL.h"

using namespace std;
Emulator myC8;
int width = 1024, height = 512;

uint8_t keyboard_map[16] = {
    SDLK_x, // 0
    SDLK_1, // 1
    SDLK_2, // 2
    SDLK_3, // 3
    SDLK_q, // 4
    SDLK_w, // 5
    SDLK_e, // 6
    SDLK_a, // 7
    SDLK_s, // 8
    SDLK_d, // 9
    SDLK_z, // A
    SDLK_c, // B
    SDLK_4, // C
    SDLK_r, // D
    SDLK_f, // E
    SDLK_v  // F 
};

int main(){
    int pixels[2048];
    // SDL 
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Init(SDL_INIT_EVERYTHING);
    window = SDL_CreateWindow("CHIP-8 Emulator", SDL_WINDOWPOS_CENTERED,  SDL_WINDOWPOS_CENTERED,
                                width, height, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_Texture* texture;
    texture = SDL_CreateTexture(renderer,
            SDL_PIXELFORMAT_ARGB8888,
            SDL_TEXTUREACCESS_STREAMING,
            64, 32);


    myC8.loadGame("./invaders.c8");
    
    long int frame_speed, start_time;
    SDL_Event event;
    const int MILLISECONDS_PER_CYCLE = 3;

    while (true){
        start_time = SDL_GetTicks();
        myC8.emulationCycle();
        frame_speed = SDL_GetTicks() - start_time;
        if (frame_speed < MILLISECONDS_PER_CYCLE) {
            SDL_Delay(MILLISECONDS_PER_CYCLE - frame_speed);
        }
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT){
                SDL_DestroyRenderer(renderer);
                SDL_DestroyWindow(window);
                SDL_Quit();
                exit(0);
            }
            if (event.type == SDL_KEYDOWN){
                for (int i = 0; i < 16; ++i) {
                    if (event.key.keysym.sym == keyboard_map[i]) {
                        myC8.key[i] = 1;
                        //cout << i << keyboard_map[i] << endl;
                    }
                }
            }
            if (event.type == SDL_KEYUP){
                for (int i = 0; i < 16; ++i) {
                    if (event.key.keysym.sym == keyboard_map[i]) {
                        myC8.key[i] = 0;
                    }
                }
            }
        }
        if (myC8.drawFlag) {
            myC8.drawFlag = false;

            // Store pixels in temporary buffer
            for (int i = 0; i < 2048; i++) {
                uint8_t pixel = myC8.gfx[i];
                pixels[i] = (0x00FFFFFF * pixel) | 0xFF000000;
            }
            // Update SDL texture
            SDL_UpdateTexture(texture, NULL, pixels, 64 * sizeof(uint32_t));
            // Clear screen and render
            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, NULL, NULL);
            SDL_RenderPresent(renderer);
        }
        //usleep(1000);
    }
    return 0;
}

