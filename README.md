# Xonix Game - C++ with SFML

**Author:** Wasif Mehmood Mughal
**Roll No:** i240699
**University:** FAST University Islamabad

A modern two-player version of the classic Xonix game. Players navigate the grid to capture territory while avoiding bouncing enemies and collecting power-ups. Featuring multi-level progression, dynamic difficulty, and a persistent scoreboard.

---

## 🎮 Gameplay Overview

* Two-player competitive territory capture
* Avoid enemies and rival trails
* Earn power-ups to freeze enemies
* Scoreboard tracks top 5 scores
* Dynamic difficulty scaling with time

---

## 🧱 Built With

* **C++ Standard Library**
* **SFML (Simple and Fast Multimedia Library)**
* Uses: `Graphics`, `Audio`, `Event Handling`, `System Timing`

---

## 🕹 Controls

* **Player 1:** Arrow Keys
* **Player 2:** W, A, S, D
* **Use Power-Up:** Press `1` (P1) or `2` (P2)
* **Pause/Quit:** Escape

---

## 📁 Assets Required

Place these files in the same folder as your executable:

* `images/tiles.png`
* `images/gameover.png`
* `images/enemy.png`
* `pokemon.ttf`

---

## ⚙️ Compilation Instructions

### 🔧 Prerequisites:

* Install [SFML 2.5+](https://www.sfml-dev.org/download.php)
* C++17-compatible compiler (g++, clang++)

### 🧪 Compile:

```bash
g++ -o xonix main.cpp -lsfml-graphics -lsfml-window -lsfml-system
```

### ▶️ Run:

```bash
./xonix
```

---

## 🧠 Key Features

* 🌀 2D Grid-based capture mechanics
* 👾 Randomly moving bouncing enemies
* 🧊 Power-up system (freeze enemies)
* ⏫ Levels increase every 20 seconds
* 🧮 Score multipliers for large captures
* 📜 Scoreboard stored in `scores.txt`
* 💾 Bubble sort for score ranking

---

## 📈 Scoreboard System

* Top 5 scores saved in `scores.txt`
* Scores sorted by highest first, with time as tiebreaker
* Input player names upon game over
* Displays scoreboard in console after each game

---

## 🧩 How It Works

* Grid matrix: `grid[M][N]`

  * `0 = empty`, `1 = border`, `2 = P1 trail`, `3 = P2 trail`
* Collision with enemy or trail ends that player’s game
* `drop()` flood-fill algorithm claims territory
* Each 50 points earns a power-up
* Levels add enemies and increase their speed

---

## 🔮 Future Enhancements

* 🎵 Add background music and SFX
* 🧠 Smarter enemy AI (target players)
* ❤️ Lives/respawn system
* 🧮 Use `std::vector` instead of raw pointers
* 🖼 In-game scoreboard UI (instead of console)
* ✨ Enhanced trail and capture animations

---

## 📜 License

This project is developed for academic and personal learning purposes. Commercial use is prohibited without prior consent.

---
