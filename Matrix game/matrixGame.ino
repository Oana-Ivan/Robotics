// For the lcd
#include <LiquidCrystal.h>
const int RS = 6;
const int enable = 7; 
const int d4 = 4;
const int d5 = 3;
const int d6 = 2;
const int d7 = 5;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

// For the joystick
const int pinSw = 9;
const int pinX = A0; 
const int pinY = A1;

int swValue;
int xValue;
int yValue;

int minThreshold = 400;
int maxThreshold = 600;
int buttonState = 0;
bool joyMoved = false;

// For the matrix
#include "LedControl.h"
LedControl lc = LedControl(12, 11, 10, 1);

// For the game
#include <EEPROM.h>
int matrix[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
};
// matrix[i][j] == 0 => the position is not occupied
// matrix[i][j] == 1 => player
// matrix[i][j] == 2 => food
// matrix[i][j] == 3 => bomb

bool game = false; // false = off, true = on
unsigned long timeSincePlaying;
unsigned long timeSinceStart;
unsigned long playingTime = 60000; // the duration of each level
int lives;
int level;
int speed1 = 400, speed2 = 300, speed3 = 200; // speedi - the speed at which the elements descend at level i
int minFood1 = 5, minFood2 = 10, minFood3 = 15; // minFoodi - the minimum points the player has to collect to win the level i
int currentScore, score1, score2, score3; // when level i is won scorei = currentScore; currentScore = 0; 

void initialization() { // the values when the game begins
  lives = 3;
  level = 1;
  currentScore = 0;
  score1 = 0;
  score2 = 0;
  score3 = 0;
}

// Functions for the lcd
void lcdWelcome() {
  lcd.setCursor(1, 0);
  lcd.print("Catch and Run");
  unsigned long currentTime = millis();
  int i = 0;
  while (i != 16) {
    if (millis() -  currentTime == 100) {
      currentTime = millis();
      lcd.setCursor(i, 1); lcd.print("~");
      i++;
    }
  }
  lcd.clear();
}
int currentPosition = 0; // used for scroll through the menu
int lcdState = 0;
// displayMenu             lcdState = 0
void displayStart();    // lcdState = 1
void displayScore();    // lcdState = 2
void displaySettings(); // lcdState = 3
void displayInfo();     // lcdState = 4
void showOption(); // start display option based on the lcdState
unsigned long startDisplayScore; // the time when the option "Score" was selected 
unsigned long timeStartInfo;     // the time when the option  "Info" was selected

void lcdGameOff() {
  if (lcdState == 0) {
    lcd.setCursor(1, 0); lcd.print("Start");
    lcd.setCursor(11, 0); lcd.print("Score");
    lcd.setCursor(1, 1); lcd.print("Settings");
    lcd.setCursor(12, 1); lcd.print("Info");
    swValue = digitalRead(pinSw);
    xValue = analogRead(pinX);
    yValue = analogRead(pinY);
  
    buttonState = swValue;
    
    if (yValue > maxThreshold && joyMoved == false) {
      if (currentPosition < 3) currentPosition++;
      else currentPosition = 0;
      joyMoved = true;
    }
    if (yValue < minThreshold && joyMoved == false) {
      if (currentPosition > 0) currentPosition--;
      else currentPosition = 3;
      joyMoved = true;
    }
    if (yValue >= minThreshold && yValue <= maxThreshold) {
      joyMoved = false;
    }
  
    // delete the last arrows printed
    lcd.setCursor(0, 0);  lcd.print(" ");
    lcd.setCursor(10, 0); lcd.print(" ");
    lcd.setCursor(0, 1);  lcd.print(" ");
    lcd.setCursor(11, 1); lcd.print(" ");
    
   if (currentPosition == 0) {
        lcd.setCursor(0, 0);
        lcd.print(">");
    }
    else if (currentPosition == 1) {
            lcd.setCursor(10, 0);
            lcd.print(">");
          }
        else if (currentPosition == 2) {
               lcd.setCursor(0, 1);
               lcd.print(">");
              }
              else if (currentPosition == 3) {
                lcd.setCursor(11, 1);
                lcd.print(">");
              }
    if (buttonState == LOW) {
        showOption(currentPosition);
    }
  }
  else {
    if (lcdState == 1) displayStart();
    else if (lcdState == 2) displayScore();
          else if (lcdState == 3) displaySettings();
            else displayInfo();
  }
}
// showOption receives p, the value of the lcdState, and makes the 
// initialization needed to run the function after it is selected
void showOption(int p) { 
  lcd.clear();
  
  if (p == 0) { // option == Start
    lcdState = 1;
    game = true; 
    initialization();
    timeSinceStart = millis();
    lc.clearDisplay(0);  
    displayStart(); 
  }
  else if (p == 1) { // option == Score
    lcdState = 2;
    startDisplayScore = millis(); 
    displayScore(); 
  }
       else if (p == 2) displaySettings(); // option == Settings
            else { // option == Info
              lcdState = 4;
              timeStartInfo = millis(); 
              displayInfo(); 
            }
}
void displayStart() {
  if (millis() - timeSinceStart <= playingTime + 1) {
    timeSincePlaying = (millis() - timeSinceStart)/1000;
    lcd.setCursor(0, 0); lcd.print("Score:");
    lcd.setCursor(6, 0); lcd.print(currentScore);
    lcd.setCursor(9, 0); lcd.print("Level:");
    lcd.setCursor(15, 0); lcd.print(level);
    lcd.setCursor(0, 1); lcd.print("Time:");
    lcd.setCursor(5, 1); lcd.print(timeSincePlaying);
    lcd.setCursor(9, 1); lcd.print("Lives:");
    lcd.setCursor(15, 1); lcd.print(lives);
  }
  else {
    lcd.clear();
    lcdState = 0;
  } 
}

