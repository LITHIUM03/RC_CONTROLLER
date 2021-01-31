#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include "C:\Users\Elad\Documents\PlatformIO\Projects\RC_Project\shared\dataPackage.h"


/*~~~~~~~~~~SPIN DIRECTIONS~~~~~~~~~~*/

#define FORWARD_SPIN_DIRECTION_FOR_L_MOTOR CW
#define FORWARD_SPIN_DIRECTION_FOR_R_MOTOR CCW
#define REVERSE_SPIN_DIRECTION_FOR_L_MOTOR CCW
#define REVERSE_SPIN_DIRECTION_FOR_R_MOTOR CW


/*~~~~~~~~~~PINS~~~~~~~~~~*/
#define HEADLIGHTS_PIN 3
#define R_MOTOR_PWM_PIN 5
#define R_MOTOR_DIR_PIN A6
#define L_MOTOR_PWM_PIN 6
#define L_MOTOR_DIR_PIN A7



/*~~~~~~~DATA OBJECT~~~~~~*/

class CarState
{
  /*
  CarState object is desigend to hold the exact state of the car i a given moment. as such, it holds details from the 
  data_package object, but also specifies the exact pwms of the motors, their directions (cw ccw).
  the reason for the use of this data structure is that holding the exact speeds of the motors allows for better control
  of the bahvior, and more accurate understanding of the car's state.
  The object does not specify base_speed and steering, because it has no direct effect on car's state, the two only indicate the desired state.
  Logically, the carState will always try to match the current data package, but is limited by coded limitations, 
  designed to prevent violent steering, unpredictable direction changes and radio issues. Furtherrmore, it will try to compensate for
  user errors.
  */
private:
public:
  //byte base_speed;
  //int8_t steering; /*NOT NEEDED, AND EVEN CONFUSING*/
  OnOrOff headlights; /*GOOD*/
  TravelDirection d_r;
  Speed R_speed;
  Speed L_speed;
  MotorSpinDirection R_dir;
  MotorSpinDirection L_dir;

  CarState() : headlights(OFF), d_r(FORWARD),R_speed(0),L_speed(0),R_dir(FORWARD_SPIN_DIRECTION_FOR_R_MOTOR),
  L_dir(FORWARD_SPIN_DIRECTION_FOR_L_MOTOR)/*TODO with the right rotation directions.*/{}
  ~CarState(){};
  void print() const{
    Serial.print("headlights: ");
    Serial.print(headlights);
    Serial.print(" | ");
    Serial.print("d_r: ");
    Serial.print(d_r);
    Serial.print(" | ");
    Serial.print("L_speed: ");
    Serial.print(L_speed);
    Serial.print(" | ");
    Serial.print("R_speed: ");
    Serial.print(R_speed);
    Serial.print(" | ");
    Serial.print("L_dir: ");
    Serial.print(L_dir);
    Serial.print(" | ");
    Serial.print("R_dir: ");
    Serial.print(R_dir);
    Serial.print(" | ");
    Serial.print("d_r: ");
    if(FORWARD==d_r) Serial.print("FORWARD");
    else Serial.print("REVERSE");
    Serial.print(" | \n");
  }
};


/*~~~~~~~CONSTANTS~~~~~~~~*/
#define INNER_TURN_DIAMETER 300 // CM
#define INNER_TURN_RADIUS (INNER_TURN_DIAMETER/2) // CM
#define WHEEL_BASE 60 // CM
#define SPEED_CHANGE_WAIT_TIME 3

unsigned long current_millis = 0;
unsigned long last_speed_change_millis = 0;
unsigned long last_signal_time = 0;
data_package current_data; 
CarState car_state; // this should always be the actual state in which the machine operates.


/*~~~~~~~~FUNCTIONS~~~~~~~*/

void update_car_physical_state_speeds_only (const CarState&){


}


