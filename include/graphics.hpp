#pragma once

#include <assert.h>
#include <cmath>
#include <vector>
#include <iostream>
#include <SDL3/SDL.h>
#include <SDL3/SDL_image.h>
#include <SDL3/SDL_ttf.h>
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

class graphic
{
public:
    graphic()
    {
        //Initialize SDL
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
            exit(EXIT_FAILURE);
        }
        constexpr int flags = IMG_INIT_PNG;
        const int initStatus = IMG_Init(flags);
        if ((initStatus & flags) != flags)
        {
            cout << "SDL image not available" << endl;
            exit(EXIT_FAILURE);
        }
        window_ = SDL_CreateWindow("TIPE", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE | SDL_WINDOW_MAXIMIZED);
        screen_surface_ = SDL_GetWindowSurface(window_);
        render_ = SDL_CreateRenderer(window_, nullptr, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

        if (TTF_Init() < 0)
        {
            cout << "SDL ttf not available" << endl;
            exit(EXIT_FAILURE);
        }
        font_ = TTF_OpenFont("Anonymous_Pro.ttf", 20);
        if (font_ == nullptr)
        {
            cout << "no Anonymous_Pro.ttf file" << endl;
            exit(EXIT_FAILURE);
        }
        
        SDL_Surface* hex_passable_surface = IMG_Load("hexa_p.png");
        SDL_Surface* hex_blocked_surface = IMG_Load("hexa_b.png");
        if (!(hex_passable_surface && hex_blocked_surface))
        {
            cout << "hex.png not found" << endl;
        }
        hex_passable_texture_ = SDL_CreateTextureFromSurface(render_, hex_passable_surface);
        hex_blocked_texture_ = SDL_CreateTextureFromSurface(render_, hex_blocked_surface);
        SDL_DestroySurface(hex_passable_surface);
        SDL_DestroySurface(hex_blocked_surface);
    }

    void quit() const
    {
        TTF_CloseFont(font_);

        //Destroy window
        SDL_DestroyWindow(window_);

        //Quit SDL subsystems
        SDL_QuitSubSystem(SDL_INIT_VIDEO);
    }

    bool update(state& s);

    [[nodiscard]]
    int mouse_get_q() const
    {
        return x_mouse_ / zoom_ - y_mouse_ / (zoom_ * HEX_HEIGHT_COEFF * 2) - y_shift_ / (HEX_HEIGHT_COEFF * 2) +
               x_shift_ / zoom_ + 1;
    }

    [[nodiscard]]
    int mouse_get_r() const
    {
        return y_mouse_ / (zoom_ * HEX_HEIGHT_COEFF) - 0.5 - y_shift_ / HEX_HEIGHT_COEFF;
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
