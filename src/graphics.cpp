#include <cassert>
#include <cmath>
#include <vector>
#include <GLFW/glfw3.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
#include "hex_map.hpp"
#include "graphics.hpp"
#include "state_class.hpp"

using namespace std;

void graphic_class::print(const float x, const float y, const char* text, const SDL_Color& text_color) const
{
    SDL_FRect mess_rect = {x, y, 0, 0};
    int w, h;
    SDL_Surface* mess_fps = TTF_RenderText_Solid(font_, text, text_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render_, mess_fps);
    TTF_SizeText(font_, text, &w, &h);
    mess_rect.w = static_cast<float>(w);
    mess_rect.h = static_cast<float>(h);
    SDL_RenderTexture(render_, texture, nullptr, &mess_rect);
    SDL_DestroySurface(mess_fps);
    SDL_DestroyTexture(texture);
}

void graphic_class::print_right(const int width, const float y, const char* text, const SDL_Color& text_color) const
{
    SDL_FRect mess_rect = {0, y, 0, 0};
    int w, h;
    SDL_Surface* mess_fps = TTF_RenderText_Solid(font_, text, text_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render_, mess_fps);
    TTF_SizeText(font_, text, &w, &h);
    mess_rect.x = static_cast<float>(width - w);
    mess_rect.w = static_cast<float>(w);
    mess_rect.h = static_cast<float>(h);
    SDL_RenderTexture(render_, texture, nullptr, &mess_rect);
    SDL_DestroySurface(mess_fps);
    SDL_DestroyTexture(texture);
}

void graphic_class::draw_tile(const hex_tile *hex) const
{
    SDL_FRect map_tile_rect;
    map_tile_rect.x = static_cast<float>((hex->graphic_x() - 0.5) * zoom_ + x_shift_ * zoom_);
    map_tile_rect.y = static_cast<float>((hex->graphic_y() - 0.5) * hex_height_coefficient * zoom_ +
                                         y_shift_ * zoom_);
    map_tile_rect.w = static_cast<float>(zoom_);
    map_tile_rect.h = static_cast<float>(zoom_);
    if (hex->passable())
        SDL_RenderTexture(render_, hex_passable_texture_, nullptr, &map_tile_rect);
    else
        SDL_RenderTexture(render_, hex_blocked_texture_, nullptr, &map_tile_rect);
}

void graphic_class::draw_unit(const unit_class* unit, const game_class *game) const
{
    SDL_FRect life_rect;
    life_rect.h = static_cast<float>(0.1 * zoom_);
    SDL_FRect unit_rect;
    unit_rect.w = static_cast<float>(zoom_ * 0.5);
    unit_rect.h = static_cast<float>(zoom_ * 0.5);

    if (unit->player_get() == 0)
        SDL_SetRenderDrawColor(render_, BLUE);
    else
        SDL_SetRenderDrawColor(render_, RED);
    const hex_tile* tile = game->map_get()->tile_get(unit->q_get(), unit->r_get());
    unit_rect.x = (tile->graphic_x()) * zoom_ - unit_rect.w / 2 + x_shift_ * zoom_ + unit->player_get()*zoom_/5;
    unit_rect.y =
        (unit->r_get() * hex_height_coefficient) * zoom_ - unit_rect.h / 2 + y_shift_ * zoom_;
    SDL_RenderFillRect(render_, &unit_rect);

    life_rect.w = 0.1 * unit_hp * zoom_;
    life_rect.x = unit_rect.x + unit_rect.w / 2 - life_rect.w / 2;
    life_rect.y = unit_rect.y + 0.7 * zoom_;
    SDL_RenderRect(render_, &life_rect);

    life_rect.w = 0.1 * unit->hp_get() * zoom_;
    SDL_RenderFillRect(render_, &life_rect);
}

void graphic_class::draw_food(const food_class* food, const game_class* game) const
{
    SDL_FRect food_rect;
    food_rect.w = static_cast<float>(0.2 * zoom_);
    food_rect.h = static_cast<float>(0.2 * zoom_);
    SDL_SetRenderDrawColor(render_, GREEN);
    const hex_tile* tile = game->map_get()->tile_get(food->q_get(), food->r_get());
    food_rect.x = (tile->graphic_x()) * zoom_ - food_rect.w / 2 + x_shift_ * zoom_;
    food_rect.y = (tile->graphic_y() * hex_height_coefficient) * zoom_ - food_rect.h / 2 + y_shift_ * zoom_;
    SDL_RenderFillRect(render_, &food_rect);
    
}

