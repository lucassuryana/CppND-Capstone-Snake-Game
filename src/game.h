#ifndef GAME_H
#define GAME_H

#include <random>
#include <mutex>
#include <thread>
#include <condition_variable>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "high_score_manager.h"

class Game {
 public:
  // Constructor
  // Initialize the game by creating a grid with the size of grid times height, and setups the random number generator
  Game(std::size_t grid_width, std::size_t grid_height);
  // Runs the main game loop: handles input, updates game state, and renders the game
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  // Returns the current score of the game
  int GetScore() const;
  // Returns the current size of the snake
  int GetSize() const;

 private:
  Snake snake; // The snake objects representing the player's snake
  SDL_Point food; // The current position of the food
  SDL_Point bonus_food; // The current position of bonus food

  std::random_device dev; // Random device for seeding the random number generator
  std::mt19937 engine; // Mersenne Twister random number generator
  std::uniform_int_distribution<int> random_w; // Distribuion for random food placement on the x-axis
  std::uniform_int_distribution<int> random_h; // Distribution for random food placement on the y-axis
  std::mutex mutex; // Mutex 
  std::condition_variable condition_var;
  std::thread bonusFoodThread;

  int score{0}; // The current score of the game
  int count_place_food{0}; // The counter of how many time normal food is placed
  int bonus_food_remaining_time{0}; // Initialization of the bonus food remaining time
  bool is_bonus_food_active{false}; // The current status of bonus food

  // Places food at random location not occupied by the snake
  void PlaceFood();

  // Places bonus food at random location not occupied by the snake and normal food
  void PlaceBonusFood();

  // Timer before bonus food dissappear
  void BonusFoodTimer();

  // Updates the game state: moves the snake, check for collisions, and handles food consumption
  void Update();

  HighScoreManager high_score_manager{"highscores.txt"}; // Manages the high scores
};

#endif