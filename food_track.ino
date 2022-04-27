#include <SoftwareSerial.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial miBT(6, 7);

const int BUTTON = 4;
const int BUZZER = 5;
const int INTERNAL_CODE_WATING_ORDER = 1;
const int INTERNAL_CODE_REGISTERING_ORDER = 2;
const int INTERNAL_CODE_PREPARING_ORDER = 3;
const int INTERNAL_CODE_READY_ORDER = 4;
const int INTERNAL_CODE_CLOSE_ORDER = 5;

const char ENDER = '#';
const char SOUND_ALARM = '1';

const String CODE_WATING_ORDER = "WAITING_ORDER";
const String CODE_REGISTERING_ORDER = "REGISTERING_ORDER";
const String CODE_PREPARING_ORDER = "PREPARING_ORDER";
const String CODE_READY_ORDER = "READY_ORDER";
const String CODE_CLOSE_ORDER = "CLOSE_ORDER";

char dato;
String message;
int currentState;
boolean transmission = false; 


void setup() {
  Wire.begin();
  Serial.begin(9600);  
  miBT.begin(38400);
  
  pinMode(BUTTON, INPUT);
  pinMode(BUZZER, OUTPUT);

  currentState = INTERNAL_CODE_WATING_ORDER;
}

void loop() {
  if (miBT.available()){
        dato = miBT.read();
        miBT.flush();
        switch (dato){
          case SOUND_ALARM:{
            setUpAlarm();
            break;
          }
      }
    }

    validatePushButton();
}

void setUpAlarm(){
  for (int i = 0; i <= 2; i++) {
    tone(BUZZER, 1000); 
    delay(1000);        
    noTone(BUZZER);     
    delay(1000);
  }
  
}


void validatePushButton(){
  if (digitalRead(BUTTON)==HIGH){
    if (!transmission) {
      switch (currentState) {
        case INTERNAL_CODE_WATING_ORDER: {
          currentState = INTERNAL_CODE_REGISTERING_ORDER;
          message+=CODE_REGISTERING_ORDER;
          break;
        }
        case INTERNAL_CODE_REGISTERING_ORDER: {
          currentState = INTERNAL_CODE_PREPARING_ORDER;
          message+=CODE_PREPARING_ORDER;
          break;
        }
        case INTERNAL_CODE_PREPARING_ORDER: {
          currentState = INTERNAL_CODE_READY_ORDER;
          message+=CODE_READY_ORDER;
          break;
        }
        case INTERNAL_CODE_READY_ORDER: {
          currentState = INTERNAL_CODE_CLOSE_ORDER;
          message+=CODE_CLOSE_ORDER;
          break;
        }
        default: {
          currentState = INTERNAL_CODE_WATING_ORDER;
          message+=CODE_WATING_ORDER;
          break;
        }
      }

      sendMessage();
      
    }
  } else {
    transmission = false;
  }
}

void sendMessage() {
  message+=ENDER;
      miBT.println(message);
      message = "";
      transmission = true;
      delay(1000);
}
