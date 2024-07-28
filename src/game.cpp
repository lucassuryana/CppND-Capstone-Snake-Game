#include "game.h"
#include <iostream>
#include "SDL.h"
#include "high_score_manager.h"
#include <dispatch/dispatch.h>

// Constructor
Game::Game(std::size_t grid_width, std::size_t grid_height)
    : snake(grid_width, grid_height),
      engine(dev()),
      random_w(0, static_cast<int>(grid_width - 1)),
      random_h(0, static_cast<int>(grid_height - 1)),
      high_score_manager("highscores.txt"),
      running(true),
      renderer_ptr(nullptr) {
  PlaceFood();
  high_score_manager.LoadHighScores();
}

// Runs the main game loop: handles input, updates game state, and renders the game
void Game::Run(Controller const &controller, Renderer &renderer,
               std::size_t target_frame_duration) {
  renderer_ptr = &renderer; // Set the renderer pointer to the passed renderer

  // Start the game update and rendering threads
  game_thread = std::thread(&Game::GameLoop, this, target_frame_duration);
  render_thread = std::thread(&Game::RenderLoop, this);

  while (running) {
    controller.HandleInput(running, snake);

    if (!running) {
      std::string player_name;
      std::cout << "Enter your name: ";
      std::cin >> player_name;
      high_score_manager.UpdateHighScores(player_name, score);
      high_score_manager.PrintHighScores();
      high_score_manager.SaveHighScores();
    }
  }

  // Ensure threads are joined before exiting
  if (game_thread.joinable()) game_thread.join();
  if (render_thread.joinable()) render_thread.join();
}

void Game::GameLoop(std::size_t target_frame_duration) {
  Uint32 frame_start;
  Uint32 frame_end;
  Uint32 frame_duration;
  int frame_count = 0;
  Uint32 title_timestamp = SDL_GetTicks();

  while (running) {
    frame_start = SDL_GetTicks();

    {
      std::lock_guard<std::mutex> lock(game_mutex);
      Update(); //Update the game state
    }

    frame_end = SDL_GetTicks();

    frame_count++;
    frame_duration = frame_end - frame_start;

    if (frame_end - title_timestamp >= 1000) {
      // Post the title update to the main thread
      PostToMainThread([this, frame_count]() {
        std::lock_guard<std::mutex> lock(game_mutex);
        if (renderer_ptr) {
          renderer_ptr->UpdateWindowTitle(score, frame_count);
        }
      });
      frame_count = 0;
      title_timestamp = frame_end;
    }

    if (frame_duration < target_frame_duration) {
      SDL_Delay(target_frame_duration - frame_duration);
    }
  }
}

void Game::RenderLoop() {
  while (running) {
    {
      std::lock_guard<std::mutex> lock(game_mutex);
      if (renderer_ptr) {
        renderer_ptr->Render(snake, food); // Render the game
      }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(8)); // Roughly 60 FPS
  }
}

void Game::PlaceFood() {
  int x, y;
  while (true) {
    x = random_w(engine);
    y = random_h(engine);
    if (!snake.SnakeCell(x, y)) {
      food.x = x;
      food.y = y;
      return;
    }
  }
}

void Game::Update() {
  if (!snake.IsAlive()) return;

  snake.Update();

  int new_x = static_cast<int>(snake.GetHeadX());
  int new_y = static_cast<int>(snake.GetHeadY());

  if (food.x == new_x && food.y == new_y) {
    score++;
    PlaceFood();
    snake.GrowBody();
    snake.SetSpeed(snake.GetSpeed() + 0.02);
  }
}

int Game::GetScore() const { return score; }
int Game::GetSize() const { return snake.GetSize(); }

void Game::Stop() { running = false; }

// Utility function to post work to the main thread
void Game::PostToMainThread(std::function<void()> func) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        main_thread_queue.push(func);
    }
    queue_cv.notify_one();
}

void Game::RunMainThreadTasks() {
    std::function<void()> func;
    while (running) {
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            queue_cv.wait(lock, [this]{ return !main_thread_queue.empty(); });
            func = std::move(main_thread_queue.front());
            main_thread_queue.pop();
        }
        func();
    }
}

