# Pong Game in C++

This project is a simple terminal-based Pong game implemented in C++ for Linux systems. The game allows two players to control paddles and play table tennis (pong) against each other.

## Features

- Two-player Pong game
- Player 1 controls the left paddle, and Player 2 controls the right paddle
- Ball bounces off walls and paddles
- The first player to miss the ball gives the opponent a point
- The game continues until Player 1 or Player 2 decides to quit

## Controls

### Player 1:
- **Move up**: `W`
- **Move down**: `S`

### Player 2:
- **Move up**: `I`
- **Move down**: `K`

### Quit Game:
- Press `Q` to quit the game.

## How to Compile and Run

### Prerequisites

- Linux-based system (e.g., Fedora, Ubuntu)
- g++ compiler

### Installation of g++ on Fedora:
To install g++ (C++ compiler) on Fedora, use the following command:
```bash
/  sudo dnf install gcc-c++ /   (in fedora)
/  sudo apt-get install gcc-c++  /  (in ubuntu)
