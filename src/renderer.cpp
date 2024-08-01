#include "renderer.h"
#include <iostream>
#include <string>
#include <algorithm>  // For std::copy

// Custom deleter for SDL_Window
void SDLWindowDeleter(SDL_Window* window) {
    SDL_DestroyWindow(window);
}

// Custom deleter for SDL_Renderer
void SDLRendererDeleter(SDL_Renderer* renderer) {
    SDL_DestroyRenderer(renderer);
}

Renderer::Renderer(const std::size_t screen_width,
                   const std::size_t screen_height,
                   const std::size_t grid_width, const std::size_t grid_height)
    : screen_width(screen_width),
      screen_height(screen_height),
      grid_width(grid_width),
      grid_height(grid_height),
      sdl_window(nullptr, SDLWindowDeleter),
      sdl_renderer(nullptr, SDLRendererDeleter) {
  
  // Initialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    return;
  }

  // Create Window
  sdl_window.reset(SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                    SDL_WINDOWPOS_CENTERED, screen_width,
                                    screen_height, SDL_WINDOW_SHOWN));

  if (sdl_window == nullptr) {
    std::cerr << "Window could not be created.\n";
    std::cerr << " SDL_Error: " << SDL_GetError() << "\n";
    SDL_Quit();
    return;
  }

  // Create renderer
  sdl_renderer.reset(SDL_CreateRenderer(sdl_window.get(), -1, SDL_RENDERER_ACCELERATED));

  if (sdl_renderer == nullptr) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    SDL_DestroyWindow(sdl_window.release());
    SDL_Quit();
  }
}

// Destructor
Renderer::~Renderer() {
  // Smart pointers with custom deleters will automatically clean up resources
}

// Copy Constructor
Renderer::Renderer(const Renderer& other)
    : screen_width(other.screen_width),
      screen_height(other.screen_height),
      grid_width(other.grid_width),
      grid_height(other.grid_height),
      sdl_window(nullptr, SDLWindowDeleter),
      sdl_renderer(nullptr, SDLRendererDeleter) {
  // Initialize SDL
  std::cout << "Copy Operator called\n";
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    return;
  }

  // Create Window
  sdl_window.reset(SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN));

  if (!sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    SDL_Quit();
    return;
  }

  // Create renderer
  sdl_renderer.reset(SDL_CreateRenderer(sdl_window.get(), -1, SDL_RENDERER_ACCELERATED));
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    SDL_DestroyWindow(sdl_window.release());
    SDL_Quit();
  }
}

// Copy Assignment Operator
Renderer& Renderer::operator=(const Renderer& other) {
  std::cout << "Copy Assignment Operator called\n";
  if (this == &other) return *this;  // Self-assignment check

  // Clean up existing resources
  if (sdl_renderer) SDL_DestroyRenderer(sdl_renderer.release());
  if (sdl_window) SDL_DestroyWindow(sdl_window.release());

  // Copy state
  screen_width = other.screen_width;
  screen_height = other.screen_height;
  grid_width = other.grid_width;
  grid_height = other.grid_height;

  // Reinitialize SDL
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    std::cerr << "SDL could not initialize.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    return *this;
  }

  // Create Window
  sdl_window.reset(SDL_CreateWindow("Snake Game", SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED, screen_width,
                                screen_height, SDL_WINDOW_SHOWN));

  if (nullptr == sdl_window) {
    std::cerr << "Window could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    SDL_Quit();
    return *this;
  }

  // Create renderer
  sdl_renderer.reset(SDL_CreateRenderer(sdl_window.get(), -1, SDL_RENDERER_ACCELERATED));
  if (nullptr == sdl_renderer) {
    std::cerr << "Renderer could not be created.\n";
    std::cerr << "SDL_Error: " << SDL_GetError() << "\n";
    SDL_DestroyWindow(sdl_window.release());
    SDL_Quit();
  }

  return *this;
}

// Move Constructor
Renderer::Renderer(Renderer&& other) noexcept
    : screen_width(other.screen_width),
      screen_height(other.screen_height),
      grid_width(other.grid_width),
      grid_height(other.grid_height),
      sdl_window(std::move(other.sdl_window)),
      sdl_renderer(std::move(other.sdl_renderer)) {
  // Nullify the other object's pointers
  std::cout << "Move Constructor called\n";
  other.sdl_window = nullptr;
  other.sdl_renderer = nullptr;
}

// Move Assignment Operator
Renderer& Renderer::operator=(Renderer&& other) noexcept {
  std::cout << "Move Assignment called\n";
  if (this == &other) return *this;  // Self-assignment check

  sdl_window = std::move(other.sdl_window);
  sdl_renderer = std::move(other.sdl_renderer);

  screen_width = other.screen_width;
  screen_height = other.screen_height;
  grid_width = other.grid_width;
  grid_height = other.grid_height;

  return *this;
}

void Renderer::Render(Snake const snake, SDL_Point const &food, SDL_Point const &bonus_food, int &bonus_food_remaining_time) {
  SDL_Rect block;
  block.w = screen_width / grid_width;
  block.h = screen_height / grid_height;

  // Clear screen
  SDL_SetRenderDrawColor(sdl_renderer.get(), 0x1E, 0x1E, 0x1E, 0xFF);
  SDL_RenderClear(sdl_renderer.get());

  // Render food
  SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xCC, 0x00, 0xFF);
  block.x = food.x * block.w;
  block.y = food.y * block.h;
  SDL_RenderFillRect(sdl_renderer.get(), &block);

  // Render bonus food
  if (bonus_food.x != -1 && bonus_food.y != -1) { // Ensure bonus food is active
    // Determine blinking effect
    int blinkInterval = 200;
    if (bonus_food_remaining_time < 4) {
         if (SDL_GetTicks() / blinkInterval % 2 == 0) {
            SDL_SetRenderDrawColor(sdl_renderer.get(), 0x00, 0xFF, 0x00, 0xFF); // Green color for bonus food
            block.x = bonus_food.x * block.w;
            block.y = bonus_food.y * block.h;
            SDL_RenderFillRect(sdl_renderer.get(), &block); 
          }
    }
    else {
            SDL_SetRenderDrawColor(sdl_renderer.get(), 0x00, 0xFF, 0x00, 0xFF); // Green color for bonus food
            block.x = bonus_food.x * block.w;
            block.y = bonus_food.y * block.h;
            SDL_RenderFillRect(sdl_renderer.get(), &block); 
    }
  }

  // Render snake's body
  SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0xFF, 0xFF, 0xFF);
  for (SDL_Point const &point : snake.GetBody()) {
    block.x = point.x * block.w;
    block.y = point.y * block.h;
    SDL_RenderFillRect(sdl_renderer.get(), &block);
  }

  // Render snake's head
  block.x = static_cast<int>(snake.GetHeadX()) * block.w;
  block.y = static_cast<int>(snake.GetHeadY()) * block.h;
  if (snake.IsAlive()) {
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0x00, 0x7A, 0xCC, 0xFF);
  } else {
    SDL_SetRenderDrawColor(sdl_renderer.get(), 0xFF, 0x00, 0x00, 0xFF);
  }
  SDL_RenderFillRect(sdl_renderer.get(), &block);

  // Update Screen
  SDL_RenderPresent(sdl_renderer.get());
}

void Renderer::UpdateWindowTitle(int score, int fps) {
  std::string title{"Snake Score: " + std::to_string(score) + " FPS: " + std::to_string(fps)};
  SDL_SetWindowTitle(sdl_window.get(), title.c_str());
}
