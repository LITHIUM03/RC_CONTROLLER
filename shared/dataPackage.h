/*~~~~~~~CONSTANTS~~~~~~~~*/
//#define OFF 0x0
//#define ON 0x1
//#define FORWARD 0x11
//#define REVERSE 0X00

#define SPEED_QUANTUM 11
#define MINIMAL_PWM_SPEED 0
#define MAXIMAL_PWM_SPEED 255
#define MAX_REVERSE_SPEED 150

typedef enum{LEFT, RIGHT} SteeringDirection;
typedef enum{FORWARD, REVERSE} TravelDirection;
typedef enum{CW, CCW} MotorSpinDirection;
typedef enum{ON, OFF} OnOrOff;
typedef uint8_t Speed;

/*~~~~~~~DATA OBJECT~~~~~~*/
class data_package
{
private:
public:
  Speed base_speed;
  int8_t steering;
  OnOrOff headlights;
  TravelDirection d_r;
 
  data_package() : base_speed(0),steering(0),headlights(OFF),d_r(FORWARD){}
  ~data_package(){};
  
  void printHeadlights() const{
    if(ON==headlights) Serial.print("ON");
    else Serial.print("OFF");
  }

    void print_d_r() const{
    if(FORWARD==d_r) Serial.print("FORWARD");
    else Serial.print("REVERSE");
  }

  void print() const{
    Serial.print("base_speed: ");
    Serial.print(base_speed);
    Serial.print(" | ");
    Serial.print("steering: ");
    Serial.print(steering);
    Serial.print(" | ");
    Serial.print("headlights: ");
    printHeadlights();
    Serial.print(" | ");
    Serial.print("d_r: ");
    print_d_r();
    Serial.print(" | \n");
  }
};

  bool operator==(const data_package& LHS,const data_package& RHS){
    return ((LHS.base_speed==RHS.base_speed) &&
    (LHS.d_r==RHS.d_r) &&
    (LHS.headlights==RHS.headlights) &&
    (LHS.steering==RHS.steering));
  }

  bool operator!=(const data_package& LHS,const data_package& RHS){
    return !((LHS.base_speed==RHS.base_speed) &&
    (LHS.d_r==RHS.d_r) &&
    (LHS.headlights==RHS.headlights) &&
    (LHS.steering==RHS.steering));

  }

/*~~~~~~~FUNCTIONS~~~~~~~*/


long myMap(long x, long in_min, long in_max){
  long mid_in = (in_max+in_min)/2;
  //long mid_out = (out_max+out_min)/2;
  const long epsilon = 40;

  if (x>mid_in + epsilon){
    return map(x,mid_in,in_max,MINIMAL_PWM_SPEED,255);
  }
  if (x<mid_in - epsilon){
    return map(x,in_min,mid_in,MAX_REVERSE_SPEED,MINIMAL_PWM_SPEED);

  }
  return 0;
}