unsigned long displayScoreValue = 3000;
void displayScore() {
  if (millis() - startDisplayScore <= displayScoreValue) {
    lcd.setCursor(0, 0); lcd.print("HighScore:");
    int score = EEPROM.read(0);                                
    lcd.setCursor(0, 1); lcd.print(score);
  }
  else {
    lcd.clear();
    lcdState = 0;
  }
}

void displaySettings() {
  // lcd.setCursor(0, 0); lcd.print("Settings:");
  lcd.clear();
  lcdState = 0;
}

unsigned long displayInfoTime = 3000;
void displayInfo() {
  if (millis() - timeStartInfo < displayInfoTime) {
     lcd.setCursor(2, 0); lcd.print("Creator name:");
     lcd.setCursor(3, 1); lcd.print("Oana Ivan");
  }
  else if (millis() - timeStartInfo == displayInfoTime){
    lcd.clear();
  }
  else if (millis() - timeStartInfo < 2 * displayInfoTime) {
     lcd.setCursor(0, 0); lcd.print("github.com/Oana-");
     lcd.setCursor(2, 1); lcd.print("Ivan/Robotics");
  } 
  else if (millis() - timeStartInfo == 2 * displayInfoTime) {
    lcd.clear();
  }
  else if (millis() - timeStartInfo < 3 * displayInfoTime) {
     lcd.setCursor(0, 0); lcd.print("  Game name:        ");
     lcd.setCursor(0, 1); lcd.print(" Catch and Run ");
  }
  else if (millis() - timeStartInfo == 3 * displayInfoTime) {
    lcd.clear();
  }
  else if (millis() - timeStartInfo <= 4 * displayInfoTime) {
     lcd.setCursor(0, 0); lcd.print("@UnibucRobotics");
     lcd.setCursor(0, 1); lcd.print("               ");
  }
  else {
    lcd.clear(); 
    lcdState = 0; 
  }
}

void lcdGameOn() {
  displayStart();
}

// Functions for the matrix when the game is off:

// Variables for the animation displayed when the game is off:
int col = 0, row = 0, row2 = 0;
long unsigned interval1 = 600;
long unsigned interval2 = 1000;
long unsigned lastTime;
long unsigned lastTime2;

void matrixGameOff() {
  if (millis() - lastTime >= interval1) {
    lastTime = millis();
    if (col == 7) {
      lc.setLed(0, 7, col, 0);
      col = 0;
      lc.setLed(0, 7, col, 1);
    }
    else {
      col++;
      lc.setLed(0, 7, col, 1);
      lc.setLed(0, 7, col - 1, 0);
    }

    if (row == 5) {
      lc.setLed(0, row, 1, 0);
      lc.setLed(0, row, 5, 0);
      row = 0;
      lc.setLed(0, row, 1, 1);
      lc.setLed(0, row, 5, 1);
    }
    else {
      row++;
      lc.setLed(0, row, 1, 1);
      lc.setLed(0, row, 5, 1);
      lc.setLed(0, row - 1, 1, 0);
      lc.setLed(0, row - 1, 5, 0);
    }
  }
  if (millis() - lastTime2 >= interval2) {
    lastTime2 = millis();
    if (row2 == 5) {
      lc.setLed(0, row2, 3, 0);
      lc.setLed(0, row2, 7, 0);
      row2 = 0;
      lc.setLed(0, row2, 3, 1);
      lc.setLed(0, row2, 7, 1);
    }
    else {
      lc.setLed(0, row2, 3, 0);
      lc.setLed(0, row2, 7, 0);
      row2++;
      lc.setLed(0, row2, 3, 1);
      lc.setLed(0, row2, 7, 1);
    }
  }
}

