# 🏓 Retro Pong Arcade

A modern, arcade-styled take on the classic table tennis game, built entirely from scratch in **C++** using a custom win32 platform layer and software renderer!

---

## ✨ Features

* **SOLO Mode:** Test your skills against a built-in AI opponent.
* **DUO Mode:** Grab a friend and play local multiplayer on the same keyboard.
* **Smooth Physics:** Responsive paddle acceleration, friction mechanics, and dynamic ball velocity scaling.
* **Modular Architecture:** Clean separation of systems (`arena.cpp`, `menu.cpp`, `paddle.cpp`, `game.cpp`, and `renderer.cpp`).

---

## 🎮 Controls

| Action | Keys |
| :--- | :--- |
| **Player 1** *(Right Paddle / Solo Mode)* | `Up Arrow` / `Down Arrow` |
| **Player 2** *(Left Paddle in Duo Mode)* | `W` / `S` Keys |
| **Menu Navigation** | `Left` / `Right Arrows` to toggle, `Enter` to start |

---

## 🚀 Getting Started & Building from Source

### Prerequisites
* **Visual Studio** (with the *Desktop development with C++* workload installed).
* Windows OS (uses Win32 API for window creation and rendering).

### Steps to Build and Run
1. Clone or download this repository to your local machine.
2. Open the solution file (`.sln`) in Visual Studio.
3. Set the build configuration to **x64** and **Debug** (or Release).
4. Press **F5** (or click the green **Local Windows Debugger** button) to compile and launch the game!

---

## 🛠️ Credits & Inspiration
Built as a custom game engine and programming project following **Dan Zaidan's C++ game development tutorials**.
