#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define BUTTON_PIN 2 
RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

int msg[1];

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(BUTTON_PIN,INPUT_PULLUP);
}

void loop(){

if (digitalRead(BUTTON_PIN) == LOW){
msg[0] = 111;
radio.write(msg, 1);
Serial.print("inside tx IF\n");
}
else{
msg[0] = 000;
radio.write(msg, 1);
Serial.print("inside tx loop, not IF\n");
}
//delay(50);
}