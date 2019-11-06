const int redPinIn = A0;
const int greenPinIn = A1;
const int bluePinIn = A2; 

const int redPinOut = 11;
const int greenPinOut = 10;
const int bluePinOut = 9;

int ledRed = 0;
int ledGreen = 0;
int ledBlue = 0;

int redValue = 0;
int greenValue = 0;
int blueValue = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(redPinIn, INPUT);
  pinMode(greenPinIn, INPUT);
  pinMode(bluePinIn, INPUT);

  pinMode(redPinOut, OUTPUT);
  pinMode(greenPinOut, OUTPUT);
  pinMode(bluePinOut, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  ledRed = analogRead(redPinIn);
  ledGreen = analogRead(greenPinIn);
  ledBlue = analogRead(bluePinIn);

  redValue = map(ledRed, 0, 1023, 0, 255);
  greenValue = map(ledGreen, 0, 1023, 0, 255);
  blueValue = map(ledBlue, 0, 1023, 0, 255);

  setColor(redValue, greenValue, blueValue);
}

void setColor(int r, int g, int b)
{
  analogWrite(redPinOut, r);
  analogWrite(greenPinOut, g);
  analogWrite(bluePinOut, b);
}
