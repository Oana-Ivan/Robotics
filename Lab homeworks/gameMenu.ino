#include <LiquidCrystal.h>
const int RS = 12;
const int enable = 11; 
const int d4 = 5;
const int d5 = 4;
const int d6 = 3;
const int d7 = 2;
LiquidCrystal lcd(RS, enable, d4, d5, d6, d7);

const int pinSw = 6;
const int pinX = A0; 
const int pinY = A1;

int swValue;
int xValue;
int yValue;

int minThreshold = 400;
int maxThreshold = 600;
int buttonState = 0;

int currentPosition = 0;
int startingLevelValue = 0;
int score = 0;
bool joyMoved = false;

void showMenu();

void displayStart() {
  
  lcd.setCursor(0, 0); lcd.println("Lives: 3");
  lcd.setCursor(0, 1); lcd.print("Level: ");
  lcd.println(startingLevelValue);

  int level = startingLevelValue;
  int gameOn = 1, congratulationsOn = 1;
  unsigned long gameTime = 10000;
  unsigned long incrementLevelTime = 2000;
  unsigned long congratulationsTime = 2000;
  unsigned long currentTime;
  unsigned long currentTimeLevel;
  
  currentTime = millis();
  currentTimeLevel = millis();
  
  while (gameOn == 1) {
    
    if (millis() - currentTime >= gameTime)
        gameOn = 0;
    if (millis() - currentTimeLevel >= incrementLevelTime) {
        level++;
        lcd.setCursor(7, 1);
        lcd.print(level);
        currentTimeLevel = millis();
     }
  }
  
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Congratulations!");
  
  currentTime = millis();
  while (congratulationsOn == 1) {
    if (millis() - currentTime > congratulationsTime)
        congratulationsOn = 0;
  }

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("You finished");
  lcd.setCursor(0, 1); lcd.print(" the game!");
  
  currentTime = millis();
  congratulationsOn = 1;
  while (congratulationsOn == 1) {
    if (millis() - currentTime > congratulationsTime)
        congratulationsOn = 0;
  }
  
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Press button");
  lcd.setCursor(0, 1); lcd.print("to exit!");
  
  score = level * 3;

  int buttonPressed = 0;
  while (buttonPressed == 0) {
    swValue = digitalRead(pinSw);
    if (swValue == LOW) buttonPressed = 1;
  }
  
  delay(2000);
  lcd.clear();
  showMenu();
}

void displayScore() {
  
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("HighScore:");
  lcd.setCursor(0, 1); lcd.print(score);
  
  int display = 1;
  unsigned long displayTime = 3000;
  unsigned long currentTime = millis();
  
  while (display == 1) {
    if (millis() - currentTime >= displayTime)
        display = 0;
  }
  
  lcd.clear();
  showMenu();
}

void displaySettings() {
  
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Starting level: ");
  
  int buttonPressed = 0;
  int maxLevel = 99;
  joyMoved == false;
  
  while (buttonPressed == 0) {
    
    xValue = analogRead(pinX);
    
    if (xValue < minThreshold && joyMoved == false) {
      if (startingLevelValue > 0) startingLevelValue--;
      else startingLevelValue = 0;
      joyMoved = true;
    }
    
    if (xValue > maxThreshold && joyMoved == false) {
      if (startingLevelValue < maxLevel) startingLevelValue++;
      else startingLevelValue = 0;
      joyMoved = true;
    }
    
    if (xValue >= minThreshold && xValue <= maxThreshold) {
      joyMoved = false;
    }
    
    lcd.setCursor(0, 1); lcd.print(startingLevelValue);
    if (startingLevelValue < 10) { // delete any possible residual value 
      lcd.setCursor(1, 1); 
      lcd.print(" ");
    }
    
    swValue = digitalRead(pinSw);
    if (swValue == LOW) {
      buttonPressed = 1;
      //break;
    }
  }
  
  lcd.clear();
  showMenu();
}

void showOption(int p) {
  lcd.clear();
  if (p == 0) displayStart();
  else if (p == 1) displayScore();
       else if (p == 2) displaySettings();
}

void showMenu() {

  lcd.setCursor(1, 0); lcd.print("Start");
  lcd.setCursor(9, 0); lcd.print("Score");
  lcd.setCursor(1, 1); lcd.print("Settings");
  
  swValue = digitalRead(pinSw);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  
  buttonState = swValue;
  
  if (yValue > maxThreshold && joyMoved == false) {
    if (currentPosition < 2) currentPosition++;
    else currentPosition = 0;
    joyMoved = true;
  }
  if (yValue < minThreshold && joyMoved == false) {
    if (currentPosition > 0) currentPosition--;
    else currentPosition = 2;
    joyMoved = true;
  }
  if (yValue >= minThreshold && yValue <= maxThreshold) {
    joyMoved = false;
  }

  // delete the last arrows printed
  lcd.setCursor(0, 0); lcd.print(" ");
  lcd.setCursor(8, 0); lcd.print(" ");
  lcd.setCursor(0, 1); lcd.print(" ");
  
  if (currentPosition == 0) {
      lcd.setCursor(0, 0);
      lcd.print(">");
  }
  else if (currentPosition == 1) {
          lcd.setCursor(8, 0);
          lcd.print(">");
        }
      else if (currentPosition == 2) {
             lcd.setCursor(0, 1);
             lcd.print(">");
            }
  if (buttonState == LOW) {
      lcd.clear();
      showOption(currentPosition);
  }
}
void setup(){
  
 pinMode(pinSw, INPUT_PULLUP);
 pinMode(pinX, INPUT);
 pinMode(pinY, INPUT);
 
 lcd.begin(16, 2);
}

void loop() {
  
  showMenu();
}