// Functions for the matrix when the game ended
int won[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 1, 0, 0, 0, 0, 1, 0},
{0, 1, 0, 0, 0, 0, 1, 0},
{0, 0, 1, 1, 1, 1, 0, 0}
};
void displayGameWon() {
  lc.clearDisplay(0);
  lcd.clear(); 
  level++; 
  
  // print on the matrix
  for (int row = 0; row < 8; row++) 
    for (int col = 0; col < 8; col++) 
      lc.setLed(0, row, col, won[row][col]);
      
  // print on the lcd
  if (level == 2) {
    lcd.setCursor(0,0); lcd.print("Lvl 1: completed");
    lcd.setCursor(0,1); lcd.print("Lvl 2 starting..");
    score1 = currentScore;
    currentScore = 0;
  }
  else if (level == 3) {
    lcd.setCursor(0,0); lcd.print("Lvl 2: completed");
    lcd.setCursor(0,1); lcd.print("Lvl 3 starting..");
    score2 = currentScore;
    currentScore = 0;
  }
  else {
    lcd.setCursor(0,0); lcd.print("Lvl 3: completed");
    lcd.setCursor(0,1); lcd.print("Yon won the game!");
    score3 = currentScore;
  }
  
  // displays the previous message for 4 seconds
  unsigned long lastTime = millis();
  while (millis() - lastTime <= 4000);
  
  //  update the score if it is higher than the last high score
  int score = EEPROM.read(0); 
  if (score < (score1 + score2 + score3)) {
    EEPROM.update(0, (score1 + score2 + score3));
  }
  lc.clearDisplay(0);
  lcd.clear();

  // if the game is won (level 3 was completed) show the score
  if (level == 4) {
    lcd.setCursor(1,0); lcd.print("Your score:");
    lcd.setCursor(14,0); lcd.print(score1 + score2 + score3);
    
    if (score < (score1 + score2 + score3)) {
      lcd.setCursor(1,1); lcd.print("HIGHEST SCORE!");
    }
    else {
      lcd.setCursor(1,1); lcd.print("Highscore: ");
      lcd.setCursor(12,1); lcd.print(score);
    }
  
  lastTime = millis();
  while (millis() - lastTime <= 4000);
  
  lc.clearDisplay(0);
  lcd.clear();
  }
}
int lose[8][8] = {
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 1, 0, 0, 1, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 0, 0, 0, 0, 0, 0},
{0, 0, 1, 1, 1, 1, 0, 0},
{0, 1, 0, 0, 0, 0, 1, 0},
{0, 1, 0, 0, 0, 0, 1, 0},
};
void displayGameLost(int causeOfLosing) { 
  // causeOfLosing = 0 => score => the player did not have enough points to finish the level
  // causeOfLosing = 1 => lives => the player did not have enough lives to finish the level
  lc.clearDisplay(0);
  lcd.clear();

  // print on the matrix
  for (int row = 0; row < 8; row++) 
    for (int col = 0; col < 8; col++) 
      lc.setLed(0, row, col, lose[row][col]);

  // print on the lcd
  if (causeOfLosing == 0) {// too lower score
    lcd.setCursor(2,0); lcd.print("You ran ");
    lcd.setCursor(3,1); lcd.print("out of time! :(");
  }
  else {
    lcd.setCursor(2,0); lcd.print("You died! :(");
  }
  
  unsigned long lastTime = millis();
  while (millis() - lastTime <= 4000);
  
  lc.clearDisplay(0);
  lcd.clear();
  game = false;
  lcdState = 0; // return to the menu
}

// Functions for the matrix when the game is on

// Variables for moving the player on his line:
const int playerPositionX = 7; // the player can move only on the last line
int playerPositionY = 3;
int prevPlayerPositionY = 3;

