#include <Arduino.h>
#include "nRF24L01.h" //NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

#define SwitchPin 2 

int SentMessage[1] = {000}; 
RF24 radio(9,10); // NRF24L01 used SPI pins + Pin 9 and 10 on the NANO

const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01 

void setup()
{
  pinMode(SwitchPin, INPUT_PULLUP); 
  digitalWrite(SwitchPin,HIGH); 
  pinMode(4,OUTPUT);
  radio.begin(); // Start the NRF24L01
  radio.openWritingPipe(pipe); // Get NRF24L01 ready to transmit
}

void loop()
{
  if (digitalRead(SwitchPin) == LOW)    // If switch is pressed
  { 
      SentMessage[0] = 111;
      radio.write(SentMessage, 1);  
        digitalWrite(4,HIGH); 
    // Send pressed data to NRF24L01
  }
  else 
  {
      SentMessage[0] = 000;
      radio.write(SentMessage, 1);     
        digitalWrite(4,LOW); 
 // Send idle data to NRF24L01
  }
}
// #include<SPI.h>
// #include<nRF24L01.h>  
// #include<RF24.h>


// RF24 radio(9,10); // CE, CSN

// const uint64_t pipe = 0xE6E6E6E6E6E6;

// //const char message[]="Hello";


// void setup() {
//   Serial.begin(9600);

//   pinMode(2,INPUT_PULLUP);
//   digitalWrite(2,HIGH);
//   radio.begin();
//   radio.openWritingPipe(pipe);
//   //radio.stopListening();
// }
// void loop() {
//   // bool button = (LOW==digitalRead(2));

//   // if(HIGH==button){
//   // radio.write(&button, sizeof(button));  
//   // }
//   // //const char text[] = "Hello World";
//   // Serial.print(digitalRead(2));

//   // radio.write(&button, sizeof(button));
//   // delay(100);

// }


// // int led =4;
// // int Delay =10;

// // RF24 radio_transmitter(8,10);//CE,CSN

// // const uint64_t address = 0xFFF28FFA;

// // bool led_state=false;



// // void setup() {
// // radio_transmitter.begin();
// // radio_transmitter.openWritingPipe(address);
// // radio_transmitter.setPALevel(RF24_PA_HIGH);
// // radio_transmitter.stopListening();


// // //DEBUG
// // pinMode(led,OUTPUT);
// // pinMode(2,INPUT_PULLUP);
// // }



// // void loop() {
// // //radio_transmitter.write(&message,sizeof(message));
// // if(LOW==digitalRead(2)){
// // led_state = true;
// // while(LOW==digitalRead(2)){

// // radio_transmitter.write(&led_state, sizeof(led_state));

// // digitalWrite(led,HIGH);
// // delay(Delay);
// // }}
// // led_state = false;

// // while(!(LOW==digitalRead(2))){

// // radio_transmitter.write(&led_state, sizeof(led_state));

// // digitalWrite(led,LOW);
// // delay(Delay);
// // }
// // }
