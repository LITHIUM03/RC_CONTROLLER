#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define LED_PIN 3
RF24 radio(7, 8); // CE, CSN
int msg[1];

const byte address[6] = "00001";

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
  pinMode(LED_PIN,OUTPUT);
}




void loop(void){
// bool done = false;

if (radio.available()){
  radio.read(msg, 1);
  Serial.println(msg[0]);

  if (msg[0] == 111){
    //delay(10);
    digitalWrite(LED_PIN, HIGH);
    }
  else {
    digitalWrite(LED_PIN, LOW);
    //delay(10);
    }
}
else{
    Serial.println("No radio available");
    digitalWrite(LED_PIN, LOW);
    //delay(10);
}
  
delay(10);}


  