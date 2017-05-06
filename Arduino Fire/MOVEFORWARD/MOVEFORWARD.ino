#include <SPI.h>

#include <SFE_LSM9DS0.h>

#include <Wire.h>
#include <SFE_LSM9DS0.h>

#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW

LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);

//Motor Pins
//Left
int MotorLeftA = 9;
int MotorLeftB = 6;
//Right
int MotorRightA = 11;
int MotorRightB = 10;

float x; 
int diff;
const float declination = 70.1;
const float xOffset = 103.0;      // Offset required to adjust x coordinate to zero origin
const float yOffset = -136.0; 

void setup() {
  diff =0;
  Serial.begin(115200); 
  uint16_t status = dof.begin(); 
  dof.readGyro();
  x = printHeading((float) dof.mx, (float) dof.my);

}//SETUP

void loop() {
  dof.readMag();
  //moveForward();
 delay(1000);
 //dof.readGyro();
 //diff = diff +(int)dof.calcGyro(dof.gx);
 Serial.println(printHeading((float) dof.mx, (float) dof.my));
 //moveForward();
}//loop

void moveForwardA(){

  analogWrite(MotorLeftA, 160);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, 180);
  analogWrite(MotorRightB, 0);
}//moveForward

void moveForward(){
  dof.readMag();
  dof.readGyro();
  
  
  Serial.print(diff, 2);
  if( x < printHeading((float) dof.mx, (float) dof.my) ){
    analogWrite(MotorLeftA, 170);
    analogWrite(MotorLeftB, 0);
    analogWrite(MotorRightA, 180);
    analogWrite(MotorRightB, 0);
  }//if
  else if(x > printHeading((float) dof.mx, (float) dof.my) ){
    analogWrite(MotorLeftA, 200);
    analogWrite(MotorLeftB, 0);
    analogWrite(MotorRightA, 170);
    analogWrite(MotorRightB, 0);
  }//else if

}//moveForward

float printHeading(float hx, float hy)
{
  float gScale = .92;  // Scale factor for +1.3Ga setting
    
    /*float adjx = hx - xOffset;
    float adjy = hy - yOffset;

    hx = adjx * gScale; 
    hy = adjy * gScale;*/
   
    
  float heading = atan2(hy, hx);
  heading += declination / 1000; // Declination for geo area
      
      if (heading < 0);
        heading +=  2*PI;
        
      if (heading > 2*PI)
        heading -= 2*PI;
       
      float angle = heading * 180/M_PI;
  return angle;
}
