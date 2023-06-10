#pragma once

#include <assert.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include "hex_map.hpp"
#include "state.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define ZOOM 1
#define HEX_HEIGHT_COEFF 0.78

#define RED 249, 137, 137, 255
#define BLUE 137, 223, 249, 255
#define GREEN 85, 180, 93, 255
#define BLACK 55, 55, 55, 255

class graphic {
public:
    graphic(){

        //Initialize SDL
        if( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
            printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
            exit(EXIT_FAILURE);
        }
        int flags = IMG_INIT_PNG;
        int initStatus = IMG_Init(flags);
        if((initStatus & flags) != flags){
            cout << "SDL image not avaible" << endl;
            exit(EXIT_FAILURE);
        }
        window = SDL_CreateWindow( "TIPE", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
        screenSurface = SDL_GetWindowSurface(window);
        render = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    }

    void quit(){
        //Destroy window
        SDL_DestroyWindow(window);

        //Quit SDL subsystems
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }
    
    void dessin(state &s, map_class &m, vector<hex_tile*> &tiles);
    bool update(state &s);

    int getMouseQ(){
        return xMouse/zoom - yMouse/(zoom*HEX_HEIGHT_COEFF*2) - y_shift/(HEX_HEIGHT_COEFF*2) - x_shift/zoom + 1;
    }
    int getMouseR(){
        return yMouse/(zoom*HEX_HEIGHT_COEFF) - 0.5 - y_shift/HEX_HEIGHT_COEFF;
    }
private:
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Renderer *render;
    int zoom = 5;
    int x_shift = 0;
    int y_shift = 0;
    SDL_Texture* hexaImage;
    float xMouse;
    float yMouse;
};