#include <Arduino.h>
#include <Servo.h>
#include "PinDefinitionsAndMore.h"
#include <IRremote.hpp>
#include "Bonezegei_ULN2003_Stepper.h"
#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 10, d5 = 9, d6 = 8, d7 = 7;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

Bonezegei_ULN2003_Stepper Stepper(A0, A1, A2, A3);

Servo myservo;
Servo myservo1;
Servo myservo2;
Servo myservo3;

int currentServo = 0;

char data[5];
int pos = 10;
int pos1 = 20;
int pos2 = 165;
int firstTime = 1;

#define FORWARD 1
#define REVERSE 0
int degrees;
int steps;
int servoValue = 22;

void setup() {
  degrees = 0;
  Serial.begin(9600);

  Stepper.begin();
  Stepper.setSpeed(2);

  lcd.begin(16, 2);
  //lcd.print("hello, world!");

  IrReceiver.begin(2, ENABLE_LED_FEEDBACK);

  myservo.attach(5);
  myservo1.attach(3);
  myservo2.attach(4);
  myservo3.attach(6);

  myservo.write(pos); 
  myservo1.write(pos1); 
  myservo2.write(pos2); 
}

void loop() {
  decodeCommand();
  lcd.setCursor(0, 0);
  lcd.print("N:" + (String)currentServo + " S:" + (String)degrees + " M0:" + (String)pos);
  lcd.setCursor(0, 1);
  lcd.print("M1:" + (String)pos1 + " M2:" + (String)pos2);
  //uint16_t command = decodeCommand();
  //Serial.println("Command is:" + (String)command);
  // char aa = Serial.read();
  
  // if (aa == '0') {
  //   char bb = Serial.read();
  //   if (bb == '+') {
  //     if (pos < 180) {
  //       pos = pos + 5;
  //     }
  //   }
  //   if (bb == '-') {
  //     if (pos > 0) {
  //     pos = pos - 5;
  //     }
  //   }
  //   myservo.write(pos);
  //   Serial.println("Servo0 = " + (String)pos);
  // }

  // if (aa == '1') {
  //   char bb = Serial.read();
  //   if (bb == '+') {
  //     if (pos1 < 180) {
  //       pos1 = pos1 + 5;
  //     }
  //   }
  //   if (bb == '-') {
  //     if (pos1 > 0) {
  //     pos1 = pos1 - 5;
  //     }
  //   }
  //   myservo1.write(pos1);
  //   Serial.println("Servo1 = " + (String)pos1);
  // }

  // if (aa == '2') {
  //   char bb = Serial.read();
  //   if (bb == '+') {
  //     if (pos2 < 180) {
  //       pos2 = pos2 + 5;
  //     }
  //   }
  //   if (bb == '-') {
  //     if (pos2 > 0) {
  //     pos2 = pos2 - 5;
  //     }
  //   }
  //   myservo2.write(pos2);
  //   Serial.println("Servo2 = " + (String)pos2);
  // }
  delay(1); 
}