void setPlayerPosition() {
  yValue = analogRead(pinY);
  
  if (yValue > maxThreshold && joyMoved == false) {
    if (playerPositionY < 7) { 
      prevPlayerPositionY = playerPositionY;
      playerPositionY++;
    }
    joyMoved = true;
  }
  if (yValue < minThreshold && joyMoved == false) {
    if (playerPositionY > 0) { 
      prevPlayerPositionY = playerPositionY;
      playerPositionY--;
    }
    joyMoved = true;
  }
  if (yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }
  // update the matrix:
  matrix[playerPositionX][prevPlayerPositionY] = 0; 
  matrix[playerPositionX][playerPositionY] = 1; 
  
  lc.setLed(0, playerPositionX, prevPlayerPositionY, 0);
  lc.setLed(0, playerPositionX, playerPositionY, 1);
}

// Variables and functions for manipulation of elements (food and bombs)
struct elem {
  unsigned long lastMove;
  int type; // 1 for food, 2 for bomb
  int rowPos;
  int colPos;
  int prevRowPos;
  int prevColPos;
  bool alive; // alive == true => the element is displayed on the matrix
};

// moveElement(struct elem *e, int s) moves the element e from the first line to the last with the speed s
// and returns true if the element is still moving (has not arrived at the last line) and false otherwise
// (the number of lives and the score will be updated accordingly)
bool moveElement(struct elem *e, int s) {
  if (millis() - (*e).lastMove >= s) {
    (*e).lastMove = millis();
    (*e).prevRowPos = (*e).rowPos;
    (*e).prevColPos = (*e).colPos;
    (*e).rowPos++;
    
    if ((*e).type == 1) { // type == food
      lc.setLed(0, (*e).prevRowPos, (*e).prevColPos, 0);
      matrix[(*e).prevRowPos][(*e).prevColPos] = 0;
      if ((*e).rowPos == 7) { // if the element arrives at the last line
        if (matrix[(*e).rowPos][(*e).colPos] == 1) { // if the player catches the food
          currentScore++;
        }
        return false; // the element dies
      } 
      // the element did not reach the last line:
      lc.setLed(0, (*e).rowPos, (*e).colPos, 1); 
      matrix[(*e).rowPos][(*e).colPos] = 2;
      return true; // the element is still moving
    }
    
    else if ((*e).type == 2) { // type == bomb
      // detete the first 2 lines of the bomb
      lc.setLed(0, (*e).prevRowPos, (*e).prevColPos, 0);
      lc.setLed(0, (*e).prevRowPos, (*e).prevColPos + 1, 0);
      matrix[(*e).prevRowPos][(*e).prevColPos] = 0;
      matrix[(*e).prevRowPos][(*e).prevColPos + 1] = 0;
      
      if ((*e).rowPos == 6) {
        // if the player touches the bomb
        if ( matrix[(*e).rowPos + 1][(*e).colPos] == 1 || matrix[(*e).rowPos + 1][(*e).colPos + 1] == 1) { 
          lives--;
          if (!lives) {
            displayGameLost(1); // game lost because of the lives
            return true;
          }
        }
        lc.setLed(0, (*e).rowPos, (*e).colPos, 0);
        lc.setLed(0, (*e).rowPos, (*e).colPos + 1, 0);
        matrix[(*e).rowPos][(*e).colPos] = 0;
        matrix[(*e).rowPos][(*e).colPos + 1] = 0;
        return false; // the element dies
      }
      lc.setLed(0, (*e).rowPos + 1, (*e).colPos, 1);
      lc.setLed(0, (*e).rowPos + 1, (*e).colPos + 1, 1);
      matrix[(*e).rowPos + 1][(*e).colPos] = 3;
      matrix[(*e).rowPos + 1][(*e).colPos + 1] = 3;
      return true; // the element is still moving
    }
  }
  return true;
}

void createFood(elem *e) {
  (*e).lastMove = millis();
  (*e).type = 1;
  (*e).alive = true;
  (*e).rowPos = (*e).prevRowPos = 0;  // starting row is 0, will grow in moveElement
  do {
    (*e).colPos = (*e).prevColPos = rand()%8;    // the column of the element is a random number between 0 and 7
  }while (matrix[(*e).colPos][(*e).prevColPos] 
          || matrix[(*e).colPos][(*e).prevColPos + 1]
          || matrix[(*e).colPos][(*e).prevColPos - 1]
          || matrix[(*e).colPos + 1][(*e).prevColPos]); // verify that the position of the new element and his surroundings are not occuped
  
  // update the matrix
  matrix[(*e).rowPos][(*e).colPos] = 2;
  // light the leds
  lc.setLed(0, (*e).rowPos, (*e).colPos, 1);
}

