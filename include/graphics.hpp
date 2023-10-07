#pragma once

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "state_class.hpp"
#include "parser.hpp"

enum
{
    screen_width = 640,
    screen_height = 480
};

constexpr auto hex_height_coefficient = 0.78;

#define RED 255, 0, 0, 255
#define GREEN 0, 255, 0, 255
#define BLUE 0, 0, 255, 255
#define BLACK 55, 55, 55, 255
#define DARK_RED 80, 0, 0, 255
#define DARK_BLUE 0, 0, 80, 255

class graphic_class
{
    void print(int x, int y, const char* text, const SDL_Color& text_color) const;
    void print_right(int width, int y, const char* text, const SDL_Color& text_color) const;
    void draw_tile(const hex_tile* hex) const;
    void draw_unit(unit_class& unit, const game_class* game) const;
    void draw_food(const food_class& food, const game_class* game) const;
    void draw_base(const base_class& base, const game_class* game) const;
    bool event_handle(const game_class* game, options_class* settings);
    void print_screen(const game_class* game, const std::vector<object_abstract_class*>& pointed_objects) const;
    SDL_Window* window_;
    SDL_Surface* screen_surface_;
    SDL_Renderer* render_;
    int zoom_ = 20;
    int x_shift_ = 0;
    int y_shift_ = 0;
    SDL_Texture* hex_passable_texture_ = nullptr;
    SDL_Texture* hex_blocked_texture_ = nullptr;
    int x_mouse_ = 0;
    int y_mouse_ = 0;
    TTF_Font* font_ = nullptr;
    bool path_debug_ = false;
    bool target_debug_ = false;

public:
    graphic_class(const std::string& graphic_folder, const std::string& font);
    ~graphic_class();

    bool update(const game_class* game, options_class* settings);

    [[nodiscard]] int mouse_get_q() const;
    [[nodiscard]] int mouse_get_r() const;
};