void graphic_class::draw_base(const base_class* base, const game_class* game) const
{
    SDL_FRect life_rect;
    life_rect.h = static_cast<float>(0.1 * zoom_);
    SDL_FRect base_rect;
    base_rect.w = static_cast<float>(zoom_);
    base_rect.h = static_cast<float>(zoom_);
    SDL_SetRenderDrawColor(render_, BLACK);
    constexpr int bar_width = 0.01 * base_hp;
    const hex_tile* tile = game->map_get()->tile_get(base->q_get(), base->r_get());
    base_rect.x = static_cast<float>(tile->graphic_x() * zoom_ - base_rect.w / 2 + x_shift_ * zoom_);
    base_rect.y =
        static_cast<float>(tile->graphic_y() * hex_height_coefficient * zoom_ - base_rect.h / 2 + y_shift_ * zoom_);
    if (base->player_id_get() == 0)
        SDL_SetRenderDrawColor(render_, DARK_BLUE);
    else
        SDL_SetRenderDrawColor(render_, DARK_RED);

    SDL_RenderFillRect(render_, &base_rect);

    life_rect.x = base_rect.x;
    life_rect.y = base_rect.y + 1.5f * static_cast<float>(zoom_);
    life_rect.w = static_cast<float>(bar_width * zoom_);
    SDL_RenderRect(render_, &life_rect);

    life_rect.w = static_cast<float>(0.01 * base->hp_get() * zoom_);
    SDL_RenderFillRect(render_, &life_rect);
}

bool graphic_class::event_handle(const game_class *game)
{
    SDL_Event e;
    bool quit = false;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_EVENT_QUIT)
            quit = true;
        if (e.type == SDL_EVENT_MOUSE_WHEEL)
        {
            if (e.wheel.y > 0)
            {
                zoom_++;
            }
            else if (e.wheel.y < 0)
            {
                if (zoom_ - 1 > 0)
                    zoom_--;
            }
        }
        if (e.type == SDL_EVENT_KEY_DOWN)
        {
            if (e.key.keysym.sym == SDLK_RIGHT)
            {
                x_shift_ -= 100 / zoom_;
            }
            else if (e.key.keysym.sym == SDLK_LEFT)
            {
                x_shift_ += 100 / zoom_;
            }
            else if (e.key.keysym.sym == SDLK_UP)
            {
                y_shift_ += 100 / zoom_;
            }
            else if (e.key.keysym.sym == SDLK_DOWN)
            {
                y_shift_ -= 100 / zoom_;
            }
        }
        if (e.type == SDL_EVENT_MOUSE_MOTION)
        {
            SDL_GetGlobalMouseState(&x_mouse_, &y_mouse_);
        }
        if (e.type == SDL_EVENT_MOUSE_BUTTON_DOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT && game->map_get()->passable(mouse_get_q(), mouse_get_r()))
            {
                const auto u = new unit_class{mouse_get_q(), mouse_get_r(), 0, unit_hp};
                game->state_get()->unit_append(u, 0);
            }
            if (e.button.button == SDL_BUTTON_RIGHT && game->map_get()->passable(mouse_get_q(), mouse_get_r()))
            {
                const auto u = new unit_class{mouse_get_q(), mouse_get_r(), 1, unit_hp};
                game->state_get()->unit_append(u, 1);
            }
            if (e.button.button == SDL_BUTTON_MIDDLE && game->map_get()->passable(mouse_get_q(), mouse_get_r()))
            {
                const auto food = new food_class{mouse_get_q(), mouse_get_r()};
                game->state_get()->food_append(food);
            }
        }
    }
    return quit;
}

