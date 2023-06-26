#include <assert.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
#include "hex_map.hpp"
#include "graphics.hpp"
#include "state.hpp"

void graphic::print(float x, float y, const char *text, SDL_Color &textColor){
    SDL_FRect mess_rect = {x,y,0,0};
    int w,h;
    SDL_Surface *mess_FPS = TTF_RenderText_Solid(font, text, textColor ); 
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, mess_FPS);
    TTF_SizeText(font, text, &w, &h);
    mess_rect.w = w;
    mess_rect.h = h;
    SDL_RenderTexture(render, texture, NULL, &mess_rect);
    SDL_DestroySurface(mess_FPS); 
    SDL_DestroyTexture(texture);
}
void graphic::printR(int width, float y, const char *text, SDL_Color &textColor){
    SDL_FRect mess_rect = {0,y,0,0};
    int w,h;
    SDL_Surface *mess_FPS = TTF_RenderText_Solid(font, text, textColor ); 
    SDL_Texture *texture = SDL_CreateTextureFromSurface(render, mess_FPS);
    TTF_SizeText(font, text, &w, &h);
    mess_rect.x = width - w;
    mess_rect.w = w;
    mess_rect.h = h;
    SDL_RenderTexture(render, texture, NULL, &mess_rect);
    SDL_DestroySurface(mess_FPS); 
    SDL_DestroyTexture(texture);
}

