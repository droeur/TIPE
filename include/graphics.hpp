#pragma once

#include <assert.h>
#include <cmath>
#include <iostream>
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include "hex_map.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

#define RED 249, 137, 137, 255
#define BLUE 137, 223, 249, 255

class graphic {
    public:
        graphic(){

            //Initialize SDL
            if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
            {
                printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
                exit(EXIT_FAILURE);
            }    
            window = SDL_CreateWindow( "SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
            screenSurface = SDL_GetWindowSurface(window);
            render = SDL_CreateRenderer(window, NULL, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        }

        void quit(){
            //Destroy window
            SDL_DestroyWindow(window);

            //Quit SDL subsystems
            SDL_QuitSubSystem(SDL_INIT_VIDEO);
        }
        void dessin(state &s, map_class &m, vector<hex_tile*> &tiles){
            float dec = 0.5;
            float previousQ = tiles[0]->q() + dec * tiles[0]->r();
            float previousR = tiles[0]->r();

            SDL_FRect rect;
            rect.x = 0;
            rect.y = 0;
            rect.w = 10;
            rect.h = 10;

            SDL_SetRenderDrawColor(render, RED);
            for(auto col:m.getTilesMap()){
                for(auto hex:col){
                    rect.x = (hex.q() + hex.r() * dec)*50.0 - 5.0;
                    rect.y = (hex.r())*50.0 - 5.0;
                    SDL_RenderRect(render, &rect);
                }
            }
            SDL_SetRenderDrawColor(render, BLUE);
            for(auto t:tiles){
                float q = t->q() + t->r() * dec;
                float r = t->r();
                SDL_RenderLine(render, q*50, r*50, previousQ*50, previousR*50);
                previousQ = q;
                previousR = r;
            }
            SDL_RenderPresent(render);
            SDL_UpdateWindowSurface(window);

        }
        bool update(state &s){
            vector<vector<unit>> list_of_U_list = s.unitList_get();
            bool quit = false; 
            SDL_Event e;
            SDL_RenderClear(render);

            SDL_FRect rect;
            rect.x = 0;
            rect.y = 0;
            rect.w = 10;
            rect.h = 10;


            SDL_SetRenderDrawColor(render, BLUE);
            for(auto U_list: list_of_U_list){
                for(auto u:U_list){
                    if(u.getHP() > 0){
                        rect.x = u.getX();
                        rect.y = u.getY();
                        SDL_RenderFillRect(render, &rect);
                    }
                }
                SDL_SetRenderDrawColor(render, RED);
            }
            
            SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
            SDL_RenderPresent(render);
            SDL_UpdateWindowSurface(window);
            while( SDL_PollEvent(&e) ){ 
                    if( e.type == SDL_EVENT_QUIT ) 
                        quit = true; 
            }
            return quit;
        }

    private:
        SDL_Window* window;
        SDL_Surface* screenSurface;
        SDL_Renderer *render;
};