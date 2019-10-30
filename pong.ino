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
  int xspeed;
  int yspeed;
};

struct Paddle {
  int x;
  int y;
  int width;
  int height;
  int speed;
  int score;
};

struct Paddle player = {2, 24, 3, 14, 1, 0};
struct Paddle computer = {arduboy.width()-4, 24, 3, 14, 1, 0};
struct Ball ball = {62, 16, 4, 1, 1, 1, 1};

bool isEdgeHit(Paddle paddle) {
  return
    (ball.y + ball.size < paddle.y + 3) ||
    (ball.y > paddle.y + 10);
}

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
      arduboy.println("wow it's pong");
      arduboy.println("---===---");
      arduboy.println();
      arduboy.println("A - power shot");
      arduboy.println("B - fast move");
      arduboy.println();
      arduboy.println("Press A to start");
      if (arduboy.justPressed(A_BUTTON)) {
        gameState += 1;
      }
      break;

    case 1: // gameplay
      //--- player input
      if (arduboy.pressed(B_BUTTON)) {
        // fast move
        player.speed = 2;
      } else {
        player.speed = 1;
      }
      if (arduboy.pressed(UP_BUTTON) && player.y > 1) {
        player.y -= 1 * player.speed;
      }
      if (arduboy.pressed(DOWN_BUTTON) && player.y + player.height < arduboy.height() - 1) {
        player.y += 1 * player.speed;
      }

      //--- computer movement
      if (random(30) == 1) {
        // random fast move
        computer.speed = 2;
      } else {
        computer.speed = 1;
      }
      if (ball.x > arduboy.width() / 5 * 4 || random(20) == 1) {
        // only move when ball is close (or random chance)
        if (ball.y < computer.y && computer.y > 1) {
          computer.y -= computer.speed;
        }
        if (ball.y > computer.y && computer.y + computer.height < arduboy.height() - 1) {
          computer.y += computer.speed;
        }
      }

      //--- player collision
      if (
        ball.x < player.x + player.width
        && ball.y + ball.size > player.y
        && ball.y < player.y + player.height
        && ball.rightDir < 0
      ) {
        // reverse direction
        ball.rightDir *= -1;
        if (arduboy.pressed(A_BUTTON)) {
          // power shot
          ball.xspeed = 2;
        } else {
          // lasts until ball is returned
          ball.xspeed = 1;
        }
        if (isEdgeHit(player)) {
          ball.yspeed = 2;
        } else {
          ball.yspeed = 1;
        }
      }

      //--- computer collision
      if (
        ball.x + ball.size > computer.x
        && ball.y + ball.size > computer.y
        && ball.y < computer.y + computer.height
        && ball.rightDir > 0
      ) {
        ball.rightDir = -1;
        if (random(10) == 1) {
          // random power shot
          ball.xspeed = 2;
        }
        if (isEdgeHit(computer)) {
          ball.yspeed = 2;
        } else {
          ball.yspeed = 1;
        }
      }

      //--- ball movement
      ball.x += ball.rightDir * ball.xspeed;
      ball.y += ball.downDir * ball.yspeed;

      //--- bounds collision
      if (ball.y <= 0) {
        ball.downDir = 1;
      }
      if (ball.y + ball.size >= 63) {
        ball.downDir = -1;
      }

      //--- points
      bool outOfBoundsLeft = ball.x <= 0;
      bool outOfBoundsRight = ball.x >= arduboy.width();
      bool outOfBounds = outOfBoundsLeft || outOfBoundsRight;
      if (outOfBounds) {
        ball.xspeed = 1;
        ball.yspeed = 1;
      }
      if (outOfBoundsLeft) {
        computer.score += 1;
        ball.x = arduboy.width() / 2;
        ball.y = random(10, arduboy.height()-10);
      }
      if (outOfBoundsRight) {
        player.score += 1;
        ball.x = arduboy.width() / 2;
        ball.y = random(10, arduboy.height()-10);
      }

      //--- draw the things
      // bounds
      arduboy.drawLine(0, 0, arduboy.width(), 0, WHITE);
      arduboy.drawLine(0, arduboy.height()-1, arduboy.width(), arduboy.height()-1, WHITE);
      // players
      arduboy.fillRect(player.x, player.y, player.width, player.height, WHITE);
      arduboy.fillRect(computer.x, computer.y, computer.width, computer.height, WHITE);
      // ball
      arduboy.fillRect(ball.x, ball.y, ball.size, ball.size, WHITE);

      arduboy.setCursor(arduboy.width() / 2 - 14, 2);
      arduboy.print(player.score);
      arduboy.setCursor(arduboy.width() / 2 - 2, 2);
      arduboy.print("|");
      arduboy.setCursor(arduboy.width() / 2 + 10, 2);
      arduboy.print(computer.score);

      // debug

      break;

    case 2: // win screen
      arduboy.setCursor(0, 0);
      arduboy.print("win screen");
      if (arduboy.justPressed(B_BUTTON)) {
        gameState += 1;
      }
      break;

    case 3: // game over screen
      arduboy.setCursor(0, 0);
      arduboy.print("game over");
      if (arduboy.justPressed(B_BUTTON)) {
        gameState = 0;
      }
      break;
  }

  arduboy.display();
}

