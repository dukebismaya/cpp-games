#include "raylib.h"
#include <cstddef>
#include <string>

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

    ball.position = {player.position.x,
                     player.position.y - (player.position.y / 2) - ball.radius};

    gameOver = false;
    pause = false;
  }
  ~BismayaEngine() { CloseWindow(); }

  void run() {
    while (!WindowShouldClose()) {
      update();
      draw();
    }
  }

  void update() {
    // Player Movement
    if (IsKeyDown(KEY_LEFT))
      player.position.x -= 5.0f;
    if (IsKeyDown(KEY_RIGHT))
      player.position.x += 5.0f;
    if ((player.position.x - player.size.x / 2) <= 0)
      player.position.x = player.size.x / 2;
    if ((player.position.x + player.size.x / 2) >= SCREEN_WIDTH)
      player.position.x = SCREEN_WIDTH - player.size.x / 2;
  }

  void draw() {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawRectangle((int)(player.position.x - player.size.x / 2),
                  (int)(player.position.y - player.size.y / 2),
                  (int)player.size.x, (int)player.size.y, BLACK);
    DrawCircleV(ball.position, static_cast<float>(ball.radius), BLUE);
    std::string instruction{"Press SPACE to launch the ball"};
    DrawText(instruction.c_str(), (SCREEN_WIDTH / 2) - 150, SCREEN_HEIGHT - 20,
             20, DARKGRAY);
    EndDrawing();
  }
};

int main() {
  BismayaEngine engine;
  engine.run();

  return 0;
}