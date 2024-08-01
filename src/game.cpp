#include "game.h"
#include <iostream>
#include "SDL.h"
#include "high_score_manager.h"
#include <mutex>
#include <thread>
#include <chrono>
#include <condition_variable>

// Constructor
// Initializes the game with a grid of specified width and height, and sets up the random number generators
Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      high_score_manager("highscores.txt") {
  PlaceFood(); // Place the initial food
  high_score_manager.LoadHighScores(); // Load high scores from file
}

// Runs the main game loop: handles input, updates game state, and renders the game
void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  Uint32 title_timestamp = SDL_GetTicks();
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  bool running = true;

  while (running) {
    frame_start = SDL_GetTicks();

    // Input, Update, Render - the main game loop.
    controller.HandleInput(running, snake);
    Update();
    renderer.Render(snake, food, bonus_food, bonus_food_remaining_time);

    frame_end = SDL_GetTicks();

    // Keep track of how long each loop through the input/update/render cycle
    // takes.
    frame_count++;
    frame_duration = frame_end - frame_start;

    // After every second, update the window title.
    if (frame_end - title_timestamp >= 1000) {
      renderer.UpdateWindowTitle(score, frame_count);
      frame_count = 0;
      title_timestamp = frame_end;
    }

    // If the time for this frame is too small (i.e. frame_duration is
    // smaller than the target ms_per_frame), delay the loop to
    // achieve the correct frame rate.
    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }

    if (!running) {
      std::string player_name;
      std::cout << "Enter your name: ";
      std::cin >> player_name;
      high_score_manager.UpdateHighScores(player_name, score);
      high_score_manager.PrintHighScores();
      high_score_manager.SaveHighScores();
    }
  }
}

// Places food at random location not occupied by the snake
void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      count_place_food++;
      return;
    }
  }
}

// Places bonus food at random location not occupied by the snake and the normal food
void Game::PlaceBonusFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    // Check that the location is not occupied by a snake item before placing
    // food.
    if ((!snake.SnakeCell(x, y)) && !(food.x == x && food.y == y)) {
      bonus_food.x = x;
      bonus_food.y = y;
      return;
    }
  }
}

void Game::BonusFoodTimer() {
  const int bonusSeconds = 6;
  auto startTime = std::chrono::high_resolution_clock::now();
  std::unique_lock<std::mutex> lock(mutex);
  while (is_bonus_food_active) {
    auto current_Time = std::chrono::high_resolution_clock::now();
    auto elapsed_Seconds = std::chrono::duration_cast<std::chrono::seconds>(current_Time - startTime).count();

    if (elapsed_Seconds >= bonusSeconds) {
      // Bonus food time is up
      is_bonus_food_active = false;
      bonus_food.x = -1;  // Mark as not present
      bonus_food.y = -1;
      break;
    }

    bonus_food_remaining_time = bonusSeconds - static_cast<int>(elapsed_Seconds);
    condition_var.wait_for(lock, std::chrono::milliseconds(800));
  }
}

// Updates the game state: moves the snake, check for collisions, and handles food consumption
void Game::Update() {
  if (!snake.IsAlive()) return;

  snake.Update();

  int new_x = static_cast<int>(snake.GetHeadX());
  int new_y = static_cast<int>(snake.GetHeadY());

  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    
    if (count_place_food % 4 == 0) {
      std::lock_guard<std::mutex> guard(mutex);
      if (!is_bonus_food_active) {
        PlaceBonusFood();
        is_bonus_food_active = true;
        bonusFoodThread = std::thread(&Game::BonusFoodTimer, this);
        bonusFoodThread.detach();
      }
    }

    snake.GrowBody();
    snake.SetSpeed(snake.GetSpeed() + 0.02);
  }

  if (bonus_food.x == new_x && bonus_food.y == new_y) {
    score += 2 + bonus_food_remaining_time; // Bonus scores
    is_bonus_food_active = false;
    bonus_food.x = -1;
    bonus_food.y = -1;
  }
}


// Returns the current score of the game
int Game::GetScore() const { return score; }
// Returns the current size of the snake
int Game::GetSize() const { return snake.GetSize(); }