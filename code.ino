// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++ Mini Project using Arduino Due ++++++++++++++++
// ++++++++ Functions: Door Open/Close and Lights Control +++++++
// ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

/***********************************************************************************
 * Copyright (c) April 2017 - present Sujaykumar.Hublikar - All Rights Reserved    *
 * Unauthorized copying of this file, via any medium is strictly prohibited        *
 * Proprietary and confidential                                                    *
 * Written by Sujaykumar.Hublikar  <hello@sujaykumarh.com> April 2017           *
 ***********************************************************************************/

#include <LiquidCrystal.h>
//  LCD Declaration
LiquidCrystal lcd(40, 38, 36, 34, 32, 30);

// Pin Declaration
const char PIN_LED_LIGHT = 2;
const char PIN_LED_DOOR_CLOSE = 3;
const char PIN_LED_DOOR_OPEN = 4;

const char PIN_SW_DOOR_OPEN = 50;
const char PIN_SW_DOOR_CLOSE = 52;

const char PIN_MOTOR_1 = 6;
const char PIN_MOTOR_2 = 7;

// Global Varialbles
String serialCommand = "";
String statusLight = "";
bool initialRun = false;
int countDownTimer = 5;

char STATUS_LED_LIGHT = LOW;
char STATUS_LED_DOOR_OPEN = LOW;
char STATUS_LED_DOOR_CLOSE = LOW;

void setup(){
  Serial.begin(9600); // Default Baud rate 9600
  // LCD Configuration
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Initilizing...");
  delay(3000);

  // Pin Configuration
  // LED
  pinMode(PIN_LED_LIGHT, OUTPUT);
  pinMode(PIN_LED_DOOR_CLOSE, OUTPUT);
  pinMode(PIN_LED_DOOR_OPEN, OUTPUT);
  // MOTOR
  pinMode(PIN_MOTOR_1, OUTPUT);
  pinMode(PIN_MOTOR_2, OUTPUT);

  // SWITCH
  pinMode(PIN_SW_DOOR_OPEN, INPUT);
  pinMode(PIN_SW_DOOR_CLOSE, INPUT);

  // Default Values
  digitalWrite(PIN_LED_LIGHT, LOW);
  digitalWrite(PIN_LED_DOOR_CLOSE, LOW);
  digitalWrite(PIN_LED_DOOR_OPEN, LOW);
}

void checkDoorStatus(){
  if(digitalRead(PIN_SW_DOOR_CLOSE) != HIGH){
      statusLight = "Closing in ";
      statusLight += countDownTimer;
      showLCD(statusLight);
      if(countDownTimer <= 0) {
        initialRun = true;
        doorClose();
      }
  }else{
    initialRun = true;
    showLCD("Door CLOSED");
  }
}

void loop(){
  digitalWrite(PIN_LED_LIGHT, STATUS_LED_LIGHT);
  digitalWrite(PIN_LED_DOOR_OPEN, STATUS_LED_DOOR_OPEN);
  digitalWrite(PIN_LED_DOOR_CLOSE, STATUS_LED_DOOR_CLOSE);

  STATUS_LED_DOOR_OPEN = digitalRead(PIN_SW_DOOR_OPEN);
  STATUS_LED_DOOR_CLOSE = digitalRead(PIN_SW_DOOR_CLOSE);

  if(!initialRun){
    checkDoorStatus();
    delay(1000);
    countDownTimer--;
  }else{
    if(Serial.available()){
      readCommand();
    }
  }
}

void readCommand(){
  serialCommand = "";
  serialCommand = Serial.readString(); // Generate String form serial input
  //Serial.println("Command: " + serialCommand);
  handleCommand();
}

void handleCommand(){
  // command "open" to open door
  if(serialCommand == "open"){
    if(digitalRead(PIN_SW_DOOR_OPEN) == HIGH){
      Serial.println("Door is OPEN");
    }else{
      doorOpen();
    }
  }

  // command "close" to close door
  if(serialCommand == "close"){
    if(digitalRead(PIN_SW_DOOR_CLOSE) == HIGH){
      Serial.println("Door is CLOSED");
    }else{
      doorClose();
    }
  }

  // command "lon" to trun ON lights
  if(serialCommand == "lon"){
    Serial.println("Lights ON");
    STATUS_LED_LIGHT = HIGH;
    updateLCD();
  }

  // command "loff" to trun OFF lights
  if(serialCommand == "loff"){
    Serial.println("Lights OFF");
    STATUS_LED_LIGHT = LOW;
    updateLCD();
  }
}

// open door function
void doorOpen(){
  Serial.println("Opening DOOR");
  showLCD("Door Opening");
  while(digitalRead(PIN_SW_DOOR_OPEN) != HIGH){
    digitalWrite(PIN_MOTOR_1, LOW);
    digitalWrite(PIN_MOTOR_2, HIGH);
  }
  showLCD("Door Open");
  Serial.println("Door OPEN");
  digitalWrite(PIN_MOTOR_1, LOW);
  digitalWrite(PIN_MOTOR_2, LOW);
}

// close door function
void doorClose(){
  Serial.println("Closing DOOR");
  showLCD("Door Closing");

  while(digitalRead(PIN_SW_DOOR_CLOSE) != HIGH){
    digitalWrite(PIN_MOTOR_1, HIGH);
    digitalWrite(PIN_MOTOR_2, LOW);
  }

  showLCD("Door Closed");
  Serial.println("Door CLOSED");

  digitalWrite(PIN_MOTOR_1, LOW);
  digitalWrite(PIN_MOTOR_2, LOW);
}

// LCD display function LINE 1
void showLCD(String msg){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(msg);
  updateLCD();
}

// LCD display function LINE 2
void updateLCD(){
  if(STATUS_LED_LIGHT == HIGH){
    statusLight = "Lights ON";
  }else{
    statusLight = "Lights OFF";
  }
  lcd.setCursor(0,1);
  lcd.print("            ");
  lcd.setCursor(0,1);
  lcd.print(statusLight);
}
