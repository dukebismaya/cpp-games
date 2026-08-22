#include "raylib.h"

constexpr int SCREEN_HEIGHT = 450;
constexpr int SCREEN_WIDTH = 800;
constexpr int PLAYER_MAX_LIFE = 5;

class Player {
public:
  Vector2 position;
  Vector2 size;
  int life;
  Player() {
    size = {(float)SCREEN_WIDTH / 10, 20};
    position = {(float)SCREEN_WIDTH / 2, (float)SCREEN_HEIGHT * 7 / 8};
    life = PLAYER_MAX_LIFE;
  }
};

class Ball {
public:
  Vector2 position;
  Vector2 speed;
  int radius;
  bool active;

  Ball() {
    position = {0, 0};
    speed = {0, 0};
    radius = 7;
    active = false;
  }
};

class BismayaEngine {
private:
  Player player;
  Ball ball;
  bool gameOver;
  bool pause;

public:
  BismayaEngine() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Packet-Braker C++");
    SetTargetFPS(60);
    gameOver = false;
    pause = false;
  }
  ~BismayaEngine() { CloseWindow(); }

  void run() {
    while (!WindowShouldClose()) {
      Update();
      draw();
    }
  }

  void Update() {
    // I will write later.
  }

  void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("BismayaEngine is Running. Ready to draw entities.", 200, 200, 20,
             DARKGRAY);
    EndDrawing();
  }
};

int main() {
  BismayaEngine engine;
  engine.run();

  return 0;
}