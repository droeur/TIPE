#pragma once

#include <assert.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
#include "hex_map.hpp"
#include "state.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define ZOOM 1
#define HEX_HEIGHT_COEFF 0.78

#define RED 255, 0, 0, 255
#define GREEN 0, 255, 0, 255
#define BLUE 0, 0, 255, 255
#define BLACK 55, 55, 55, 255
#define DARK_RED 80, 0, 0, 255
#define DARK_BLUE 0, 0, 80, 255

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
        window = SDL_CreateWindow( "TIPE", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
        screenSurface = SDL_GetWindowSurface(window);
        render = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if(TTF_Init() < 0){
            cout << "SDL ttf not avaible" << endl;
            exit(EXIT_FAILURE);
        }
        font = TTF_OpenFont( "Anonymous_Pro.ttf", 20 );
        if( font == NULL ) { 
            cout << "no Anonymous_Pro.ttf file" << endl;
            exit(EXIT_FAILURE);
        } 

    }

    void quit(){
        TTF_CloseFont(font);
        
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
    void print(float x, float y, const char *text, SDL_Color &textColor);
    void printR(int width, float y, const char *text, SDL_Color &textColor);
    SDL_Window* window;
    SDL_Surface* screenSurface;
    SDL_Renderer *render;
    int zoom = 20;
    int x_shift = 0;
    int y_shift = 0;
    SDL_Texture* hexaImage;
    float xMouse;
    float yMouse;
    TTF_Font *font; 
};