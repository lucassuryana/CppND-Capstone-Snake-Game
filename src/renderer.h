#ifndef RENDERER_H
#define RENDERER_H

#include <vector>
#include "SDL.h"
#include "snake.h"
#include <memory>

class Renderer {
public:
    Renderer(const std::size_t screen_width, const std::size_t screen_height,
             const std::size_t grid_width, const std::size_t grid_height);
    ~Renderer();

    Renderer(const Renderer& other);  // Copy constructor
    Renderer& operator=(const Renderer& other);  // Copy assignment operator
    Renderer(Renderer&& other) noexcept;  // Move constructor
    Renderer& operator=(Renderer&& other) noexcept;  // Move assignment operator

    void Render(Snake const snake, SDL_Point const &food, SDL_Point const&bonus_food, int &bonus_food_remaining_time);
    void UpdateWindowTitle(int score, int fps);

private:
    std::unique_ptr<SDL_Window, void(*)(SDL_Window*)> sdl_window;
    std::unique_ptr<SDL_Renderer, void(*)(SDL_Renderer*)> sdl_renderer;
    std::size_t screen_width;
    std::size_t screen_height;
    std::size_t grid_width;
    std::size_t grid_height;
};

#endif