void createBomb(elem *e) {
  (*e).lastMove = millis();
  (*e).type = 2;
  (*e).alive = true;
  (*e).rowPos = (*e).prevRowPos = 0; // starting row is 0, will grow in moveElement
  do {
    (*e).colPos = (*e).prevColPos = rand()%7; // starting col is a random number between 0 and 6
  }while (matrix[(*e).rowPos][(*e).colPos] && 
          matrix[(*e).rowPos][(*e).colPos + 1] && 
          matrix[(*e).rowPos + 1][(*e).colPos] && 
          matrix[(*e).rowPos + 1][(*e).colPos + 1] && 
          matrix[(*e).rowPos][(*e).colPos - 1] && 
          matrix[(*e).rowPos][(*e).colPos + 2] && 
          matrix[(*e).rowPos + 2][(*e).colPos] && 
          matrix[(*e).rowPos + 2][(*e).colPos + 1]); // verify that the position of the new element and his surroundings are not occuped
  
  // update the matrix
  matrix[(*e).rowPos][(*e).colPos] = 3;
  matrix[(*e).rowPos][(*e).colPos + 1] = 3;
  matrix[(*e).rowPos + 1][(*e).colPos] = 3;
  matrix[(*e).rowPos + 1][(*e).colPos + 1] = 3;
  // light the leds
  lc.setLed(0, (*e).rowPos, (*e).colPos, 1);
  lc.setLed(0, (*e).rowPos, (*e).colPos + 1, 1);
  lc.setLed(0, (*e).rowPos + 1, (*e).colPos, 1);
  lc.setLed(0, (*e).rowPos + 1, (*e).colPos + 1, 1);
}

void addNewElement(int type, elem listOfElements[20], int *n) {
    // Possible types:
    // 1 => 1xfood, 2 => 2xfood, 3 => 3xfood, 4 => 1xfood & 1xbomb, 5 => 2xfood & 1xbomb, 6 =>1xbomb
    // 7 => 2xfood, 8 => 2xfood & 1xbomb, 9 => 3xfood, 10 => 2xfood & 1xbomb, 11 => 1xfood & 1xbomb, 12 => 1xfood & 2xbomb
    // 13 => 2xfood, 14 => 3xfood, 15 => 4xfood, 16 => 1xfood 2xbomb,  17 => 2xfood 2xbomb,  18 => 2xbomb
    
    // 1xfood
    if (type == 1 || type == 2 || type == 3 || type == 4 || type == 5 ||
        type == 7 || type == 8 || type == 9 || type == 10 || type == 11||
        type == 12 || type == 13 || type == 14 || type == 15 || type == 16 || type == 17) {
       createFood(&listOfElements[*n]);
       (*n)++;
    }

    // 2xfood
    if (type == 2 || type == 3 || type == 5 || type == 7 || type == 8 || type == 9 || type == 10 ||
        type == 13 || type == 14 || type == 15 || type == 17) {
       createFood(&listOfElements[*n]);
       (*n)++;
    }

    // 3xfood
    if (type == 3 || type == 9 || type == 14 || type == 15) {
       createFood(&listOfElements[*n]);
       (*n)++;
    }

    // 4xfood
    if (type == 15) {
       createFood(&listOfElements[*n]);
       (*n)++;
    }
    
    // 1xbomb
    if (type == 4 || type == 5 || type == 6 || type == 8 || type == 10 || 
        type == 11 || type == 12 || type == 16 || type == 17|| type == 18) {
       createBomb(&listOfElements[*n]);
       (*n)++;
   }

   // 2xbomb
    if (type == 12 || type == 16 || type == 17 || type == 18){
       createBomb(&listOfElements[*n]);
       (*n)++;
   }
}

// the intervals of time for creating new elements based on the level:
unsigned long timeOfNewElements1 = 2000;
unsigned long timeOfNewElements2 = 1500;
unsigned long timeOfNewElements3 = 1000;
unsigned long lastTimeOfNewElements = 0;