void update_car_physical_state (const CarState& car_state){
//TODO
/*
this function should look like:
digitalWrite(cat_state.field,relevat_pin);

FORWARD_SPIN_DIRECTION_FOR_L_MOTOR 
FORWARD_SPIN_DIRECTION_FOR_R_MOTOR 
REVERSE_SPIN_DIRECTION_FOR_L_MOTOR 
REVERSE_SPIN_DIRECTION_FOR_R_MOTOR 
*/
analogWrite(R_MOTOR_PWM_PIN,car_state.R_speed);
analogWrite(L_MOTOR_PWM_PIN,car_state.L_speed);
if (FORWARD == car_state.d_r){
digitalWrite(R_MOTOR_DIR_PIN,FORWARD_SPIN_DIRECTION_FOR_R_MOTOR);
digitalWrite(L_MOTOR_DIR_PIN,FORWARD_SPIN_DIRECTION_FOR_L_MOTOR);
}
else{
digitalWrite(R_MOTOR_DIR_PIN,REVERSE_SPIN_DIRECTION_FOR_R_MOTOR);
digitalWrite(L_MOTOR_DIR_PIN,REVERSE_SPIN_DIRECTION_FOR_L_MOTOR);

}
digitalWrite(HEADLIGHTS_PIN, car_state.headlights);
}



void halt_car(CarState& car_state){
  Serial.print("\n");
  Serial.print(__func__);
  Serial.print("\n");
  Serial.print(__func__);
  Serial.print("\n");
  Serial.print(__func__);
  Serial.print("\n");
while(car_state.L_speed > car_state.R_speed){
  car_state.L_speed--;
  update_car_physical_state(car_state);
  delay (SPEED_CHANGE_WAIT_TIME);
}
while(car_state.L_speed < car_state.R_speed){
  car_state.R_speed--;
  update_car_physical_state(car_state);
  delay (SPEED_CHANGE_WAIT_TIME);


}
//now equal

while (car_state.L_speed)
{
  car_state.L_speed--;
  car_state.R_speed--;
  update_car_physical_state(car_state);
  delay (SPEED_CHANGE_WAIT_TIME);
}
last_signal_time = millis();
}




