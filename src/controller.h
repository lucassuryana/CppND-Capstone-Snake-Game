#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "snake.h"
#include <atomic>

class Controller {
 public:
  void HandleInput(bool &running, Snake &snake) const;

 private:
  void ChangeDirection(Snake &snake, Snake::Direction input,
                       Snake::Direction opposite) const;
};

#endif