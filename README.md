# 🎮 Maze Chase – Interactive 2D Game with BFS Pathfinding

## 📌 About the Project

Maze Chase is a simple interactive 2D maze game developed in C. The player navigates through a grid-based maze, collects coins, avoids enemies, and reaches the exit to complete each level.

The game uses the **Breadth-First Search (BFS)** algorithm to help enemies find the shortest path toward the player's position in an unweighted maze.

## ✨ Features

- 🗺️ Grid-based maze with walls and open paths
- 🎯 Three difficulty levels: Easy, Medium, and Hard
- 👾 Enemies that chase the player using BFS pathfinding
- 🪙 Coin collection and score tracking
- 🚪 Exit unlocks after all coins are collected
- 💥 Collision detection and game-over condition
- ⌨️ Keyboard controls for player movement
- 🔄 Automatic progression through levels

## 🧠 Algorithm Used: BFS

Breadth-First Search explores neighbouring cells level by level using a queue. It finds a shortest path in an unweighted grid.

In this game, BFS determines the next step an enemy should take to move toward the player.

### Data Structures Used

- **2D Array:** Represents the maze grid.
- **Queue:** Stores grid positions to be explored by BFS.
- **Visited Array:** Tracks explored positions.
- **Parent Arrays:** Reconstruct the path from the enemy to the player.
- **Structures in C:** Store player and enemy information.

## 🕹️ Controls

| Key | Action |
|---|---|
| W | Move Up |
| S | Move Down |
| A | Move Left |
| D | Move Right |
| Q | Quit Game |

## 🛠️ Technologies Used

- Programming Language: C
- Algorithm: Breadth-First Search (BFS)
- Data Structures: 2D arrays, queue, structures
- Development Environment: VS Code

## ▶️ How to Run

### Prerequisites

Install a C compiler such as GCC.

### Compile

```bash
gcc main.c -o maze_chase
```

### Run on Windows

```bash
maze_chase.exe
```

### Run on Linux

```bash
./maze_chase
```

Note: The screen-clearing command in the current source code uses `system("cls")`, which is intended for Windows. For Linux compatibility, update the screen-clearing logic before running it there.

## 🎯 Project Objective

The objective is to demonstrate how pathfinding algorithms and data structures can be applied to an interactive grid-based game.

## 👩‍💻 Author

Shreya Nahar
