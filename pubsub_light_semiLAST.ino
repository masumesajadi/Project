/*
 * rosserial Servo Control Example
 *
 * This sketch demonstrates the control of hobby R/C servos
 * using ROS and the arduiono
 * 
 * For the full tutorial write up, visit
 * www.ros.org/wiki/rosserial_arduino_demos
 *
 * For more information on the Arduino Servo Library
 * Checkout :
 * http://www.arduino.cc/en/Reference/Servo
 */

#if (ARDUINO >= 100)
 #include <Arduino.h>
#else
 #include <WProgram.h>
#endif

#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Float32.h>
#include <ArduinoHardware.h>
#include <geometry_msgs/Twist.h>
#include <Wire.h>


int BH1750_address = 0x23; // i2c Addresse
byte buff[2];
const int ENA = 2;
const int IN1 = 3;
const int IN2 = 4;
const int ENB = 7;
const int IN4 = 5;
const int IN3 = 6;

int light;
int i;
int value;

int s=0;
float angle_new; 
int desire;
int b=1;
int x=0;
float sth;
float valf; 

ros::NodeHandle  nh;


void Light_cb(const std_msgs::Int16& cmd_msg){
  desire=cmd_msg.data;  
  
 
  if(BH1750_Read(BH1750_address)==2){   //measure light
        valf=((buff[0]<<8)|buff[1])/1.2;
        light=(int) valf;
        //Serial.print(light);
  }
  delay(1000);
  
  /////*********************** FORWARD **********************************
  
   while((s < 4) && (light < desire)){  // we shoud increase light by opening the curtain
      x++;
      forward(506.25,5);
      delay(1000);
      
      if(BH1750_Read(BH1750_address)==2){   //measure light
        valf=((buff[0]<<8)|buff[1])/1.2;
        light=(int) valf;
        //Serial.print(light);
      }
      if((light > desire)){
       s++;
       sth=angle(s,b);
      }
      else{s++;}
      sth=angle(s,b);
      }
      
      if(BH1750_Read(BH1750_address)==2){   //measure light
        valf=((buff[0]<<8)|buff[1])/1.2;
        light=(int) valf;
        //Serial.print(light);
    
      ////rotate totally but we didnt reach the desire light yet
      if((light < desire)){
        delay(10);
        digitalWrite(13, HIGH-digitalRead(13)); //so turn on LED  ** check if it blink or not + we nead real LEDs actually**
      }
   
   }
   
    
  /////*********************** REVERSE **********************************
  if(x == 0){    
    while((s < 4) && (light > desire)){  // we shoud increase light by opening the curtain
         
      if(BH1750_Read(BH1750_address)==2){   //measure light
        valf=((buff[0]<<8)|buff[1])/1.2;
        light=(int) valf;
        //Serial.print(light);
      }
      
      if((light > desire)){
         reverse(506.25,5);
        delay(1000);
        s++;
      }
       
      else{
       sth=angle(s,b); 
      }
      
      sth=angle(s,b);
    }
  }
}

ros::Subscriber<std_msgs::Int16> sub("MyLight", Light_cb);

std_msgs::Float32 str_msg;
ros::Publisher chatter("chatter", &str_msg);


void setup(){
  pinMode(13, OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(IN1,OUTPUT);
  pinMode(IN2,OUTPUT);
  pinMode(ENB,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);

  nh.initNode();
  nh.subscribe(sub);
  nh.advertise(chatter);
}

void loop(){

  str_msg.data = sth;///angle of motor shoud be here
  chatter.publish( &str_msg );
  nh.spinOnce();
  delay(10);

}
//////**************** Light_sensor Read

  void BH1750_Init(int address){
    Wire.beginTransmission(address);
    Wire.write(0x10); // 1 [lux] aufloesung
    Wire.endTransmission();
  }
  byte BH1750_Read(int address){
    byte i=0;
    Wire.beginTransmission(address);
    Wire.requestFrom(address, 2);
    while(Wire.available()){
      buff[i] = Wire.read();
      i++;
    }
    Wire.endTransmission();  
    return i;
  }

/////======================= Angle Function
float angle(int s , int b){ 
    angle_new = ((s*506.25)/5.625)*b;
  return  angle_new;  
  }
////=======================================

void reverse(int i, int j) {

  // set both motors ON
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  while (1)   {
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 1);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);
    delay(j);
    i--;
    if (i < 1) break; 

    digitalWrite(IN1, 0);
    digitalWrite(IN2, 1);
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
    delay(j);  
    i--;
    if (i < 1) break;

    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
    delay(j);
    i--;
    if (i < 1) break;

    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);
    delay(j);  
    i--;
    if (i < 1) break;
  }

  // set both motors OFF
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);
  
}  // end reverse()

void forward(int i, int j) {

  // Set both motors ON
  digitalWrite(ENA, HIGH);
  digitalWrite(ENB, HIGH);

  while (1)   {
    digitalWrite(IN1, 0);
    digitalWrite(IN2, 1);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);
    delay(j);  
    i--;
    if (i < 1) break;

    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 0);
    digitalWrite(IN4, 1);
    delay(j);
    i--;
    if (i < 1) break;

    digitalWrite(IN1, 1);
    digitalWrite(IN2, 0);
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
    delay(j);  
    i--;
    if (i < 1) break;

    digitalWrite(IN1, 0);
    digitalWrite(IN2, 1);
    digitalWrite(IN3, 1);
    digitalWrite(IN4, 0);
    delay(j);
    i--;
    if (i < 1) break;
  }

  // set both motors OFF
  digitalWrite(ENA, LOW);
  digitalWrite(ENB, LOW);

}




