#ifndef GAME_H
#define GAME_H

#include <random>
#include <atomic>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <queue>
#include "SDL.h"
#include "controller.h"
#include "renderer.h"
#include "snake.h"
#include "high_score_manager.h"

class Game {
 public:
  Game(std::size_t grid_width, std::size_t grid_height);
  void Run(Controller const &controller, Renderer &renderer,
           std::size_t target_frame_duration);
  int GetScore() const;
  int GetSize() const;
  void Stop(); // Method to stop the game loop

 private:
  Snake snake;
  SDL_Point food;
  std::random_device dev;
  std::mt19937 engine;
  std::uniform_int_distribution<int> random_w;
  std::uniform_int_distribution<int> random_h;
  int score{0};

  HighScoreManager high_score_manager;
  std::thread game_thread;
  std::thread render_thread;
  std::atomic<bool> running;
  std::mutex game_mutex;
  Renderer* renderer_ptr; // Pointer to the Renderer instance

  void PlaceFood();
  void Update();
  void GameLoop(std::size_t target_frame_duration);
  void RenderLoop();
  void PostToMainThread(std::function<void()> func); // Add this line
  void RunMainThreadTasks();

  // New members for managing main thread tasks
  std::queue<std::function<void()>> main_thread_queue;
  std::mutex queue_mutex;
  std::condition_variable queue_cv;
};

#endif