bool graphic::update(state &s){
    bool quit = false; 
    SDL_Event e;
    vector<object_abstract_class*> pointed_objects;

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


    SDL_FRect lifeRect;
    lifeRect.h = 0.1*zoom;

    /**
     * Draw units
     * 
     */
    SDL_FRect unitRect;
    unitRect.x = 0;
    unitRect.y = 0;
    unitRect.w = 0.5*zoom;
    unitRect.h = 0.5*zoom;
    vector<vector<unit>> list_of_U_list = s.unitList_get();
    for(auto &U_list: list_of_U_list){
        for(auto &u:U_list){
            hex_tile *tile = NULL;
            if(u.getHP() > 0){
                if(u.getPlayer() == (PlayerID)0){
                    SDL_SetRenderDrawColor(render, BLUE);
                } else {
                    SDL_SetRenderDrawColor(render, RED);
                }
                tile = s.map_get()->get_tile(u.getQ(),u.getR());
                unitRect.x = (tile->xGraphic())* zoom  - unitRect.w/2 + x_shift*zoom;
                unitRect.y = (tile->yGraphic() * HEX_HEIGHT_COEFF) * zoom  - unitRect.h/2 + y_shift*zoom;
                SDL_RenderFillRect(render, &unitRect);

                lifeRect.w = 0.1*UNIT_HP*zoom;
                lifeRect.x = unitRect.x + unitRect.w/2 - lifeRect.w/2;
                lifeRect.y = unitRect.y + 0.7*zoom;
                SDL_RenderRect(render, &lifeRect);

                lifeRect.w = 0.1*u.getHP()*zoom;
                SDL_RenderFillRect(render, &lifeRect);
                if(u.getQ() == getMouseQ() && u.getR() == getMouseR()){
                    pointed_objects.push_back((object_abstract_class*)&u);
                }
            }
        }
    }
    
    SDL_SetRenderDrawColor(render, BLUE);
    for(int player = 0; player < NUMBER_OF_PLAYERS; player++){
        if(s.choosed_actions_get((PlayerID)player).size()){
            int action_index = 0;
            for(unitAction action:s.choosed_actions_get((PlayerID)player)){
                unit *u = action.unit_get();
                switch(action.actionType_get()){
                case uActionID::PICK:
                case uActionID::ATTACK:
                    {
                        object_abstract_class *ennemyU = action.targetUnit_get();
                        SDL_RenderLine( render,
                                        (u->position_get().getXGraphic(s.map_get()) + x_shift)* zoom,
                                        (u->position_get().getYGraphic(s.map_get()) * HEX_HEIGHT_COEFF) * zoom  + y_shift*zoom,
                                        ennemyU->position_get().getXGraphic(s.map_get())* zoom  + x_shift*zoom,
                                        (ennemyU->position_get().getYGraphic(s.map_get()) * HEX_HEIGHT_COEFF) * zoom  + y_shift*zoom); 
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
    SDL_FRect foodRect;
    foodRect.x = 0;
    foodRect.y = 0;
    foodRect.w = 0.2*zoom;
    foodRect.h = 0.2*zoom;
    SDL_SetRenderDrawColor(render, GREEN);
    vector<food_class>* food_list =  s.foodList_get();
    for(food_class &food: *food_list){
        hex_tile *tile = NULL;
        tile = s.map_get()->get_tile(food.getQ(),food.getR());
        foodRect.x = (tile->xGraphic())* zoom  - foodRect.w/2 + x_shift*zoom;
        foodRect.y = (tile->yGraphic() * HEX_HEIGHT_COEFF) * zoom  - foodRect.h/2 + y_shift*zoom;
        SDL_RenderFillRect(render, &foodRect);
        if(food.getQ() == getMouseQ() && food.getR() == getMouseR()){
            pointed_objects.push_back((object_abstract_class*)&food);
        }
    }


    /**
     * draw bases
     * 
     */
    SDL_FRect baseRect;
    baseRect.w = zoom;
    baseRect.h = zoom;
    SDL_SetRenderDrawColor(render, BLACK);
    vector<base_class>* base_list =  s.baseList_get();
    for(base_class &base: *base_list){
        hex_tile *tile = NULL;
        tile = s.map_get()->get_tile(base.getQ(),base.getR());
        baseRect.x = (tile->xGraphic())* zoom  - baseRect.w/2 + x_shift*zoom;
        baseRect.y = (tile->yGraphic() * HEX_HEIGHT_COEFF) * zoom  - baseRect.h/2 + y_shift*zoom;
        if(base.playerId_get() == (PlayerID)0)
            SDL_SetRenderDrawColor(render, DARK_BLUE);
        else
            SDL_SetRenderDrawColor(render, DARK_RED);

        SDL_RenderFillRect(render, &baseRect);
        
        lifeRect.x = baseRect.x;
        lifeRect.y = baseRect.y + 1.5*zoom;
        lifeRect.w = 0.01*BASE_HP*zoom;
        SDL_RenderRect(render, &lifeRect);

        lifeRect.w = 0.01*base.getHP()*zoom;
        SDL_RenderFillRect(render, &lifeRect);
        if(base.getQ() == getMouseQ() && base.getR() == getMouseR()){
            pointed_objects.push_back((object_abstract_class*)&base);
        }
    }

    /**
     * Textes
     * 
     */
    SDL_Color text_white{255, 255, 255, 255};
    SDL_Color text_red{255, 0, 0, 255};
    SDL_Color text_blue{0, 0, 255, 255};
    SDL_Color text_green{0, 255, 0, 255};
    int windowW, windowH;

    SDL_GetWindowSize(window, &windowW, &windowH);

    char arr_FPS[20];
    sprintf(arr_FPS, "%d", (int)s.fps_get());
    print(2, 2, arr_FPS, text_white);

    char arr_Size[3];
    sprintf(arr_Size, "%d", (int)pointed_objects.size());
    print(50, 2, arr_Size, text_white);
    int i = 0;
    for(object_abstract_class* obj:pointed_objects){
        char arr_info[20];
        SDL_Color text_color = text_white;
        string path;
        if(obj->getPlayer() == -1){
            text_color = text_green;
        } else if (obj->getPlayer() == 0) {
            text_color = text_blue;
        } else if (obj->getPlayer() == 1) {
            text_color = text_red;
        }
        if(obj->object_type_get() == object_type::UNIT){
            unit* u = (unit*) obj;
            vector<hex_tile*> *p = u->getPath();
            path += "[";
            path += to_string(p->size());
            path += "]: ";
            for(hex_tile *t:*p){
                path += to_string(t->q());
                path += " ";
                path += to_string(t->r());
                path += " / ";
            }
            sprintf(arr_info, "HP: %d carry: %s", u->getHP(), u->carry_food_get()? "true" : "false");
        } else {
            sprintf(arr_info, "HP: %d", obj->getHP());
        }
        printR(windowW, 2 + i * 20, arr_info, text_color);
        i++;
        sprintf(arr_info, "%d %d", (int)obj->getQ(), (int)obj->getR());
        printR(windowW, 2 + i * 20, arr_info, text_color);
        i++;
        printR(windowW, 2 + i * 20, path.c_str(), text_color);
        i+=2;
    }

    SDL_SetRenderDrawColor(render, BLACK); // background
    SDL_RenderPresent(render);
    SDL_UpdateWindowSurface(window);

    /**
     * Gerer la souris
     * 
     */
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
                unit u{getMouseQ(), getMouseR(), (PlayerID)0, UNIT_HP};
                s.unit_append(u, (PlayerID)0);
            }
            if(e.button.button == SDL_BUTTON_RIGHT && s.map_get()->passable(getMouseQ(), getMouseR())){
                unit u{getMouseQ(), getMouseR(), (PlayerID)1, UNIT_HP};
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