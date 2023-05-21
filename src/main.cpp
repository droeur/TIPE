#include <iostream>
#include <vector>
#include <rapidcsv.h>
#include <memory>
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <stdio.h>

#include "game.hpp"
#include "player.hpp"
#include "player_random.hpp"

using namespace std;

#include "graphics.hpp"

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

int main() {

    // Load map
    rapidcsv::Document map_doc("map.csv", rapidcsv::LabelParams(-1, -1));
    vector<vector<tile>> map;
    for(int i = 0; i < map_doc.GetColumnCount(); i++){
        vector<tile> column;
        for(int j:map_doc.GetColumn<int>(i)) {
            tile t{(bool)j};
            column.push_back(t);
        }
        map.push_back(column);
    }
    // Print map
    for(vector<tile> i:map){
        for(tile j:i){
            cout << j.passable << " ";
        }
        cout << endl;
    }

    game g;
    state s{map, &g};
    vector<vector<unitAction>(*)(state *s, vector<unit> U)> P;
    unit u1{0,0};
    unit u2{1,1};
    vector<unit> unitlist1{u1};
    vector<unit> unitlist2{u2};
    s.unitList_add(unitlist1);
    s.unitList_add(unitlist2);
    // while (!glfwWindowShouldClose(window)) {
    //     g.play(&s, P);
    //     draw_screen(s, window);
    // }

    // graphic_exit();
    //The window we'll be rendering to
    SDL_Window* window = NULL;
    
    //The surface contained by the window
    SDL_Surface* screenSurface = NULL;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
    }    
    else
    {
        //Create window
        window = SDL_CreateWindow( "SDL Tutorial", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);
        if( window == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
        }
        else
        {
            //Get window surface
            screenSurface = SDL_GetWindowSurface( window );

            //Fill the surface white
            SDL_FillSurfaceRect( screenSurface, NULL, SDL_MapRGB( screenSurface->format, 0x00, 0xFF, 0xFF ) );
            
            //Update the surface
            SDL_UpdateWindowSurface( window );

            //Hack to get window to stay up
            SDL_Event e; bool quit = false; while( quit == false ){ while( SDL_PollEvent( &e ) ){ if( e.type == SDL_EVENT_QUIT ) quit = true; } }
        }
    }
    //Destroy window
    SDL_DestroyWindow( window );

    //Quit SDL subsystems
    SDL_QuitSubSystem(SDL_INIT_VIDEO);


    return 0;

}