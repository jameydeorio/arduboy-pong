#include <Arduboy2.h>

Arduboy2 arduboy;

const int FPS = 60;

int gameState = 0;

struct Ball {
  int x;
  int y;
  int size;
  int rightDir;
  int downDir;
};

struct Player {
  int x;
  int y;
  int width;
  int height;
};

struct Player player = {2, 0, 3, 12};
struct Ball ball = {62, 0, 4, 1, 1};

void setup() {
  arduboy.begin();
  arduboy.setFrameRate(FPS);
  arduboy.initRandomSeed();
}

void loop() {
  // pause render until it's time for the next frame
  if (!(arduboy.nextFrame()))
    return;

  arduboy.clear();

  arduboy.pollButtons();

  switch(gameState) {
    case 0: // title screen
      arduboy.setCursor(0, 0);
      arduboy.print("title screen");
      if (arduboy.justPressed(A_BUTTON)) {
        gameState += 1;
      }
      break;
      
    case 1: // gameplay
      arduboy.setCursor(0, 0);
      arduboy.print("gameplay");

      // player movement
      if (arduboy.pressed(UP_BUTTON)) {
        player.y -= 1;
      }
      if (arduboy.pressed(DOWN_BUTTON)) {
        player.y += 1;
      }

      // ball movement
      ball.x += ball.rightDir;
      if (ball.x == 0) {
        ball.rightDir = 1;
      }
      if (ball.x + ball.size == 127) {
        ball.rightDir = -1;
      }

      ball.y += ball.downDir;
      if (ball.y == 0) {
        ball.downDir = 1;
      }
      if (ball.y + ball.size == 63) {
        ball.downDir = -1;
      }

      // draw the things
      arduboy.drawLine(0, 0, arduboy.width(), 0, WHITE);
      arduboy.drawLine(0, arduboy.height()-1, arduboy.width(), arduboy.height()-1, WHITE);
      arduboy.fillRect(player.x, player.y, player.width, player.height, WHITE);
      arduboy.fillRect(ball.x, ball.y, ball.size, ball.size, WHITE);

      if (arduboy.justPressed(A_BUTTON)) {
        gameState += 1;
      }
      break;
      
    case 2: // win screen
      arduboy.setCursor(0, 0);
      arduboy.print("win screen");
      if (arduboy.justPressed(A_BUTTON)) {
        gameState += 1;
      }
      break;
      
    case 3: // game over screen
      arduboy.setCursor(0, 0);
      arduboy.print("game over");
      if (arduboy.justPressed(A_BUTTON)) {
        gameState = 0;
      }
      break;
  }

  arduboy.display();
}
