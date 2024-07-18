#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake {
 public:
  enum class Direction { kUp, kDown, kLeft, kRight };

  Snake(int grid_width, int grid_height);

  void Update();
  void GrowBody();
  bool SnakeCell(int x, int y);

  // Public getter methods
  Direction GetDirection() const;
  float GetSpeed() const;
  int GetSize() const;
  bool IsAlive() const;
  float GetHeadX() const;
  float GetHeadY() const;
  std::vector<SDL_Point> GetBody() const;

  // Public setter methods
  void SetDirection(Direction direction);
  void SetSpeed(float speed);
  void SetSize(int size);
  void SetAlive(bool alive);
  void SetHeadX(float head_x);
  void SetHeadY(float head_y);

  private:
    void UpdateHead();
    void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

    Direction direction;
    float speed;
    int size;
    bool alive;
    float head_x;
    float head_y;
    std::vector<SDL_Point> body;
    bool growing;
    int grid_width;
    int grid_height;
};

#endif