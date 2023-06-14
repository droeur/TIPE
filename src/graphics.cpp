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
    bool quit = false; 
    SDL_Event e;
    SDL_RenderClear(render);

    /**
     * Drawing map
     * 
     */
    SDL_Surface* hexa_passable_surface = IMG_Load("hexa_p.png");
    SDL_Surface* hexa_blocked_surface = IMG_Load("hexa_b.png");
    if(!(hexa_passable_surface && hexa_blocked_surface)){
        cout << "hexa.png not found"<< endl;
    }
    SDL_Texture* hexa_passable_texture = SDL_CreateTextureFromSurface(render, hexa_passable_surface);
    SDL_Texture* hexa_blocked_texture = SDL_CreateTextureFromSurface(render, hexa_blocked_surface);

    SDL_FRect map_tile_rect[Q_TAILLE][R_TAILLE];

    SDL_SetRenderDrawColor(render, RED);
    int x = 0,y = 0;
    for(auto col : s.map_get()->getTilesMap()){
        for(auto hex:col){
            if(hex.q() != getMouseQ() || hex.r() != getMouseR()){
                map_tile_rect[x][y].x = (hex.xGraphic() - 0.5)*zoom + x_shift*zoom;
                map_tile_rect[x][y].y = (hex.yGraphic() - 0.5)*HEX_HEIGHT_COEFF*zoom + y_shift*zoom;
                map_tile_rect[x][y].w = 1 * zoom;
                map_tile_rect[x][y].h = 1 * zoom;
                if(hex.passable())
                    SDL_RenderTexture(render, hexa_passable_texture, NULL, &map_tile_rect[x][y]);
                else
                    SDL_RenderTexture(render, hexa_blocked_texture, NULL, &map_tile_rect[x][y]);
            }
            y++;
        }
        x++;
        y=0;
    }


    /**
     * Draw units
     * 
     */
    SDL_FRect unitRect;
    unitRect.x = 0;
    unitRect.y = 0;
    unitRect.w = 0.5*zoom;
    unitRect.h = 0.5*zoom;
    SDL_SetRenderDrawColor(render, BLUE);
    vector<vector<unit>> list_of_U_list = s.unitList_get();
    hex_tile *tile;
    for(auto U_list: list_of_U_list){
        for(auto u:U_list){
            if(u.getHP() > 0){
                tile = s.map_get()->get_tile(u.getQ(),u.getR());
                unitRect.x = (tile->xGraphic())* zoom  - unitRect.w/2 + x_shift*zoom;
                unitRect.y = (tile->yGraphic() * HEX_HEIGHT_COEFF) * zoom  - unitRect.h/2 + y_shift*zoom;
                SDL_RenderFillRect(render, &unitRect);
            }
        }
        SDL_SetRenderDrawColor(render, RED);
    }
    
    SDL_SetRenderDrawColor(render, BLUE);
    for(int player = 0; player < NUMBER_OF_PLAYERS; player++){
        if(s.choosed_actions_get((PlayerID)player).size()){
            int action_index = 0;
            for(unitAction action:s.choosed_actions_get((PlayerID)player)){
                unit *u = action.unit_get();
                switch(action.actionType_get()){
                case uActionID::ATTACK:
                    {
                        unit *ennemyU = action.targetUnit_get();
                        SDL_RenderLine( render,
                                        u->position_get().getXGraphic(s.map_get())* zoom  - unitRect.w/2 + x_shift*zoom,
                                        (u->position_get().getYGraphic(s.map_get()) * HEX_HEIGHT_COEFF) * zoom  - unitRect.h/2 + y_shift*zoom,
                                        ennemyU->position_get().getXGraphic(s.map_get())* zoom  - unitRect.w/2 + x_shift*zoom,
                                        (ennemyU->position_get().getYGraphic(s.map_get()) * HEX_HEIGHT_COEFF) * zoom  - unitRect.h/2 + y_shift*zoom); 
                    }
                    break;
                default:
                    cout << "Error : invalid action to do in non-zero vec" << endl;
                }
                action_index++;
            }
        }
        SDL_SetRenderDrawColor(render, RED);
    }
    

    /**
     * draw food
     * 
     */
    SDL_SetRenderDrawColor(render, GREEN);
    vector<food_class>* food_list =  s.foodList_get();
    for(food_class food: *food_list){
        tile = s.map_get()->get_tile(food.getQ(),food.getR());
        unitRect.x = (tile->xGraphic())* zoom  - unitRect.w/2 + x_shift*zoom;
        unitRect.y = (tile->yGraphic() * HEX_HEIGHT_COEFF) * zoom  - unitRect.h/2 + y_shift*zoom;
        SDL_RenderFillRect(render, &unitRect);
    }


    /**
     * draw bases
     * 
     */
    SDL_SetRenderDrawColor(render, BLACK);
    vector<base_class>* base_list =  s.baseList_get();
    for(base_class base: *base_list){
        tile = s.map_get()->get_tile(base.getQ(),base.getR());
        unitRect.x = (tile->xGraphic())* zoom  - unitRect.w/2 + x_shift*zoom;
        unitRect.y = (tile->yGraphic() * HEX_HEIGHT_COEFF) * zoom  - unitRect.h/2 + y_shift*zoom;
        SDL_RenderFillRect(render, &unitRect);
    }



    /**
     * Gerer la souris
     * 
     */
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
        if( e.type == SDL_EVENT_MOUSE_BUTTON_DOWN ){
            if(e.button.button == SDL_BUTTON_LEFT && s.map_get()->passable(getMouseQ(), getMouseR())){
                unit u{getMouseQ(), getMouseR(), (PlayerID)0, 100};
                s.unit_append(u, (PlayerID)0);
            }
            if(e.button.button == SDL_BUTTON_RIGHT && s.map_get()->passable(getMouseQ(), getMouseR())){
                unit u{getMouseQ(), getMouseR(), (PlayerID)1, 100};
                s.unit_append(u, (PlayerID)1);
            }
            if(e.button.button == SDL_BUTTON_MIDDLE && s.map_get()->passable(getMouseQ(), getMouseR())){
                food_class food{getMouseQ(), getMouseR()};
                s.food_append(food);
            }
        }
    }
    SDL_DestroySurface(hexa_passable_surface);
    SDL_DestroySurface(hexa_blocked_surface);
    SDL_DestroyTexture(hexa_passable_texture);
    SDL_DestroyTexture(hexa_blocked_texture);
    return quit;
}