#include "raylib.h"

constexpr int SCREEN_HEIGHT = 450;
constexpr int SCREEN_WIDTH = 800;

class BismayaEngine {
private:
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
      // Update();
      draw();
    }
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