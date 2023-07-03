#pragma once

#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
#include "state_class.hpp"

enum
{
    screen_width = 640,
    screen_height = 480
};

constexpr auto hex_height_coefficient = 0.78f;

#define RED 255, 0, 0, 255
#define GREEN 0, 255, 0, 255
#define BLUE 0, 0, 255, 255
#define BLACK 55, 55, 55, 255
#define DARK_RED 80, 0, 0, 255
#define DARK_BLUE 0, 0, 80, 255

class graphic_class
{
public:
    graphic_class(const std::string& graphic_folder, const std::string& font)
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

    ~graphic_class()
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

    bool update(game_class* game);

    [[nodiscard]]
    int mouse_get_q() const
    {
        return static_cast<int>(x_mouse_ / static_cast<float>(zoom_) -
                                y_mouse_ / (static_cast<float>(zoom_) * hex_height_coefficient * 2) +
                                static_cast<float>(y_shift_) * 0.65f -
                                static_cast<float>(x_shift_) + 1);
    }

    [[nodiscard]]
    int mouse_get_r() const
    {
        return static_cast<int>(static_cast<float>(y_mouse_) / (static_cast<float>(zoom_) * hex_height_coefficient) -
                                0.5f -
                                static_cast<float>(y_shift_) / hex_height_coefficient);
    }

private:
    void print(float x, float y, const char* text, const SDL_Color& text_color) const;
    void print_right(int width, float y, const char* text, const SDL_Color& text_color) const;
    SDL_Window* window_;
    SDL_Surface* screen_surface_;
    SDL_Renderer* render_;
    int zoom_ = 20;
    int x_shift_ = 0;
    int y_shift_ = 0;
    SDL_Texture* hex_passable_texture_ = nullptr;
    SDL_Texture* hex_blocked_texture_ = nullptr;
    float x_mouse_ = 0;
    float y_mouse_ = 0;
    TTF_Font* font_ = nullptr;
};
