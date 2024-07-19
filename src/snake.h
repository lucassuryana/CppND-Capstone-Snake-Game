#ifndef SNAKE_H
#define SNAKE_H

#include <vector>
#include "SDL.h"

class Snake {
 public:
  // Enum to represent the direction of the snake's movement
  enum class Direction { kUp, kDown, kLeft, kRight };

  // Constructor
  // Initializes the snake at the center of the grid 
  Snake(int grid_width, int grid_height);

  // Updates the snake's position and check for collisions
  void Update();

  // Sets the snake's length to grow by one whenever the snake eats food
  void GrowBody();

  // Check if a given cell is occupied by the snake
  bool SnakeCell(int x, int y);

  // Public getter methods

  // Gets the current direction of the snake
  Direction GetDirection() const;

  // Gets the current speed of the snake
  float GetSpeed() const;

  // Get the current size of the snake
  int GetSize() const;

  // Checks if the snake is alive
  bool IsAlive() const;

  // Get the x-coordinate of the snake
  float GetHeadX() const;

  // Get the y-coordinate of the snake
  float GetHeadY() const;

  // Get the body of the snake
  std::vector<SDL_Point> GetBody() const;

  // Public setter methods

  // Set the direction of the snake
  void SetDirection(Direction direction);

  // Set the speed of the snake
  // Throws invalid_argument if speed is non-positive
  void SetSpeed(float speed);

  private:
    // Update the position of the snake's head based on its direction
    void UpdateHead();
    // Update the snake's body to follow the head and checks for collisions
    void UpdateBody(SDL_Point &current_cell, SDL_Point &prev_cell);

    Direction direction; // The current direction of the snake
    float speed; // The speed at which the snake moves
    int size; // The current size of the snake
    bool alive; // The alive status of the snake
    float head_x; // The x-coordinate of the snake's head
    float head_y; // The y-coordinate of the snake's head
    std::vector<SDL_Point> body; // The current body of the snake
    bool growing; // Indicates if the snake will grow in the next update
    int grid_width; // The width of the grid
    int grid_height; // The height of the grid
};

#endif