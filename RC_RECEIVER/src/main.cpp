#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "C:\Users\Elad\Documents\PlatformIO\Projects\RC_Project\shared\dataPackage.h"

/*~~~~~~~~~~PINS~~~~~~~~~~*/
#define R_MOTOR_PWM_PIN
#define R_MOTOR_DIR_PIN
#define L_MOTOR_PWM_PIN
#define L_MOTOR_DIR_PIN


/*~~~~~~~CONSTANTS~~~~~~~~*/
// #define OFF 0x00
// #define ON 0x11
// #define FORWARD 0x11
// #define REVERSE 0X00
#define SPEED_CHANGE_WAIT_TIME 10

/*~~~~~~~STEERING CHART~~~~~~*/
/*

*/
// uint8_t steering_chart [31][2][3] =
// ; 

unsigned long current_millis = 0;
unsigned long last_speed_change_millis = 0;
unsigned long last_signal_time = 0;
data_package current_data; 
data_package car_state; // this should always be the actual state in which the machine operates.


/*~~~~~~~~FUNCTIONS~~~~~~~*/

void update_car_physical_state (const data_package&){
//TODO
/*
this function should look like:
digitalWrite(cat_state.field,relevat_pin);

*/
// analogWrite(R_MOTOR_PWM_PIN,);
// analogWrite(L_MOTOR_PWM_PIN,);
// digitalWrite(R_MOTOR_DIR_PIN,);
// digitalWrite(L_MOTOR_DIR_PIN,);


}
//TODO USE POINTERS OR PASS BY REF TO AVOID CREATING LOCAL COPIES
void reset_car(data_package& car_state){
//this function should halt the car and set steering to zero. this function only updates 
//the data structure then sends it to update_car_physical_state(data_package) 
//to alter the physical car_state.
while(car_state.base_speed || car_state.steering){
  
  if (car_state.base_speed) car_state.base_speed--;
  if (car_state.steering>0) car_state.steering--;
  else if (car_state.steering<0) car_state.steering++;
  if(!(car_state.base_speed)) {car_state.d_r = FORWARD;}
  update_car_physical_state(car_state);//this will actually cause the changes
  delay (SPEED_CHANGE_WAIT_TIME);
  //BEWARE- this method does not use millis
}


}
/*~~~~~~~~~~RADIO~~~~~~~~~*/
RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";


void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();
}

/*
IDEA:

perform all data structure changes, only then update physical state of the machine.

*/

void loop(void){
  current_millis = millis();
  if(current_millis-last_signal_time > 1000){
    reset_car(car_state); // TODO
  }
  if (radio.available()){
    last_signal_time = current_millis;
    radio.read(&current_data,sizeof(data_package));
    current_data.print();
    if (current_data != car_state) {
      
      if (car_state.headlights != current_data.headlights)
      {
        car_state.headlights = current_data.headlights;
        //TODO need to actually perform the physical change- digitalWrite(WHATEVER)
      }
      
      if ((car_state.base_speed != current_data.base_speed)&&(current_millis>=last_speed_change_millis + 
      SPEED_CHANGE_WAIT_TIME))
      {
        /*
        Here I need to calculate a gap towards the currently desired speed. and than increase or decrease by the speed quantum
        */
       short speed_gap = car_state.base_speed-current_data.base_speed;
        // if negative we need to increase the speed, and if positive we need to decrease it.
        last_speed_change_millis = current_millis;
        if ( speed_gap < 0){
          car_state.base_speed+=1;
        //TODO need to actually perform the physical change- digitalWrite(WHATEVER)
        }
        else{
          car_state.base_speed-=1;
        //TODO need to actually perform the physical change- digitalWrite(WHATEVER)  
        }
        last_speed_change_millis = current_millis;
        // TODO update the speed_change_time
        //TODO need to actually perform the physical change- digitalWrite(WHATEVER)
      }
      if (car_state.steering!= current_data.steering){
        //TODO handle steering

      }
      if(car_state.d_r!=current_data.d_r){
        //
        //need to halt, than
        reset_car(car_state);
        car_state.d_r = current_data.d_r;
        //continue;
        //change gear
        //continue?      
      }


    } 

  update_car_physical_state(car_state);
  //car_state.print();


  delay(10);
  }
}

  