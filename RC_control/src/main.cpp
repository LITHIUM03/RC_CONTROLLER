#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "C:\Users\Elad\Documents\PlatformIO\Projects\RC_Project\shared\dataPackage.h"

/*~~~~~~~~~~PINS~~~~~~~~~~*/
#define THROTTLE_PIN A0 
#define STEERING_PIN A1
#define BUTTON_PIN 2 
#define HEADLIGHTS_PIN  6
//#define D_R_PIN

/*~~~~~~~~FUNCTIONS~~~~~~~*/

/*~~~~~~~~CONSTANTS~~~~~~~*/
#define MIN_THROTTLE 0
#define MAX_THROTTLE 1023

/*~~~~~~~~~~RADIO~~~~~~~~~*/
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

data_package current_data;

unsigned long current_millis = 0;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(THROTTLE_PIN, INPUT);
  pinMode(STEERING_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

}

void loop(){

current_millis = millis();
int current_throttle=analogRead(THROTTLE_PIN);
current_data.base_speed = myMap(current_throttle,MIN_THROTTLE,MAX_THROTTLE);
current_data.steering = map(analogRead(STEERING_PIN),41,951,-15,15); //TODO
current_data.headlights = (HIGH == digitalRead(HEADLIGHTS_PIN) ? ON : OFF) ;
current_data.d_r = (analogRead(THROTTLE_PIN)>=(MIN_THROTTLE+MAX_THROTTLE)/2 ? FORWARD : REVERSE) ;
radio.write(&current_data, sizeof(current_data));
current_data.print();
Serial.print("THROTTLE_PIN: ");
Serial.print(analogRead(THROTTLE_PIN));
Serial.print(" | ");
Serial.print("STEERING_PIN: ");
Serial.print(analogRead(STEERING_PIN));
Serial.print(" | ");
delay(1);
}