void graphic_class::print_screen(const game_class* game, const vector<object_abstract_class*>& pointed_objects) const
{
    SDL_Color text_white{255, 255, 255, 255};
    SDL_Color text_red{255, 0, 0, 255};
    SDL_Color text_blue{0, 0, 255, 255};
    SDL_Color text_green{0, 255, 0, 255};
    int window_w, window_h;

    SDL_GetWindowSize(window_, &window_w, &window_h);

    char arr_fps[20];
    if (sprintf_s(arr_fps, "%d", static_cast<int>(game->state_get()->fps_get())) == -1)
    {
        cerr << "Error: printing" << endl;
    }
    print(2, 2, arr_fps, text_white);

    char arr_size[3];
    if (sprintf_s(arr_size, "%d", static_cast<int>(pointed_objects.size())) == -1)
    {
        cerr << "Error: printing" << endl;
    }
    print(50, 2, arr_size, text_white);
    string pos_str;
    pos_str += to_string(mouse_get_q());
    pos_str += " ";
    pos_str += to_string(mouse_get_r());
    print_right(window_w, 0, pos_str.c_str(), text_white);
    int i = 2;
    for (object_abstract_class* obj : pointed_objects)
    {
        char arr_info[250];
        SDL_Color text_color = text_white;
        string path;
        if (obj->player_get() == -1)
        {
            text_color = text_green;
        }
        else if (obj->player_get() == 0)
        {
            text_color = text_blue;
        }
        else if (obj->player_get() == 1)
        {
            text_color = text_red;
        }
        if (obj->object_type_get() == object_type::undefined)
        {
            auto* u = dynamic_cast<unit_class*>(obj);
            vector<hex_tile*>* p = u->path_get();
            path += "[";
            path += to_string(p->size());
            path += "]: ";
            for (hex_tile* t : *p)
            {
                path += to_string(t->q());
                path += " ";
                path += to_string(t->r());
                path += " / ";
            }
            if (sprintf_s(arr_info, "HP: %d carry: %g", u->hp_get(), u->carry_food_get() ? "true" : "false") == -1)
            {
                cerr << "Error printing" << endl;
            }
        }
        else
        {
            if (sprintf_s(arr_info, "HP: %d", obj->hp_get()) == -1)
            {
                cerr << "Error printing" << endl;
            }
        }
        print_right(window_w, static_cast<float>(2 + i * 20), arr_info, text_color);
        i++;
        if (sprintf_s(arr_info, "%d %d", obj->q_get(), obj->r_get()) == -1)
        {
            cerr << "Error printing" << endl;
        }
        print_right(window_w, static_cast<float>(2 + i * 20), arr_info, text_color);
        i++;
        print_right(window_w, static_cast<float>(2 + i * 20), path.c_str(), text_color);
        i += 2;
    }
}

