#include <Arduino.h>
#include "nRF24L01.h" // NRF24L01 library created by TMRh20 https://github.com/TMRh20/RF24
#include "RF24.h"
#include "SPI.h"

#define LED_PIN 2

int ReceivedMessage[1] = {000}; // Used to store value received by the NRF24L01
RF24 radio(9,10); // NRF24L01 SPI pins. Pin 9 and 10 on the Nano

const uint64_t pipe = 0xE6E6E6E6E6E6; // Needs to be the same for communicating between 2 NRF24L01 

void setup(void)
{
  radio.begin(); // Start the NRF24L01
  
  radio.openReadingPipe(1,pipe); // Get NRF24L01 ready to receive
  
  radio.startListening(); // Listen to see if information received
  
  pinMode(LED_PIN, OUTPUT); 
}

void loop(void)
{
  while (radio.available())
  {
    Serial.print("kkkk!k");
    radio.read(ReceivedMessage, 1); // Read information from the NRF24L01

    if (ReceivedMessage[0] == 111) // Indicates switch is pressed
    {
      digitalWrite(LED_PIN, HIGH);
    }
    else
    {
       digitalWrite(LED_PIN, LOW);
    }
    delay(2);
   }
}
// #include<SPI.h>
// #include<nRF24L01.h>  
// #include<RF24.h>


// //int led =4;

// //RF24 radio_receiver(8,10);//CE,CSN

// // const uint64_t address = 0xFFF28FFA;
// // //const char message[]="Hello";

// // bool led_state=false;


// // void setup() {
// // pinMode(led,OUTPUT);

// // radio_receiver.begin();
// //   radio_receiver.openReadingPipe(0, address);
// //   radio_receiver.setPALevel(RF24_PA_HIGH);
// //   radio_receiver.startListening();
// // }

// // void loop() {
// // if (radio_receiver.available()){
// // radio_receiver.read(&led_state,sizeof(led_state));
// // if(true==led_state){
//  //   digitalWrite(led,HIGH);

// // }
// // else
// //   digitalWrite(led,LOW);
// // }

// //   // put your main code here, to run repeatedly:
// // }


// RF24 radio(9,10); // CE, CSN

// const uint64_t pipe = 0xE6E6E6E6E6E6;

// void setup() {
//   pinMode(4,OUTPUT); 
//   Serial.begin(9600);
//   radio.begin();
//   radio.openReadingPipe(0, pipe);
//   radio.startListening();

//  // radio.setPALevel(RF24_PA_MAX);
//  // radio.startListening();
// }

// void loop() {
//   if (radio.available()) {
//     bool button;
//     radio.read(&button, sizeof(button));
//     Serial.println(button);
//     Serial.print("radio was available");
//     if(button){ 
//       digitalWrite(4,HIGH);
//     }
//     else{
//     digitalWrite(4,LOW);

//     }
//   //radio.flush_rx();
// Serial.print("radio");
//   }

//       Serial.print("looping");
//    delay(100);

// }