void no_radio_stop(CarState& car_state){//TODO
//this function should halt the car and set steering to zero. this function only updates 
//the data structure then sends it to update_car_physical_state(data_package) 
//to alter the physical car_state.

//this method stops the car and prevents it from any further actions.
while(car_state.L_speed > car_state.R_speed){
  car_state.L_speed--;
  update_car_physical_state(car_state);
  delay (SPEED_CHANGE_WAIT_TIME);
    car_state.print();

}
while(car_state.L_speed < car_state.R_speed){
  car_state.R_speed--;
  update_car_physical_state(car_state);
  delay (SPEED_CHANGE_WAIT_TIME);
  car_state.print();


}
//now equal

while (car_state.L_speed)
{
  car_state.L_speed--;
  car_state.R_speed--;
  update_car_physical_state(car_state);
  delay (SPEED_CHANGE_WAIT_TIME);
  car_state.print();

}
//TODO light some emergency_led
car_state.headlights = ON;
Serial.print("HERE \n");

update_car_physical_state(car_state);
Serial.print("HERE2 \n");
  car_state.print();
Serial.print(__func__);

while (true){//this works but wont print Serial inside while(true)
}
/*to prevent any further actions in case a signal is received after some time.*/
  //car_state.print();
//Serial.print("inside the stuck loop of no_radio_stop() \n");
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
   float mapUint2Float(int x, int in_min, int in_max, float out_min, float out_max) {
     return ((x - in_min) * (out_max - out_min) / (in_max - in_min)) + out_min;
}
void loop(void){
  current_millis = millis();
  if(current_millis-last_signal_time > 3500){
    Serial.print("now calling no_radio_stop()");
    no_radio_stop(car_state); // TODO
  }
  if (radio.available()){
    last_signal_time = current_millis;
    radio.read(&current_data,sizeof(data_package));

    float steering_delta_absolute_val = mapUint2Float(abs(current_data.steering),0,15,0,(1.0/6.0));// TODO change the hard coded numbers to defines
    Serial.print(abs(current_data.steering));
    Serial.print("\n");
    Serial.print(current_data.steering);
    Serial.print("\n");
    Serial.print(steering_delta_absolute_val);
    Serial.print("\n");
    SteeringDirection dir = (current_data.steering >= 0) ? RIGHT : LEFT ;

    Speed desired_R_speed; 
    Speed desired_L_speed;
    MotorSpinDirection desired_R_spin_direction;
    MotorSpinDirection desired_L_spin_direction;

    if (current_data.d_r != car_state.d_r){
      Serial.print("direction received is different from current direction");
      halt_car(car_state);
      car_state.d_r = current_data.d_r;
    }

    
    if ( FORWARD == car_state.d_r && RIGHT == dir){
      //the left wheel is the fast wheel
    desired_R_speed = current_data.base_speed * (1-steering_delta_absolute_val); 
    desired_L_speed = current_data.base_speed * (1+steering_delta_absolute_val);
    desired_R_spin_direction = FORWARD_SPIN_DIRECTION_FOR_R_MOTOR;
    desired_L_spin_direction = FORWARD_SPIN_DIRECTION_FOR_L_MOTOR;
    }
    else if ( FORWARD==car_state.d_r && LEFT == dir){
      //the right wheel is the fast wheel
    desired_R_speed = current_data.base_speed * (1+steering_delta_absolute_val); 
    desired_L_speed = current_data.base_speed * (1-steering_delta_absolute_val);
    desired_R_spin_direction =FORWARD_SPIN_DIRECTION_FOR_R_MOTOR;
    desired_L_spin_direction =FORWARD_SPIN_DIRECTION_FOR_L_MOTOR;
    }
    else if ( REVERSE == car_state.d_r && RIGHT == dir){
      //the left wheel is the fast wheel
    desired_R_speed = current_data.base_speed * (1-steering_delta_absolute_val); 
    desired_L_speed = current_data.base_speed * (1+steering_delta_absolute_val);
    desired_R_spin_direction =REVERSE_SPIN_DIRECTION_FOR_R_MOTOR ;
    desired_L_spin_direction =REVERSE_SPIN_DIRECTION_FOR_L_MOTOR ;
    }
    else if ( REVERSE==car_state.d_r && LEFT == dir){
      //the right wheel is the fast wheel
    desired_R_speed = current_data.base_speed * (1+steering_delta_absolute_val); 
    desired_L_speed = current_data.base_speed * (1-steering_delta_absolute_val);
    desired_R_spin_direction =REVERSE_SPIN_DIRECTION_FOR_R_MOTOR ;
    desired_L_spin_direction =REVERSE_SPIN_DIRECTION_FOR_L_MOTOR ;
    }
    car_state.L_dir = desired_L_spin_direction;
    car_state.R_dir = desired_R_spin_direction;
    // car_state.L_speed=desired_L_speed;
    // car_state.R_speed=desired_R_speed;
    
    if(current_millis-last_speed_change_millis>=SPEED_CHANGE_WAIT_TIME){
    if(desired_R_speed>car_state.R_speed) {
      car_state.R_speed++;
      last_speed_change_millis=current_millis;
    }
    else if(desired_R_speed<car_state.R_speed){ 
      car_state.R_speed--;
      last_speed_change_millis=current_millis;
    }
    if(desired_L_speed>car_state.L_speed) {
      car_state.L_speed++; 
      last_speed_change_millis=current_millis;
    }
    else if(desired_L_speed<car_state.L_speed) {
      car_state.L_speed--;
      last_speed_change_millis=current_millis;
    }
    update_car_physical_state(car_state);
    }   
   Serial.print("inside radio received scope");
   Serial.print("\n");
   }
  car_state.print();
  //delay(26);
}



