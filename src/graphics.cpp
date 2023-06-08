#include <assert.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include "hex_map.hpp"
#include "graphics.hpp"
#include "state.hpp"

bool graphic::update(state &s){
    vector<vector<unit>> list_of_U_list = s.unitList_get();
    bool quit = false; 
    SDL_Event e;
    SDL_RenderClear(render);

    SDL_Surface* hexa_passable_surface = IMG_Load("hexa_p.png");
    SDL_Surface* hexa_blocked_surface = IMG_Load("hexa_b.png");
    if(!(hexa_passable_surface && hexa_blocked_surface)){
        cout << "hexa.png not found"<< endl;
    }
    SDL_Texture* hexa_passable_texture = SDL_CreateTextureFromSurface(render, hexa_passable_surface);
    SDL_Texture* hexa_blocked_texture = SDL_CreateTextureFromSurface(render, hexa_blocked_surface);

    SDL_FRect unitRect;
    unitRect.x = 0;
    unitRect.y = 0;
    unitRect.w = 0.5*zoom;
    unitRect.h = 0.5*zoom;

    float dec = 0.5;

    SDL_FRect map_tile_rect[Q_TAILLE][R_TAILLE];

    SDL_SetRenderDrawColor(render, RED);
    int x = 0,y = 0;
    for(auto col : s.map_get()->getTilesMap()){
        for(auto hex:col){
            map_tile_rect[x][y].x = (hex.xGraphic() - 0.5)*zoom + x_shift*zoom;
            map_tile_rect[x][y].y = (hex.yGraphic() - 0.5)*HEX_HEIGHT_COEFF*zoom + y_shift*zoom;
            map_tile_rect[x][y].w = 1 * zoom;
            map_tile_rect[x][y].h = 1 * zoom;
            if(hex.passable())
                SDL_RenderTexture(render, hexa_passable_texture, NULL, &map_tile_rect[x][y]);
            else
                SDL_RenderTexture(render, hexa_blocked_texture, NULL, &map_tile_rect[x][y]);
            y++;
        }
        x++;
        y=0;
    }

    SDL_SetRenderDrawColor(render, BLUE);
    hex_tile *tile;
    for(auto U_list: list_of_U_list){
        for(auto u:U_list){
            if(u.getHP() > 0){
                tile = s.map_get()->get_tile(u.getQ(),u.getR());
                unitRect.x = (tile->xGraphic())*zoom  - unitRect.w/2 + x_shift*zoom;
                unitRect.y = (tile->yGraphic() * HEX_HEIGHT_COEFF) * zoom  - unitRect.h/2 + y_shift*zoom;
                SDL_RenderFillRect(render, &unitRect);
            }
        }
        SDL_SetRenderDrawColor(render, RED);
    }
    for(int player = 0; player < NUMBER_OF_PLAYERS; player++){
        if(s.choosed_actions_get((PlayerID)player).size()){
            int action_index = 0;
            for(unitAction action:s.choosed_actions_get((PlayerID)player)){
                unit *u = action.unit_get();
                switch(action.actionType_get()){
                case uActionID::ATTACK:
                    {
                        unit *ennemyU = action.targetUnit_get();
                        double distance = u->position_get().distance(ennemyU->position_get(), s.map_get());
                        SDL_RenderLine( render,
                                        u->position_get().getXGraphic(s.map_get()),
                                        u->position_get().getYGraphic(s.map_get()),
                                        ennemyU->position_get().getXGraphic(s.map_get()),
                                        ennemyU->position_get().getYGraphic(s.map_get())); 
                    }
                    break;
                default:
                    cout << "Error : invalid action to do in non-zero vec" << endl;
                }
                action_index++;
            }
        }
    }
    
    SDL_SetRenderDrawColor(render, 0, 0, 0, 255);
    SDL_RenderPresent(render);
    SDL_UpdateWindowSurface(window);
    while( SDL_PollEvent(&e) ){ 
        if( e.type == SDL_EVENT_QUIT ) 
            quit = true;
        if( e.type == SDL_EVENT_MOUSE_WHEEL){
            if(e.wheel.y > 0){
                zoom++;
            }else if(e.wheel.y < 0){
                if(zoom-1 > 0)
                    zoom--;
            }
        }
        if( e.type == SDL_EVENT_KEY_DOWN ){
            if(e.key.keysym.sym == SDLK_RIGHT){
                x_shift--;
            }else if(e.key.keysym.sym == SDLK_LEFT){
                x_shift++;
            }else if(e.key.keysym.sym == SDLK_UP){
                y_shift++;
            }else if(e.key.keysym.sym == SDLK_DOWN){
                y_shift--;
            }
        }
        if( e.type == SDL_EVENT_MOUSE_MOTION ){
            SDL_GetGlobalMouseState(&xMouse,&yMouse);
        }
    }
    SDL_DestroySurface(hexa_passable_surface);
    SDL_DestroySurface(hexa_blocked_surface);
    SDL_DestroyTexture(hexa_passable_texture);
    SDL_DestroyTexture(hexa_blocked_texture);
    return quit;
}

void graphic::dessin(state &s, map_class &m, vector<hex_tile*> &tiles){
    float dec = 0.5;
    if(tiles.empty())
        return;
    float previousQ = tiles[0]->q() + dec * tiles[0]->r();
    float previousR = tiles[0]->r();

    SDL_FRect rect;
    rect.x = 0;
    rect.y = 0;
    rect.w = 2*ZOOM;
    rect.h = 2*ZOOM;

    SDL_SetRenderDrawColor(render, RED);
    for(auto col:m.getTilesMap()){
        for(auto hex:col){
            rect.x = (hex.q() + hex.r() * dec)*ZOOM - 0.5*ZOOM;
            rect.y = (hex.r())*ZOOM - 0.5*ZOOM;
            if(hex.passable())
                SDL_SetRenderDrawColor(render, GREEN);
            else
                SDL_SetRenderDrawColor(render, BLACK);

            SDL_RenderRect(render, &rect);
        }
    }
    SDL_SetRenderDrawColor(render, BLUE);
    for(auto t:tiles){
        float q = t->q() + t->r() * dec;
        float r = t->r();
        SDL_RenderLine(render, q*ZOOM, r*ZOOM, previousQ*ZOOM, previousR*ZOOM);
        previousQ = q;
        previousR = r;
    }
    SDL_RenderPresent(render);
    SDL_UpdateWindowSurface(window);

}