elem listOfElements[20]; 
int sizeOfListOfElements = 19; // the capacity of the listOfElements array
int nr = 0; // the number of elements added to the list

// Next function deletes the first element of the list, letting the last position free for a new element
void deleteElementFromList (elem listOfElements[20], int sizeList, int *nr) {
  for (int k = 1; k <= sizeList; k++) {
     listOfElements[k - 1].lastMove = listOfElements[k].lastMove;
     listOfElements[k - 1].type = listOfElements[k].type;
     listOfElements[k - 1].rowPos = listOfElements[k].rowPos;
     listOfElements[k - 1].colPos = listOfElements[k].colPos;
     listOfElements[k - 1].prevRowPos = listOfElements[k].prevRowPos;
     listOfElements[k - 1].prevColPos = listOfElements[k].prevColPos;
     listOfElements[k - 1].alive = listOfElements[k].alive;
  }
  (*nr)--;
}

int typeOfElement; // what to create 

void levelFunction(int level) {
  int currentSpeed = (level == 1)? speed1 : ((level == 2)? speed2 : speed3);
  unsigned long timeOfNewElements = (level == 1)? timeOfNewElements1 : ((level == 2)? timeOfNewElements2 : timeOfNewElements3);
  int minFood = (level == 1)? minFood1 : ((level == 2)? minFood2 : minFood3);
  
  if (millis() - lastTimeOfNewElements >= timeOfNewElements) {
    lastTimeOfNewElements = millis();
    typeOfElement = (level == 1)? ((rand()%6) + 1): ((level == 2)? ((rand()%6) + 7) : ((rand()%6) + 13));
    // Possible types:
    // 1 => 1xfood, 2 => 2xfood, 3 => 3xfood, 4 => 1xfood & 1xbomb, 5 => 2xfood & 1xbomb, 6 =>1xbomb
    // 7 => 2xfood, 8 => 2xfood & 1xbomb, 9 => 3xfood, 10 => 2xfood & 1xbomb, 11 => 1xfood & 1xbomb, 12 => 1xfood & 2xbomb
    // 13 => 2xfood, 14 => 3xfood, 15 => 4xfood, 16 => 1xfood 2xbomb,  17 => 2xfood 2xbomb,  18 => 2xbomb
    // lvl 1: type is between 1 and 6
    // lvl 2: type is between 7 and 12
    // lvl 3: type is between 13 and 18
    // rand() % (upper - lower + 1)) + lower;
  
    addNewElement(typeOfElement, listOfElements, &nr); 
   }
   
   // Iterate through the listOfElements array.
   // Move every element who is still alive. If it arrives at the last line delete the first element
   // of the listOfElements. The first element who dies is the first element in the list because the 
   // elements are created at a constant interval of time and move with the same speed.
   for (int j = 0; j < nr; j++) { 
    if (listOfElements[j].alive) {
      if (!moveElement(&listOfElements[j], currentSpeed)) {
        listOfElements[j].alive = false; 
        deleteElementFromList(listOfElements, sizeOfListOfElements, &nr);
       }
      if (!game) { initialization(); break; } // verify if the number of lifes is 0 after moving an element
     }
   }
   
   // verify if the time for a level is over
   if (timeSincePlaying + 1 >= playingTime/1000) {
    if (currentScore >= minFood) {
      displayGameWon();
      
      if (level == 3) {
        game = false; 
        initialization();
      }
      else timeSinceStart = millis(); // if the game is not finished, update the time of starting for a new level 
    }
    else {
      displayGameLost(0);  // game lost because of the score
      initialization(); 
    }
   }
}

void matrixGameOn() {
  if (millis() - timeSinceStart <= playingTime + 1) {
    setPlayerPosition();
    levelFunction(level);
  }
  else {
    lc.clearDisplay(0);
    game = false;
  }
}

void setup() {
  // setup for the lcd
  lcd.begin(16, 2);
  lcdWelcome();
  
  // setup for the joystick
  pinMode(pinSw, INPUT_PULLUP);
  pinMode(pinX, INPUT);
  pinMode(pinY, INPUT);
  
  // setup for the matrix
  lc.shutdown(0, false);
  lc.setIntensity(0, 2);
  lc.clearDisplay(0);

  Serial.begin(9600);
}

void loop() {
  if (!game) {
    lcdGameOff();
    matrixGameOff();
  }
  else {
    lcdGameOn();
    matrixGameOn();
  }
}
