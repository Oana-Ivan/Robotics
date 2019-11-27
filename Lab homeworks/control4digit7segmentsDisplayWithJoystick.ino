// pins for joystick
const int pinSW = 1; 
const int pinX = A0; 
const int pinY = A1; 

// values for the joystick
int switchValue;
int xValue = 0;
int yValue = 0;

// pins for the 4 digits 7 segments display
const int pinA = 12;
const int pinB = 8;
const int pinC = 5;
const int pinD = 3;
const int pinE = 2;
const int pinF = 11;
const int pinG = 6;
const int pinDP = 4;
const int pinD1 = 7;
const int pinD2 = 9;
const int pinD3 = 10;
const int pinD4 = 13;

const int segSize = 8;
int segments[segSize] = {pinA, pinB, pinC, pinD, pinE, pinF, pinG, pinDP};

const int noOfDisplays = 4;
int digits[noOfDisplays] = {pinD1, pinD2, pinD3, pinD4};

const int noOfDigits = 10;
byte digitMatrix[noOfDigits][segSize - 1 ] = {
{ 1, 1, 1, 1, 1, 1, 0 }, // 0
{ 0, 1, 1, 0, 0, 0, 0 }, // 1
{ 1, 1, 0, 1, 1, 0, 1 }, // 2
{ 1, 1, 1, 1, 0, 0, 1 }, // 3
{ 0, 1, 1, 0, 0, 1, 1 }, // 4
{ 1, 0, 1, 1, 0, 1, 1 }, // 5
{ 1, 0, 1, 1, 1, 1, 1 }, // 6
{ 1, 1, 1, 0, 0, 0, 0 }, // 7
{ 1, 1, 1, 1, 1, 1, 1 }, // 8
{ 1, 1, 1, 1, 0, 1, 1 }  // 9
};

int currentDisplay = 0;
int numbers[noOfDisplays] = {0, 0, 0, 0};

// constants for X axis
const int zoneMinX = 400;
const int zoneMaxX = 600;

// constants for Y axis
const int zoneMinY = 400;
const int zoneMaxY = 600;

bool joyMoved = false;
bool switchState = false; //true pressed, false unpressed

void displayNumber(byte digit, byte decimalPoint) {
  for (int i = 0; i < segSize - 1; i++) {
    digitalWrite(segments[i], digitMatrix[digit][i]);
  }
  digitalWrite(segments[segSize - 1], decimalPoint);
 }

void showDigit(int num) {
  for(int i = 0; i < noOfDisplays; i++) {
    digitalWrite(digits[i], HIGH);
  }
  digitalWrite(digits[num], LOW);
  displayNumber(numbers[num], LOW);
}


void setup() {
  pinMode(pinSW, INPUT_PULLUP);
  for (int i = 0; i < segSize; i++){
    pinMode(segments[i], OUTPUT);
  }
  for (int i = 0; i < noOfDisplays; i++){
    pinMode(digits[i], OUTPUT);
  }
  showDigit(currentDisplay);
}

void loop() {
  //reading the values from joystick
  switchValue = digitalRead(pinSW);
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
  
  if (switchValue == LOW && switchState == false){
    switchState = true;
    showDigit(currentDisplay);
    digitalWrite(segments[segSize - 1], LOW); // write the decimal point of the current display 
  }
  else if (switchValue == LOW){
    switchState = false;
    showDigit(currentDisplay);
    digitalWrite(segments[segSize - 1], LOW);  
  }
  //if the button was not pressed
  if (switchState == false) {
    if (xValue < zoneMinX && joyMoved == false){
      if (currentDisplay == 0) currentDisplay = noOfDisplays - 1;
      else currentDisplay--;
      joyMoved == true;
    }
    if (xValue > zoneMaxX && joyMoved == false){
      if (currentDisplay == noOfDisplays - 1) currentDisplay = 0;
      else currentDisplay++;
      joyMoved == true;
    }
    if (xValue >= zoneMinX && xValue <= zoneMaxX)
      joyMoved = false;
    showDigit(currentDisplay);
    displayNumber(numbers[currentDisplay], LOW);
  }
  else{ //if the button was pressed
    if (yValue > zoneMaxY && joyMoved == false){
      if (numbers[currentDisplay] > 0){
        numbers[currentDisplay]--;
       }
       else{
        numbers[currentDisplay] = 9;
        }
       joyMoved = true;
    }
    if (yValue < zoneMinY && joyMoved == false){
      if (numbers[currentDisplay] < 9){
        numbers[currentDisplay]++;
      }
      else{
        numbers[currentDisplay] = 0;
      }
      joyMoved = true;
    }
    if (yValue > zoneMinY && yValue < zoneMaxY){
        joyMoved == false;
    }
    showDigit(currentDisplay);
    displayNumber(numbers[currentDisplay], LOW);
  }
}
