# CPPND: Capstone Snake Game Example

This is a starter repo for the Capstone project in the [Udacity C++ Nanodegree Program](https://www.udacity.com/course/c-plus-plus-nanodegree--nd213). The code for this repo was inspired by [this](https://codereview.stackexchange.com/questions/212296/snake-game-in-c-with-sdl) excellent StackOverflow post and set of responses.

<img src="snake_game.gif"/>

The Capstone Project gives you a chance to integrate what you've learned throughout this program. This project will become an important part of your portfolio to share with current and future colleagues and employers.

In this project, you can build your own C++ application or extend this Snake game, following the principles you have learned throughout this Nanodegree Program. This project will demonstrate that you can independently create applications using a wide range of C++ features.

## Features

- **Snake Game**: A classic Snake game implemented using C++ and SDL2.
- **High Score Management**: Tracks and saves high scores using a `high_score_manager` component. High scores are stored in a file (`highscores.txt`) and are updated every time the player finishes a game. This feature helps keep track of the best scores and adds a competitive edge to the game.
- **Rule of Five Implementation**: The `Renderer` class implements the Rule of Five, managing resources properly with a destructor, copy constructor, copy assignment operator, move constructor, and move assignment operator. It utilizes smart pointers to handle resource management effectively.
- **Multithreading**: The game uses multithreading to separate game logic and rendering tasks. It creates and manages separate threads for updating game state and rendering graphics. Synchronization between threads is handled using mutexes to prevent data races and ensure thread safety.

## Dependencies for Running Locally

* cmake >= 3.7
  * All OSes: [click here for installation instructions](https://cmake.org/install/)
* make >= 4.1 (Linux, Mac), 3.81 (Windows)
  * Linux: make is installed by default on most Linux distros
  * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
  * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
* SDL2 >= 2.0
  * All installation instructions can be found [here](https://wiki.libsdl.org/Installation)
  >Note that for Linux, an `apt` or `apt-get` installation is preferred to building from source. 
* gcc/g++ >= 5.4
  * Linux: gcc / g++ is installed by default on most Linux distros
  * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
  * Windows: recommend using [MinGW](http://www.mingw.org/)

## Basic Build Instructions

1. Clone this repo.
2. Make a build directory in the top level directory: `mkdir build && cd build`
3. Compile: `cmake .. && make`
4. Run it: `./SnakeGame`.

## High Score Management

The game includes a `high_score_manager` that manages player scores. The `high_score_manager` does the following:
- **Load High Scores**: When the game starts, it loads previously saved high scores from `highscores.txt`.
- **Update High Scores**: After each game, the player's score is compared to the existing high scores. If the player achieves a new high score, it is updated in the file.
- **Save High Scores**: The updated high scores are saved back to `highscores.txt`, ensuring that the high scores persist across game sessions.
- **Display High Scores**: At the end of each game, the updated high scores are displayed to the player.

## Implementation Details

1. **Member Initialization Lists**: Class constructors utilize member initialization lists to initialize class members efficiently.
2. **Rule of Five**: The `Renderer` class implements the Rule of Five, using smart pointers to manage resources and ensure proper cleanup.
3. **Multithreading and Mutexes**: The game implements multithreading to handle game logic and rendering in separate threads. Mutexes are used to protect shared resources and ensure thread safety.

## License

CC Attribution-ShareAlike 4.0 International

Shield: [![CC BY-SA 4.0][cc-by-sa-shield]][cc-by-sa]

This work is licensed under a
[Creative Commons Attribution-ShareAlike 4.0 International License][cc-by-sa].

[![CC BY-SA 4.0][cc-by-sa-image]][cc-by-sa]

[cc-by-sa]: http://creativecommons.org/licenses/by-sa/4.0/
[cc-by-sa-image]: https://licensebuttons.net/l/by-sa/4.0/88x31.png
[cc-by-sa-shield]: https://img.shields.io/badge/License-CC%20BY--SA%204.0-lightgrey.svg