graphic_class::graphic_class(const std::string& graphic_folder, const std::string& font)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "SDL could not initialize! SDL_Error:" << SDL_GetError();
    }
    constexpr int flags = IMG_INIT_PNG;
    if (const int init_status = IMG_Init(flags); (init_status & flags) != flags)
    {
        std::cerr << "SDL image not available" << std::endl;
    }
    window_ = SDL_CreateWindow("TIPE", screen_width, screen_height, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
    screen_surface_ = SDL_GetWindowSurface(window_);
    render_ = SDL_CreateRenderer(window_, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (TTF_Init() < 0)
    {
        std::cerr << "SDL ttf not available" << std::endl;
    }
    font_ = TTF_OpenFont(font.c_str(), 20);
    if (font_ == nullptr)
    {
        std::cerr << "Error opening " << font << " file" << std::endl;
    }

    const std::string hex_p = graphic_folder + "/hex_p.png";
    const std::string hex_b = graphic_folder + "/hex_b.png";
    SDL_Surface* hex_passable_surface = IMG_Load(hex_p.c_str());
    SDL_Surface* hex_blocked_surface = IMG_Load(hex_b.c_str());
    if (!hex_passable_surface)
    {
        std::cerr << "Error opening " << hex_p << " file " << std::endl;
    }
    if (!hex_blocked_surface)
    {
        std::cerr << "Error opening " << hex_b << " file " << std::endl;
    }
    hex_passable_texture_ = SDL_CreateTextureFromSurface(render_, hex_passable_surface);
    hex_blocked_texture_ = SDL_CreateTextureFromSurface(render_, hex_blocked_surface);
    SDL_DestroySurface(hex_passable_surface);
    SDL_DestroySurface(hex_blocked_surface);
}

graphic_class::~graphic_class()
{
    SDL_DestroyTexture(hex_passable_texture_);
    SDL_DestroyTexture(hex_blocked_texture_);

    SDL_DestroyRenderer(render_);

    TTF_CloseFont(font_);

    //Destroy window
    SDL_DestroyWindow(window_);

    //Quit SDL subsystems
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
}

bool graphic_class::update(const game_class* game)
{
    bool quit = false;
    vector<object_abstract_class*> pointed_objects;

    SDL_RenderClear(render_);
    /**
     * Drawing map
     * 
     */


    SDL_SetRenderDrawColor(render_, RED);
    int x = 0, y = 0;
    for (const auto row : *game->map_get()->tiles_map_get())
    {
        for (const auto hex : *row)
        {
            if (hex->q() != mouse_get_q() || hex->r() != mouse_get_r())
            {
                draw_tile(hex);
            }
            x++;
        }
        y++;
        x = 0;
    }


    /**
     * Draw units
     * 
     */
    const vector<vector<unit_class*>> list_of_u_list = game->state_get()->unit_list_get();
    for (auto& unit_list : list_of_u_list)
    {
        for (const auto& unit : unit_list)
        {
            if (unit->hp_get() > 0)
            {
                draw_unit(unit, game);
                if (unit->q_get() == mouse_get_q() && unit->r_get() == mouse_get_r())
                {
                    pointed_objects.push_back(unit);
                }
            }
        }
    }
    /*
    SDL_SetRenderDrawColor(render_, BLUE);
    for (int player = 0; player < NUMBER_OF_PLAYERS; player++)
    {
        if (!g->chosen_actions_get(player).empty())
        {
            int action_index = 0;
            for (unit_action action : g->chosen_actions_get(player))
            {
                unit* u = action.unit_get();
                switch (action.action_type_get())
                {
                case u_action_id::pick:
                case u_action_id::attack: {
                    object_abstract_class* enemy_u = action.target_unit_get();
                    SDL_RenderLine(render_,
                                   static_cast<float>((u->position_get().getXGraphic(g->map_get()) + x_shift_) * zoom_),
                                   static_cast<float>(
                                       u->position_get().getYGraphic(g->map_get()) * HEX_HEIGHT_COEFF * zoom_) +
                                   y_shift_ * zoom_,
                                   enemy_u->position_get().getXGraphic(g->map_get()) * zoom_ + x_shift_ * zoom_,
                                   (enemy_u->position_get().getYGraphic(g->map_get()) * HEX_HEIGHT_COEFF) * zoom_ +
                                   y_shift_ * zoom_);
                }
                break;
                }
                action_index++;
            }
        }
        SDL_SetRenderDrawColor(render_, RED);
    }
    */
    /**
     * draw food
     * 
     */
    const vector<food_class*>* food_list = game->state_get()->food_list_get();
    for (const auto food : *food_list)
    {
        draw_food(food, game);
        if (food->q_get() == mouse_get_q() && food->r_get() == mouse_get_r())
        {
            pointed_objects.push_back(food);
        }
    }

    /**
     * draw bases
     * 
     */
    const vector<base_class*>* base_list = game->state_get()->base_list_get();
    for (const auto base : *base_list)
    {
        draw_base(base, game);
        if (base->q_get() == mouse_get_q() && base->r_get() == mouse_get_r())
        {
            pointed_objects.push_back(base);
        }
    }

    /**
     * Textes
     * 
     */
    print_screen(game, pointed_objects);
    SDL_SetRenderDrawColor(render_, BLACK); // background
    SDL_RenderPresent(render_);
    SDL_UpdateWindowSurface(window_);

    /**
     * Gerer la souris
     * 
     */
    quit = event_handle(game);
    return quit;
}

int graphic_class::mouse_get_q() const
{
    return static_cast<int>(x_mouse_ / static_cast<float>(zoom_) -
                            y_mouse_ / (static_cast<float>(zoom_) * hex_height_coefficient * 2) +
                            static_cast<float>(y_shift_) * 0.65f -
                            static_cast<float>(x_shift_) + 1);
}

int graphic_class::mouse_get_r() const
{
    return static_cast<int>(static_cast<float>(y_mouse_) / (static_cast<float>(zoom_) * hex_height_coefficient) -
                            0.5f -
                            static_cast<float>(y_shift_) / hex_height_coefficient);
}