uint16_t decodeCommand() {
  if (IrReceiver.decode()) {
    if (IrReceiver.decodedIRData.protocol == UNKNOWN) {
        //Serial.println(F("Received noise or an unknown (or not yet enabled) protocol"));
        //IrReceiver.printIRResultRawFormatted(&Serial, true);

        IrReceiver.resume();
    } else {
        IrReceiver.resume();

        IrReceiver.printIRResultShort(&Serial);
        //IrReceiver.printIRSendUsage(&Serial);
        //return IrReceiver.decodedIRData.command;
    }
    //Serial.println();

    if (IrReceiver.decodedIRData.flags & IRDATA_FLAGS_IS_REPEAT) {
        Serial.println(F("Repeat received. Here you can repeat the same action as before."));
        if (IrReceiver.decodedIRData.command == 0x18) {
            Serial.println(F("Received command 0x10."));
            if (currentServo == 0) {
              if (pos < 180) {
                pos = pos + 1;
              }
              myservo.write(pos);
              Serial.println("Servo0 = " + (String)pos);
            }

            if (currentServo == 1) {
              if (pos1 < 180) {
                pos1 = pos1 + 1;
              }
              myservo1.write(pos1);
              Serial.println("Servo1 = " + (String)pos1);
            }
            
            if (currentServo == 2) {
              if (pos2 > 0) {
                pos2 = pos2 - 1;
              }
              myservo2.write(pos2);
              Serial.println("Servo2 = " + (String)pos2);
            }

            if (currentServo == 3) {
              if (degrees < 360) {
                degrees = degrees + 1;
                steps = map(1, 0, 360, 0, 2000);
                Serial.println("Stepper FW" + (String)degrees);
                Stepper.step(FORWARD, steps);
              }
            }
        } else if (IrReceiver.decodedIRData.command == 0x52) {
            Serial.println(F("Received command 0x11."));
            if (currentServo == 0) {
              if (pos > 0) {
                pos = pos - 1;
              }
              myservo.write(pos);
              Serial.println("Servo0 = " + (String)pos);
            }

            if (currentServo == 1) {
              if (pos1 > 0) {
                pos1 = pos1 - 1;
              }
              myservo1.write(pos1);
              Serial.println("Servo1 = " + (String)pos1);
            }
            
            if (currentServo == 2) {
              if (pos2 < 180) {
                pos2 = pos2 + 1;
              }
              myservo2.write(pos2);
              Serial.println("Servo2 = " + (String)pos2);
            }

            if (currentServo == 3) {
              if (degrees > 0) {
                degrees = degrees - 1;
                steps = map(1, 0, 360, 0, 2000);
                Serial.println("stepper" + (String)degrees);
                Stepper.step(REVERSE, steps);
              }
            }
        }
    } else {
        if (IrReceiver.decodedIRData.command == 0x18) {
            Serial.println(F("Received command 0x10."));
            if (currentServo == 0) {
              if (pos < 180) {
                pos = pos + 1;
              }
              myservo.write(pos);
              Serial.println("Servo0 = " + (String)pos);
            }

            if (currentServo == 1) {
              if (firstTime) {
                pos1 = pos1 +20;
                firstTime = 0;
              }
              if (pos1 < 180) {
                pos1 = pos1 + 1;
              }
              myservo1.write(pos1);
              Serial.println("Servo1 = " + (String)pos1);
            }
            
            if (currentServo == 2) {
              if (pos2 > 0) {
                pos2 = pos2 - 1;
              }
              myservo2.write(pos2);
              Serial.println("Servo2 = " + (String)pos2);
            }

            if (currentServo == 3) {
              if (degrees < 360) {
                degrees = degrees + 1;
                steps = map(1, 0, 360, 0, 2000);
                Serial.println("Stepper FW" + (String)degrees);
                Stepper.step(FORWARD, steps);
              }
            }
        } else if (IrReceiver.decodedIRData.command == 0x52) {
            Serial.println(F("Received command 0x11."));
            if (currentServo == 0) {
              if (pos > 0) {
                pos = pos - 1;
              }
              myservo.write(pos);
              Serial.println("Servo0 = " + (String)pos);
            }

            if (currentServo == 1) {
              if (pos1 > 0) {
                pos1 = pos1 - 1;
              }
              myservo1.write(pos1);
              Serial.println("Servo1 = " + (String)pos1);
            }
            
            if (currentServo == 2) {
              if (pos2 < 180) {
                pos2 = pos2 + 1;
              }
              myservo2.write(pos2);
              Serial.println("Servo2 = " + (String)pos2);
            }

            if (currentServo == 3) {
              if (degrees > 0) {
                degrees = degrees - 1;
                steps = map(1, 0, 360, 0, 2000);
                Serial.println("stepper" + (String)degrees);
                Stepper.step(REVERSE, steps);
              }
            }
        } else if (IrReceiver.decodedIRData.command == 0x45) {
          currentServo = 0;
          Serial.println("Current Servo: 0");
        } else if (IrReceiver.decodedIRData.command == 0x46) {
          currentServo = 1;
          Serial.println("Current Servo: 1");
        } else if (IrReceiver.decodedIRData.command == 0x47) {
          currentServo = 2;
          Serial.println("Current Servo: 2");
        } else if (IrReceiver.decodedIRData.command == 0x44) {
          currentServo = 3;
          Serial.println("Current Servo: 3");
        } else if (IrReceiver.decodedIRData.command == 0x16) {
          int posOld = pos;
          int posOld1 = pos1;
          int posOld2 = pos2;
          pos = 10;
          pos1 = 20;
          pos2 = 165;
          if (posOld > pos) {
            for (; posOld > pos; posOld -= 1) { // goes from 180 degrees to 0 degrees
              myservo.write(posOld);              // tell servo to go to position in variable 'pos'
              delay(15);                       // waits 15 ms for the servo to reach the position
            }
          } else {
            for (; posOld < pos; posOld += 1) { // goes from 0 degrees to 180 degrees
              // in steps of 1 degree
              myservo.write(posOld);              // tell servo to go to position in variable 'pos'
              delay(15);                       // waits 15 ms for the servo to reach the position
            }
          }

          if (posOld1 > pos1) {
            for (; posOld1 > pos1; posOld1 -= 1) { // goes from 180 degrees to 0 degrees
              myservo1.write(posOld1);              // tell servo to go to position in variable 'pos'
              delay(15);                       // waits 15 ms for the servo to reach the position
            }
          } else {
            for (; posOld1 < pos1; posOld1 += 1) { // goes from 0 degrees to 180 degrees
              // in steps of 1 degree
              myservo1.write(posOld1);              // tell servo to go to position in variable 'pos'
              delay(15);                       // waits 15 ms for the servo to reach the position
            }
          }

          if (posOld2 > pos2) {
            for (; posOld2 > pos2; posOld2 -= 1) { // goes from 180 degrees to 0 degrees
              myservo2.write(posOld2);              // tell servo to go to position in variable 'pos'
              delay(15);                       // waits 15 ms for the servo to reach the position
            }
          } else {
            for (; posOld2 < pos2; posOld2 += 1) { // goes from 0 degrees to 180 degrees
              // in steps of 1 degree
              myservo2.write(posOld2);              // tell servo to go to position in variable 'pos'
              delay(15);                       // waits 15 ms for the servo to reach the position
            }
          }
          steps = map(degrees, 0, 360, 0, 2000);
          degrees = 0;
          //myservo.write(pos);
          //delay(20);
          //myservo1.write(pos1);
          //delay(20);
          //myservo2.write(pos2);
          //delay(20);
          Stepper.step(REVERSE, steps);
          Serial.println("Servo0 = " + (String)pos);
          Serial.println("Servo2 = " + (String)pos1);
          Serial.println("Servo2 = " + (String)pos2);
          Serial.println("StepM = " + (String)degrees);
          lcd.clear();
          firstTime = 1;
        } 
    }
    lcd.clear();
  }
}
