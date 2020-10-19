#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Button inputs Pin 2-5
const int redIn = 2;
const int blueIn = 3;
const int yellowIn = 4;
const int greenIn = 5;

//Button outputs Pin 6-9
const int redOut = 6;
const int blueOut = 7;
const int yellowOut = 8;
const int greenOut = 9;

//Button Readings
int redLastState = LOW;
int redLastUpdatedTime = 0;
int blueLastState = LOW;
int blueLastUpdatedTime = 0;
int yellowLastState = LOW;
int yellowLastUpdatedTime = 0;
int greenLastState = LOW;
int greenLastUpdatedTime = 0;

//Other Constants
const int debounceTime = 50;
const int buzzerOut = 10;
const int redToneFrequency = 262;
const int blueToneFrequency = 330;
const int yellowToneFrequency = 392;
const int greenToneFrequency = 494;
const int levelUpToneFrequency = 587;
const int gameOverToneFrequency = 220;
const int gameWonToneFrequency = 698;
const int screenWidth = 128;
const int screenHeight = 32;
const int screenPin = 11;
Adafruit_SSD1306 display(screenWidth, screenHeight, &Wire, screenPin);

//Game Variables
int sequenceArray[20]{0};
int level = 1;
int inputCounter = 0;
int timer = 2000;
int finishedDisplayTime = 0;
bool gameLost = false;



void setup() {
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  
  pinMode(redIn, INPUT);
  pinMode(blueIn, INPUT);
  pinMode(yellowIn, INPUT);
  pinMode(greenIn, INPUT);

  pinMode(redOut, OUTPUT);
  pinMode(blueOut, OUTPUT);
  pinMode(yellowOut, OUTPUT);
  pinMode(greenOut, OUTPUT);

  digitalWrite(redOut, LOW);
  digitalWrite(blueOut, LOW);
  digitalWrite(yellowOut, LOW);
  digitalWrite(greenOut, LOW);

  pinMode(buzzerOut, OUTPUT);

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(1, 0);
  display.println("Arduino Simon");
  display.setTextSize(1);
  display.println("Created by Xi Zhu");
  display.println("Repeat the sequence shown");
  display.println("Get to level 20 to win");
  display.display();

  sequenceArray[level-1] = random(1,5);
  
  delay(1000);
}

void reset(){
  for (int i = 0; i < 20; ++i) {
    sequenceArray[i] = 0;
  }
  level = 1;
  inputCounter = 0;
  timer = 2000;
}

void loop() {
  // put your main code here, to run repeatedly:
  if (gameLost) {
    tone(buzzerOut, gameOverToneFrequency);
    display.setTextSize(1);
    display.println("Game Over");
    display.display();
    reset();
    delay(100);
    noTone(buzzerOut);
  }

  display.setTextSize(1);
  display.print("Level: ");
  display.println(level);
  display.display();
  
  for (int i = 0; i < level; ++i) {
    switch (sequenceArray[i]) {
      case 1:
      //red
        digitalWrite(redOut, HIGH);
        tone(buzzerOut, redToneFrequency);
        delay(500);
        digitalWrite(redOut, LOW);
        noTone(buzzerOut);
        break;
      case 2:
      //blue
        digitalWrite(blueOut, HIGH);
        tone(buzzerOut, blueToneFrequency);
        delay(500);
        digitalWrite(blueOut, LOW);
        noTone(buzzerOut);
        break;
      case 3:
      //yellow
        digitalWrite(yellowOut, HIGH);
        tone(buzzerOut, yellowToneFrequency);
        delay(500);
        digitalWrite(yellowOut, LOW);
        noTone(buzzerOut);
        break;
      case 4:
      //green
        digitalWrite(greenOut, HIGH);
        tone(buzzerOut, greenToneFrequency);
        delay(500);
        digitalWrite(greenOut, LOW);
        noTone(buzzerOut);
        break;
    }
  }
  
  finishedDisplayTime = millis();
  while (millis() - finishedDisplayTime < timer && inputCounter < level){

  //debounce red
    int redReading = digitalRead(redIn);
    if (redReading != redLastState){
      redLastUpdatedTime = millis();
    }

    if ((millis() - redLastUpdatedTime) > debounceTime){
      if (redReading == HIGH){
        if (sequenceArray[inputCounter] == 1){
        tone(buzzerOut, redToneFrequency);
        ++inputCounter;
        delay(100);
        noTone(buzzerOut);
      } else {
        gameLost = true;
        break;
      }
    }
  }
  redLastState = redReading;

  //debounce blue
  int blueReading = digitalRead(blueIn);
    if (blueReading != blueLastState){
      blueLastUpdatedTime = millis();
    }

    if ((millis() - blueLastUpdatedTime) > debounceTime){
      if (blueReading == HIGH){
        if (sequenceArray[inputCounter] == 2){
        tone(buzzerOut, blueToneFrequency);
        ++inputCounter;
        delay(100);
        noTone(buzzerOut);
      } else {
        gameLost = true;
        break;
      }
    }
  }
  blueLastState = blueReading;

  //debounce yellow
  int yellowReading = digitalRead(yellowIn);
    if (yellowReading != yellowLastState){
      yellowLastUpdatedTime = millis();
    }

    if ((millis() - yellowLastUpdatedTime) > debounceTime){
      if (yellowReading == HIGH){
        if (sequenceArray[inputCounter] == 3){
        tone(buzzerOut, yellowToneFrequency);
        ++inputCounter;
        delay(100);
        noTone(buzzerOut);
      } else {
        gameLost = true;
        break;
      }
    }
  }
  yellowLastState = yellowReading;

  //debounce green
  int greenReading = digitalRead(greenIn);
    if (greenReading != greenLastState){
      greenLastUpdatedTime = millis();
    }

    if ((millis() - greenLastUpdatedTime) > debounceTime){
      if (greenReading == HIGH){
        if (sequenceArray[inputCounter] == 4){
        tone(buzzerOut, blueToneFrequency);
        ++inputCounter;
        delay(100);
        noTone(buzzerOut);
      } else {
        gameLost = true;
        break;
      }
    }
  }
  greenLastState = greenReading;
  
  }

  tone(buzzerOut, levelUpToneFrequency);
  timer += 2000;
  level += 1;
  inputCounter = 0;
  if (level == 21){
    tone(buzzerOut, gameWonToneFrequency);
    display.setTextSize(1);
    display.println("You Won");
    display.display();
    reset();
    delay(100);
    noTone(buzzerOut);
  }
  sequenceArray[level - 1] = random(1,5);
  delay(100);
  noTone(buzzerOut);

  
}
