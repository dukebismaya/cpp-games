#include "raylib.h"
#include <cstddef>
#include <format>
#include <string>

constexpr int SCREEN_HEIGHT = 450;
constexpr int SCREEN_WIDTH = 800;
constexpr int PLAYER_MAX_LIFE = 5;
constexpr int LINES_OF_BRICKS = 5;
constexpr int BRICKS_PER_LINE = 20;
int player_curr_life = PLAYER_MAX_LIFE;

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
    radius = 10;
    active = false;
  }
};

class Brick {
public:
  Vector2 brickSize;
  Vector2 position;
  Color brickColor;
  bool active;
  Brick() {
    position = {0, 0};
    active = false;
  };
};

class BismayaEngine {
private:
  Player player;
  Ball ball;
  Brick brick[LINES_OF_BRICKS][BRICKS_PER_LINE];
  Vector2 brickSize;
  bool gameOver;
  bool pause;
  bool enable_instructions;

public:
  BismayaEngine() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Packet-Braker C++");
    SetTargetFPS(60);

    ball.position = {player.position.x,
                     player.position.y - (player.position.y / 2) - ball.radius};
    brickSize = {(float)SCREEN_WIDTH / BRICKS_PER_LINE, 40.0f};
    int initialDownPosition = 50;

    for (int i = 0; i < LINES_OF_BRICKS; ++i) {
      for (int j = 0; j < BRICKS_PER_LINE; ++j) {
        auto &curr_brick = brick[i][j];
        curr_brick.position = {j * brickSize.x + brickSize.x / 2,
                               i * brickSize.y + initialDownPosition};
        curr_brick.active = true;
      }
    }

    gameOver = false;
    pause = false;
    enable_instructions = true;
  }
  ~BismayaEngine() { CloseWindow(); }

  void run() {
    while (!WindowShouldClose()) {
      update();
      draw();
    }
  }

  void update() {
    if (!gameOver && !pause) {
      // Player Movement
      if (IsKeyDown(KEY_LEFT))
        player.position.x -= 5.0f;
      if (IsKeyDown(KEY_RIGHT))
        player.position.x += 5.0f;
      if ((player.position.x - player.size.x / 2) <= 0)
        player.position.x = player.size.x / 2;
      if ((player.position.x + player.size.x / 2) >= SCREEN_WIDTH)
        player.position.x = SCREEN_WIDTH - player.size.x / 2;

      // Ball launch and trajectory
      if (!ball.active) {
        enable_instructions = true;
        ball.position = {player.position.x,
                         player.position.y - (player.size.y / 2) - ball.radius};
        if (IsKeyPressed(KEY_SPACE)) {
          ball.active = true;
          ball.speed = {0.0f, -5.0f}; // 5 pixels UP per frame
        }
      } else {
        enable_instructions = false;
        ball.position.x += ball.speed.x;
        ball.position.y += ball.speed.y;

        // Left and right boundary
        if (((ball.position.x + ball.radius) >= SCREEN_WIDTH) ||
            ((ball.position.x - ball.radius) <= 0))
          ball.speed.x *= -1.0f;

        // Top boundary
        if (ball.position.y - ball.radius <= 0)
          ball.speed.y *= -1.0f;

        // Bottom boundary (loss condition)
        if ((ball.position.y + ball.radius) >= SCREEN_HEIGHT) {
          ball.speed = {0, 0};
          ball.active = false;
          player.life--;
          player_curr_life--;
          if (player.life <= 0)
            gameOver = true;
        }

        // Paddle/player rectangle bound
        Rectangle paddleRec = {player.position.x - player.size.x / 2,
                               player.position.y - player.size.y / 2,
                               player.size.x, player.size.y};
        if (CheckCollisionCircleRec(
                ball.position, static_cast<float>(ball.radius), paddleRec)) {
          if (ball.speed.y > 0) {
            ball.speed.y *= -1.0f;

            ball.speed.x = (ball.position.x - player.position.x) /
                           (player.size.x / 2) * 5.0f;
          }
        }
        int activeBricks{};
        for (int i = 0; i < LINES_OF_BRICKS; ++i) {
          for (int j = 0; j < BRICKS_PER_LINE; ++j) {
            auto &curr_brick = brick[i][j];
            if (curr_brick.active) {
              activeBricks++;
              Rectangle brickRec = {curr_brick.position.x - brickSize.x / 2,
                                    curr_brick.position.y - brickSize.y / 2,
                                    brickSize.x, brickSize.y};
              if (CheckCollisionCircleRec(ball.position,
                                          static_cast<float>(ball.radius),
                                          brickRec)) {
                curr_brick.active = false;
                if (ball.position.y <= brickRec.y ||
                    ball.position.y >= brickRec.y + brickRec.height) {
                  ball.speed.y *= -1.0f;
                } else {
                  ball.speed.x *= -1.0f;
                }
              }
            }
          }
        }
        if (activeBricks <= 0) {
          gameOver = true;
        }
      }
    }
  }

  void draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawRectangle((int)(player.position.x - player.size.x / 2),
                  (int)(player.position.y - player.size.y / 2),
                  (int)player.size.x, (int)player.size.y, WHITE);
    DrawCircleV(ball.position, static_cast<float>(ball.radius), RED);

    // Drawing Bricks
    for (int i = 0; i < LINES_OF_BRICKS; ++i) {
      for (int j = 0; j < BRICKS_PER_LINE; ++j) {
        const auto &curr_brick = brick[i][j];
        if (curr_brick.active) {
          Color brickColor = ((i + j) % 2 == 0) ? GREEN : DARKGRAY;
          DrawRectangle(
              static_cast<int>(curr_brick.position.x - brickSize.x / 2),
              static_cast<int>(curr_brick.position.y - brickSize.y / 2),
              static_cast<int>(brickSize.x), static_cast<int>(brickSize.y),
              brickColor);
        }
      }
    }

    if (enable_instructions) {
      std::string instruction{"Press SPACE to launch the ball"};
      DrawText(instruction.c_str(), (SCREEN_WIDTH / 2) - 150,
               SCREEN_HEIGHT - 20, 20, DARKGRAY);
    } else {
      DrawText(std::format("{}/{}", player_curr_life, PLAYER_MAX_LIFE).c_str(),
               (SCREEN_WIDTH / 2), SCREEN_HEIGHT - 20, 20, DARKGRAY);
    }
    EndDrawing();
  }
};

int main() {
  BismayaEngine engine;
  engine.run();

  return 0;
}