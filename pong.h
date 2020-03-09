#include "initialize.h"

//This array would make all 64 LED's light up
byte ALL[] = {B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111,B11111111};

//The position and velocity of the pong ball
int x = 4;
int y = 4;
int x_dir = 1;
int y_dir = 1;

//The upper and lower bounds for position calculations
byte upper = 7;
byte lower = 0;

//The x position of the board. No y position because the board stays at the bottom of the playing field
int board_x = 4;



//Function calculating movement of and applying movement to the ball
void travel() {
  //Bouncing off the edge of the playing field, by applying the upper and lower bounds set above
  if ((x + x_dir) < lower || (x + x_dir) > upper) {
    x_dir = -x_dir;
  }
  if ((y + y_dir) < lower || (y + y_dir) > upper) {
    y_dir = -y_dir;
  }

  //Checking for collision of the ball and the board. Then apply bounce.
  //Accidentally made this work wonderfully, better than anticipated
  if ((y + y_dir) == 0 && (x + x_dir == board_x)) {
    y_dir = -y_dir;
    x_dir = abs(x_dir);
  }
  if ((y + y_dir) == 0 && (x + x_dir == board_x - 1)) {
    y_dir = -y_dir;
    x_dir = -abs(x_dir);
  }
  
  //Inputting movement to the ball
  if (x_dir == 1) {
    x++;
  } else {
    x--;
  }
  if (y_dir == 1) {
    y++;
  } else {
    y--;
  }
}

//Function for the movement of the board
void move_board() {
  //The x position of the board takes in input from the potentiometer
  board_x = 1 + (analogRead(A4)/1023.0) * 6;
}

//Function for drawing objects onto the 64-LED matrix
void draw(byte object[]) {
  //Looping through the rows
  for (int i = 0; i < 8; i++) {
    digitalWrite(rows[i], HIGH);
    //Looping through the LED's within each row
    for (int k = 0; k < 8; k++) {
      digitalWrite(cols[k], (~object[i] >> (7-k)) & 1);
      digitalWrite(cols[k], HIGH);
    }
    digitalWrite(rows[i], LOW);
  }
}

//Function for drawing the pong ball
void drawball() {
  //Initializing and declaring the array storing the pong ball's position
  byte *p_ball = new byte[8]{};
  for (int i = 0; i < 8; i++) {
    p_ball[i] = 0;
  }
  
  //Input x and y of the ball by using the array
  for (int i = 0; i <= x; i++) {
    if (i == x) {
      p_ball[i] = (1 << y);
    }
  }
  //Drawing the ball and deleting dynamically allocated array -> gotta prevent them memory leaks
  draw(p_ball);
  delete[] p_ball;
  p_ball = 0;
}

//Function for drawing the board
void drawboard() {
  //Initializing and declaring the array storing the position of the player-board
  byte *p_board = new byte[8]{};
  for (int i = 0; i < 8; i++) {
    p_board[i] = 0;
  }
  
  //Input x and y of the board using the array defined above
  for (int i = 0; i <= board_x; i++) {
    if (i == board_x) {
      p_board[i] = 1;
      p_board[i-1] = 1;
    }
  }
  //Drawing the board and deleting dynamically allocated array
  draw(p_board);
  delete[] p_board;
  p_board = 0;
}

//Default game settings. Needed for resetting the game and also increasing difficult of the game as time passes
int counter = 0;
unsigned int speedup = 0;
unsigned int velocity = 300;

//Function for resetting the game, setting all game variables back to default
void reset_game() {
    for (int i = 0; i < 5; i++) {
      draw(ALL);
      delay(100);
    }
    x = 4;
    y = 4;
    y_dir = 1;
    x_dir = 1;
    velocity = 300;
    speedup = 0;
    counter = 0;
}

//Function for game start, calling all required functions above to make the game run
void start_game() {
  drawball();
  drawboard();
  move_board();
  counter++;
  speedup++;

  //After a certain interval of time, the game difficulty increases
  if(speedup == 3500) {
    speedup = 0;
    if (velocity > 100) {
      velocity-=50;
    }
  }
  if (counter >= velocity) {
    travel();
    counter = 0;
  }
  
  //Lose, reset;
  if (y == 0) {
    reset_game();
  }
}
