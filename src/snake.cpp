#include "snake.h"
#include <cmath>
#include <iostream>

// Constructor
// Initialize the snake at the center of the grid with initial settings
Snake::Snake(int grid_width, int grid_height)
  : grid_width(grid_width),
    grid_height(grid_height),
    head_x(grid_width/2),
    head_y(grid_height/2),
    direction(Direction::kUp),
    speed(0.1f),
    size(1),
    alive(true),
    growing(false) {}

// Update the snake's position and checks for collisions
void Snake::Update() {
  // Capture the current head position before updating
  SDL_Point prev_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};
  UpdateHead();
  // Capture the current head position after updating
  SDL_Point current_cell{
      static_cast<int>(head_x),
      static_cast<int>(head_y)};  

  // Update all of the body vector items if the snake head has moved to a new
  // cell.
  if (current_cell.x != prev_cell.x || current_cell.y != prev_cell.y) {
    UpdateBody(current_cell, prev_cell);
  }
}

// Update the snake's head position based on its direction
void Snake::UpdateHead() {
  switch (direction) {
    case Direction::kUp:
      head_y -= speed;
      break;

    case Direction::kDown:
      head_y += speed;
      break;

    case Direction::kLeft:
      head_x -= speed;
      break;

    case Direction::kRight:
      head_x += speed;
      break;
  }

  // Wrap the Snake around to the beginning if going off of the screen.
  head_x = fmod(head_x + grid_width, grid_width);
  head_y = fmod(head_y + grid_height, grid_height);
}

void Snake::UpdateBody(SDL_Point &current_head_cell, SDL_Point &prev_head_cell) {
  // Add previous head location to vector
  body.push_back(prev_head_cell);

  if (!growing) {
    // Remove the tail from the vector.
    body.erase(body.begin());
  } else {
    growing = false;
    size++;
  }

  // Check if the snake has died.
  for (auto const &item : body) {
    if (current_head_cell.x == item.x && current_head_cell.y == item.y) {
      alive = false;
    }
  }
}

void Snake::GrowBody() { growing = true; }

// Inefficient method to check if cell is occupied by snake.
bool Snake::SnakeCell(int x, int y) {
  if (x == static_cast<int>(head_x) && y == static_cast<int>(head_y)) {
    return true;
  }
  for (auto const &item : body) {
    if (x == item.x && y == item.y) {
      return true;
    }
  }
  return false;
}

// Getter methods
Snake::Direction Snake::GetDirection() const {
  return direction;
}

float Snake::GetSpeed() const {
  return speed;
}

int Snake::GetSize() const {
  return size;
}

bool Snake::IsAlive() const {
  return alive;
}

float Snake::GetHeadX() const {
  return head_x;
}

float Snake::GetHeadY() const {
  return head_y;
}

std::vector<SDL_Point> Snake::GetBody() const {
  return body;
}

// Setter methods
// Set the direction of the snake
void Snake::SetDirection(Direction direction) {
  this->direction = direction;
}

// Set the speed of the snake
// Throws the invalid_argument exception if the speed is non-positive
void Snake::SetSpeed(float speed) {
  if (speed <= 0) {
    throw std::invalid_argument("Speed must be positive");
  }
  this->speed = speed;
}
