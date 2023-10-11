#include <cmath>
#include <vector>
#include <string>
#include <sstream>
#include <GLFW/glfw3.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "hex_map.hpp"
#include "graphics.hpp"
#include "state_class.hpp"
#include "player_mcts.hpp"

using namespace std;

void graphic_class::print(const int x, const int y, const char* text, const SDL_Color& text_color, const SDL_Color& bg_color) const
{
    SDL_Rect mess_rect = {x, y, 0, 0};
    int w, h;
    SDL_Surface* mess_fps = TTF_RenderText_Shaded(font_, text, text_color, bg_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render_, mess_fps);
    TTF_SizeText(font_, text, &w, &h);
    mess_rect.w = w;
    mess_rect.h = h;
    SDL_RenderCopy(render_, texture, nullptr, &mess_rect);
    SDL_FreeSurface(mess_fps);
    SDL_DestroyTexture(texture);
}

void graphic_class::print_right(const int width, const int y, const char* text, const SDL_Color& text_color,
                                const SDL_Color& bg_color) const
{
    SDL_Rect mess_rect = {0, y, 0, 0};
    int w, h;
    SDL_Surface* mess_fps = TTF_RenderText_Shaded(font_, text, text_color, bg_color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(render_, mess_fps);
    TTF_SizeText(font_, text, &w, &h);
    mess_rect.x = width - w;
    mess_rect.w = w;
    mess_rect.h = h;
    SDL_RenderCopy(render_, texture, nullptr, &mess_rect);
    SDL_FreeSurface(mess_fps);
    SDL_DestroyTexture(texture);
}

void graphic_class::draw_tile(const hex_tile *hex) const
{
    SDL_Rect map_tile_rect;
    map_tile_rect.x = static_cast<int>((hex->graphic_x() - 0.5) * zoom_ + x_shift_ * zoom_);
    map_tile_rect.y =
        static_cast<int>((hex->graphic_y() - 0.5) * static_cast<double>(hex_height_coefficient) * zoom_ +
                                         y_shift_ * zoom_);
    map_tile_rect.w = zoom_;
    map_tile_rect.h = zoom_;
    if (hex->passable())
        SDL_RenderCopy(render_, hex_passable_texture_, nullptr, &map_tile_rect);
    else
        SDL_RenderCopy(render_, hex_blocked_texture_, nullptr, &map_tile_rect);
}

void graphic_class::draw_unit(unit_class& unit, const game_class *game) const
{
    SDL_Rect life_rect;
    life_rect.h = static_cast<int>(0.1 * zoom_);
    SDL_Rect unit_rect;
    unit_rect.w = static_cast<int>(zoom_ * 0.5);
    unit_rect.h = static_cast<int>(zoom_ * 0.5);

    if (unit.player_get() == 0)
        SDL_SetRenderDrawColor(render_, BLUE);
    else
        SDL_SetRenderDrawColor(render_, RED);
    const hex_tile* tile = game->map_get()->tile_get(unit.q_get(), unit.r_get());
    unit_rect.x = static_cast<int>(tile->graphic_x()) * zoom_ - unit_rect.w / 2 + x_shift_ * zoom_ + unit.player_get()*zoom_/5;
    unit_rect.y = static_cast<int>(unit.r_get() * hex_height_coefficient * zoom_ - unit_rect.h / 2) + y_shift_ * zoom_;
    SDL_RenderFillRect(render_, &unit_rect);

    life_rect.w = static_cast<int> (0.1f * unit_hp * zoom_);
    life_rect.x = static_cast<int> (unit_rect.x + unit_rect.w / 2 - life_rect.w / 2);
    life_rect.y = static_cast<int> (unit_rect.y + 0.7 * zoom_);
    SDL_RenderDrawRect(render_, &life_rect);

    life_rect.w = static_cast<int> (0.1f * unit.hp_get() * zoom_);
    SDL_RenderFillRect(render_, &life_rect);

    const unit_action action = unit.actual_action_get();
    if (target_debug_)
    {
        switch (action.action_type_get())
        {
        case unit_action_id::pick:
        case unit_action_id::attack: {
            object_abstract_class& enemy_u = game->state_get()->object_get(action.target_type_get(), action.target_player_get(), action.target_id_get());
            SDL_RenderDrawLine(
                render_, static_cast<int>((unit.position_get().graphic_x_get(game->map_get()) + x_shift_) * zoom_),
                static_cast<int>(unit.position_get().graphic_y_get(game->map_get()) * hex_height_coefficient *
                                    zoom_) +
                    y_shift_ * zoom_,
                enemy_u.position_get().graphic_x_get(game->map_get()) * zoom_ + x_shift_ * zoom_,
                (enemy_u.position_get().graphic_y_get(game->map_get()) * hex_height_coefficient) * zoom_ +
                    y_shift_ * zoom_);
        }
        break;
        default: ;
        }
    }
    
    if (path_debug_ && !unit.path_get()->empty())
    {
        hex_tile* precendent_tile = game->map_get()->tile_get(unit.position_get().q_get(), unit.position_get().r_get());
        for (auto tile : *(unit.path_get()))
        {
            SDL_RenderDrawLine(render_, 
                static_cast<int>(precendent_tile->graphic_x() * zoom_ + x_shift_ * zoom_), 
                static_cast<int>(precendent_tile->graphic_y() * hex_height_coefficient * zoom_ + y_shift_ * zoom_),
                static_cast<int>(tile->graphic_x() * zoom_ + x_shift_ * zoom_),
                static_cast<int>(tile->graphic_y() * hex_height_coefficient * zoom_ + y_shift_ * zoom_));
            precendent_tile = tile;
        }
    }
}

void graphic_class::draw_food(const food_class& food, const game_class* game) const
{
    SDL_Rect food_rect;
    food_rect.w = static_cast<int>(0.2 * zoom_);
    food_rect.h = static_cast<int>(0.2 * zoom_);
    SDL_SetRenderDrawColor(render_, GREEN);
    const hex_tile* tile = game->map_get()->tile_get(food.q_get(), food.r_get());
    food_rect.x = static_cast<int>(tile->graphic_x()) * zoom_ - food_rect.w / 2 + x_shift_ * zoom_;
    food_rect.y =
        static_cast<int>(tile->graphic_y() * hex_height_coefficient) * zoom_ - food_rect.h / 2 + y_shift_ * zoom_;
    SDL_RenderFillRect(render_, &food_rect);
    
}

void graphic_class::draw_base(const base_class& base, const game_class* game) const
{
    SDL_Rect life_rect;
    life_rect.h = static_cast<int>(0.1 * zoom_);
    SDL_Rect base_rect;
    base_rect.w = static_cast<int>(zoom_);
    base_rect.h = static_cast<int>(zoom_);
    SDL_SetRenderDrawColor(render_, BLACK);
    constexpr int bar_width = static_cast<int>(0.01 * base_hp);
    const hex_tile* tile = game->map_get()->tile_get(base.q_get(), base.r_get());
    base_rect.x = static_cast<int>(tile->graphic_x() * zoom_ - base_rect.w / 2 + x_shift_ * zoom_);
    base_rect.y =
        static_cast<int>(tile->graphic_y() * hex_height_coefficient * zoom_ - base_rect.h / 2 + y_shift_ * zoom_);
    if (base.player_id_get() == 0)
        SDL_SetRenderDrawColor(render_, DARK_BLUE);
    else
        SDL_SetRenderDrawColor(render_, DARK_RED);

    SDL_RenderFillRect(render_, &base_rect);

    life_rect.x = base_rect.x;
    life_rect.y = base_rect.y + static_cast<int> (1.5f * static_cast<float>(zoom_));
    life_rect.w = bar_width * zoom_;
    SDL_RenderDrawRect(render_, &life_rect);

    life_rect.w = static_cast<int>(0.01 * base.hp_get() * zoom_);
    SDL_RenderFillRect(render_, &life_rect);
}

bool graphic_class::event_handle(const game_class* game, options_class* settings)
{
    SDL_Event e;
    bool quit = false;
    while (SDL_PollEvent(&e))
    {
        if (e.type == SDL_QUIT)
            quit = true;
        if (e.type == SDL_MOUSEWHEEL)
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
        if (e.type == SDL_KEYDOWN)
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
            else if (e.key.keysym.sym == SDLK_SPACE)
            {
                settings->pause_toggle();
            }
            else if (e.key.keysym.sym == SDLK_p)
            {
                path_debug_ = !path_debug_;
            }
            else if (e.key.keysym.sym == SDLK_t)
            {
                target_debug_ = !target_debug_;
            }
        }
        if (e.type == SDL_MOUSEMOTION)
        {
            SDL_GetGlobalMouseState(&x_mouse_, &y_mouse_);
        }
        if (e.type == SDL_MOUSEBUTTONDOWN)
        {
            if (e.button.button == SDL_BUTTON_LEFT && game->map_get()->passable(mouse_get_q(), mouse_get_r()))
            {
                game->state_get()->unit_new(mouse_get_q(), mouse_get_r(), 0, unit_hp);
            }
            if (e.button.button == SDL_BUTTON_RIGHT && game->map_get()->passable(mouse_get_q(), mouse_get_r()))
            {
                game->state_get()->unit_new(mouse_get_q(), mouse_get_r(), 1, unit_hp);
            }
            if (e.button.button == SDL_BUTTON_MIDDLE && game->map_get()->passable(mouse_get_q(), mouse_get_r()))
            {
                game->state_get()->food_new(mouse_get_q(), mouse_get_r());
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
    SDL_Color text_black{0, 0, 0, 255};
    int window_w, window_h;

    SDL_GetWindowSize(window_, &window_w, &window_h);

    char arr_fps[20];
    if (sprintf_s(arr_fps, "%d", static_cast<int>(game->state_get()->fps_get())) == -1)
    {
        BOOST_LOG_TRIVIAL(error) << "printing";
    }
    print(2, 2, arr_fps, text_white, text_black);
    print_player(game->player_get(0), *game->state_get(), 22);
    print_player(game->player_get(1), *game->state_get(), 42);

    char arr_size[3];
    if (sprintf_s(arr_size, "%d", static_cast<int>(pointed_objects.size())) == -1)
    {
        BOOST_LOG_TRIVIAL(error) << "printing" ;
    }
    print(50, 2, arr_size, text_white, text_black);
    string pos_str;
    pos_str += to_string(mouse_get_q());
    pos_str += " ";
    pos_str += to_string(mouse_get_r());
    print_right(window_w, 0, pos_str.c_str(), text_white, text_black);
    if (map_class::in_map(mouse_get_q(), mouse_get_r(), game->map_get()))
    {
        pos_str = to_string(game->map_get()->tile_get(mouse_get_q(), mouse_get_r())->index_x());
        pos_str += " ";
        pos_str += to_string(game->map_get()->tile_get(mouse_get_q(), mouse_get_r())->index_y());
        print_right(window_w, 22, pos_str.c_str(), text_white, text_black);
    }
    int i = 4;
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
        if (sprintf_s(arr_info, "HP: %d ID: %d", obj->hp_get(), obj->id_get()) == -1)
        {
            BOOST_LOG_TRIVIAL(error) << "printing" ;
        }
        print_right(window_w, 2 + i * 20, arr_info, text_color, text_black);
        i++;
        if (sprintf_s(arr_info, "%d %d", obj->q_get(), obj->r_get()) == -1)
        {
            BOOST_LOG_TRIVIAL(error) << "printing";
        }
        print_right(window_w, 2 + i * 20, arr_info, text_color, text_black);
        i++;
        print_right(window_w, 2 + i * 20, path.c_str(), text_color, text_black);
        i += 2;
    }
}

void graphic_class::print_player(virtual_player_class& player, const state_class& state, const int y) const
{
    constexpr SDL_Color text_white{255, 255, 255, 255};
    constexpr SDL_Color text_black{0, 0, 0, 255};
    char arr_player[100];
    ostringstream player_string_stream;
    player_string_stream << "P" << player.player_id_get() << ": " << static_cast<int>(player.player_type_get())
                  << " eval: " << state.evaluate(player.player_id_get());
    if (player.player_type_get() == player_type::mcts)
    {
        player_string_stream << " trav:" << dynamic_cast<player_mcts&>(player).mcts_get().results_get().traversals;
        player_string_stream << "/" << dynamic_cast<player_mcts&>(player).mcts_get().results_get().traversals_max;
        player_string_stream << " time:" << dynamic_cast<player_mcts&>(player).mcts_get().results_get().time;
        player_string_stream << "/" << dynamic_cast<player_mcts&>(player).mcts_get().results_get().time_max;
        player_string_stream << " ticks:" << dynamic_cast<player_mcts&>(player).mcts_get().results_get().tick_max - state.frame_get();
        player_string_stream << " c:" << dynamic_cast<player_mcts&>(player).mcts_get().c_parameter_get();
    }
    print(2, y, player_string_stream.str().c_str(), text_white, text_black);
}

graphic_class::graphic_class(const std::string& graphic_folder, const std::string& font)
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "SDL could not initialize! SDL_Error:" << SDL_GetError();
    }
    constexpr int flags = IMG_INIT_PNG;
    if (const int init_status = IMG_Init(flags); (init_status & flags) != flags)
    {
        BOOST_LOG_TRIVIAL(error) << "SDL image not available";
    }
    window_ = SDL_CreateWindow("TIPE", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screen_width, screen_height,
                               SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
    screen_surface_ = SDL_GetWindowSurface(window_);
    render_ = SDL_CreateRenderer(window_, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    if (TTF_Init() < 0)
    {
        BOOST_LOG_TRIVIAL(error) << "SDL ttf not available";
    }
    font_ = TTF_OpenFont(font.c_str(), 20);
    if (font_ == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "can't open font file " << font;
    }

    const std::string hex_p = graphic_folder + "/hex_p.png";
    const std::string hex_b = graphic_folder + "/hex_b.png";
    SDL_Surface* hex_passable_surface = IMG_Load(hex_p.c_str());
    SDL_Surface* hex_blocked_surface = IMG_Load(hex_b.c_str());
    if (!hex_passable_surface)
    {
        BOOST_LOG_TRIVIAL(error) << "can't open " << hex_p << " file ";
    }
    if (!hex_blocked_surface)
    {
        BOOST_LOG_TRIVIAL(error) << "can't open " << hex_b << " file ";
    }
    hex_passable_texture_ = SDL_CreateTextureFromSurface(render_, hex_passable_surface);
    hex_blocked_texture_ = SDL_CreateTextureFromSurface(render_, hex_blocked_surface);
    SDL_FreeSurface(hex_passable_surface);
    SDL_FreeSurface(hex_blocked_surface);
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

bool graphic_class::update(const game_class* game, options_class* settings)
{
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
    vector<vector<unit_class>>& list_of_u_list = game->state_get()->unit_list_get();
    for (vector<unit_class>& unit_list : list_of_u_list)
    {
        for (unit_class& unit : unit_list)
        {
            if (unit.hp_get() > 0)
            {
                draw_unit(unit, game);
                if (unit.q_get() == mouse_get_q() && unit.r_get() == mouse_get_r())
                {
                    pointed_objects.push_back(&unit);
                }
            }
        }
    }

    /**
     * draw food
     * 
     */
    const vector<food_class>& food_list = game->state_get()->food_list_get();
    for (auto food : food_list)
    {
        draw_food(food, game);
        if (food.q_get() == mouse_get_q() && food.r_get() == mouse_get_r())
        {
            pointed_objects.push_back(&food);
        }
    }

    /**
     * draw bases
     * 
     */
    const vector<base_class>& base_list = game->state_get()->base_list_get();
    for (auto base : base_list)
    {
        draw_base(base, game);
        if (base.q_get() == mouse_get_q() && base.r_get() == mouse_get_r())
        {
            pointed_objects.push_back(&base);
        }
    }

    /**
     * Textes
     * 
     */
    print_screen(game, pointed_objects);
    SDL_SetRenderDrawColor(render_, BLACK); // background
    SDL_RenderPresent(render_);

    /**
     * Gerer la souris
     * 
     */
    const bool quit = event_handle(game, settings);
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
