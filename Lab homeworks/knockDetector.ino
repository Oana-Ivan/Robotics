
const int speakerPin = A0;
int speakerValue = 0;
const int threshold = 1;
unsigned long intervalBeforeSound = 5000;
unsigned long lastKnock = 0;
int knockValue = 0;
unsigned long currentTime = 0;

const int buzzerPin = 11;
unsigned int buzzerFrequency = 3000;
int buzzerValue = 0;

const int pushButton = 2;
int buttonState = 0;

void setup () {
  pinMode(speakerPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(pushButton, INPUT_PULLUP);
  Serial.begin(9600);
}
void loop () {
  speakerValue = analogRead(speakerPin);
  if (speakerValue != 0)
    Serial.println(speakerValue);
  
  if (speakerValue > threshold) {
    Serial.println("Knock");
    knockValue = 1;
  }
  currentTime = millis();
  
  if((currentTime - lastKnock >= intervalBeforeSound) && knockValue == 1){
    lastKnock = currentTime;
    tone(buzzerPin, buzzerFrequency);
  }
  
  buttonState = digitalRead(pushButton);
  if(!buttonState){
    noTone(buzzerPin);
    knockValue = 0;
  }
 
}
