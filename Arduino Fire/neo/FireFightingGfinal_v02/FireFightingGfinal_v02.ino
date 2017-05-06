
//***********Motors**********************************//
/* L293 Pin 1 –> Arduino +5V
 L293 Pin 2 –> Arduino Pin 2
 L293 Pin 3 –> Left Motor Pin 1
 L293 Pin 4 –> Arduino Ground
 L293 Pin 5 –> Arduino Ground
 L293 Pin 6 –> Left Motor Pin 2
 L293 Pin 7 –> Arduino Pin 3
 L293 Pin 8 –> Battery +5V
 L293 Pin 9 –> Battery +5V
 L293 Pin 10 –> Arduino Pin 4
 L293 Pin 11 –> Right Motor Pin 2
 L293 Pin 12 –> Arduino Ground
 L293 Pin 13 –> Arduino Ground
 L293 Pin 14 –> Right Motor Pin 1
 L293 Pin 15 –> Arduino Pin 5
 L293 Pin 16 –> Arduino +5V
 */

//***********Ultrasonic sensors HC-SR04*****************//
/* HC-SR04 Pin VCC -> Arduino +5V
 HC-SR04 Pin GND -> Arduino Ground
 HC-SR04 Pin Trig -> Arduino Pin 5(Right2)(RightFront), 8(Right)(RightBack), 3(Front)
 HC-SR04 Pin Echo -> Arduino Pin 4(Right2)(RightFront), 7(Right)(RightBack), 2(Front)
 */


#include <SPI.h>

#include <Wire.h>
#include <SFE_LSM9DS0.h>

#define LSM9DS0_XM  0x1D // Would be 0x1E if SDO_XM is LOW
#define LSM9DS0_G   0x6B // Would be 0x6A if SDO_G is LOW

LSM9DS0 dof(MODE_I2C, LSM9DS0_G, LSM9DS0_XM);





//QRE Line sensor
int QRE1113_Pin = 12;

//Blower 
int blower=13;

//Motor Pins
//Left
int MotorLeftA = 6;
int MotorLeftB = 9;
//Right
int MotorRightA = 10;
int MotorRightB = 11;

//Ultrasonic sensors Pins
//Apostasi se ekatosta
long USFrontDistance;
long USRightDistance;
long USRight2Distance;

long USFrontDuration;
long USRightDuration;
long USRight2Duration;


//US trg pins
int USFrontTrigPin = 3;
int USRightTrigPin = 8;
int USRight2TrigPin = 5;


//US Echo pins
int USFrontEchoPin = 2;
int USRightEchoPin = 7;
int USRight2EchoPin = 4;
 

//Fire sensors
int RightFS = A0;
int FrontLFS = A1;
int LeftFS = A2;

//Loop
int DelayValue = 100;
int diff = 0;
int diff2 =0;
int diffFront = 0;
int range = 2;
const int validDistance = 15;


int counter=0;

float x=0;
float y=0;
float z=0;

//Flag
boolean flag = false;



void setup(){
  
  //GIA PINS 0-1
  
  //Motor pins initialization
  pinMode(MotorLeftA, OUTPUT);
  pinMode(MotorLeftB, OUTPUT);
  pinMode(MotorRightA, OUTPUT);
  pinMode(MotorRightB, OUTPUT);

  //US pins initialization
  pinMode(USFrontTrigPin, OUTPUT); 
  pinMode(USFrontEchoPin, INPUT); 
  pinMode(USRightTrigPin, OUTPUT);
  pinMode(USRightEchoPin, INPUT); 
  pinMode(USRight2TrigPin, OUTPUT);
  pinMode(USRight2EchoPin, INPUT);
  pinMode(QRE1113_Pin, INPUT);
  pinMode(blower, OUTPUT);
 // Serial.begin(9600); 
   Serial.begin(115200);
   uint16_t status = dof.begin();

}//setup

void loop(){

   /**********OTI EINAI MESA SE SXOLIA EINAI GIA ELEGXO******///
   //Serial.println(readRight2US());
  
  //Serial.println(readFrontUS()); 
  
  //delay(1000);
  
  
  //moveBackward();
  
wallFollow();
/*
 dof.readMag();
  
  x = Heading((float) dof.mx, (float) dof.my);
  y=(float) dof.mx;
  z= (float) dof.my;
   
  
  Serial.println(x);
   Serial.println();
   Serial.println(y);
    Serial.println();
     Serial.println(z);
    */
   
  //  digitalWrite(blower, HIGH);
  //Serial.print(" mprosta: ");
  //Serial.println(readFrontUS());  
  //turnRight();
  //detectFire();
  //Serial.print("deksia mprosta: ");
 // Serial.println(readRight2US());
  
 // Serial.print("deksia pisw: ");
 // Serial.println(readRightUS());

/*
Serial.print("   Deksios: ");
  Serial.print(analogRead(RightFS ));
  
  Serial.print("   Mprosta Aristera: ");
  Serial.print(analogRead(FrontLFS ));
  
  Serial.print("   Aristera: ");
  Serial.println(analogRead(LeftFS));
  
   //loop 
  */
  delay(1000);
 
   }
  
void wallFollow(){
  
  

  
 if(counter==0){  //gia thn prwth timh
  dof.readMag();
  
  x = Heading((float) dof.mx, (float) dof.my);
  }
    
  counter=counter+1;
  
  
  detectFire();

//??
 // delay(2);
 //diff2 =  readRight2US() - validDistance;
 //delay(2);
  //diff  =  readRightUS() - validDistance;
  
  
  //Line sensor (not working)
  /*
  if (readQD()<200)
  {
      flag=true;
      do 
      {
        moveForward();
        detectFire();
      }while( readFrontUS()>10 && flag==true);
      turnLeft();
      delay(1100);
  }*/
             
  detectFire();
    
   if( readFrontUS() < 15 ) //mprosta toixos se logiki apostasi
    {
      
     turnLeft();
      delay(50);//elegxo
     counter=0;
    } //if 
    
   dof.readGyro();
   if(  x < Heading((float) dof.mx, (float) dof.my)  ) //APOSTASH APO TON MPROSTA DEKSIA LIGO ARISTERA
     {
      goLeft();
     }//else if
     
   dof.readGyro();
   if (x > Heading((float) dof.mx, (float) dof.my) ) //APOSTASH APO TON PISW DEKSIA  LIGO ARISTERA  
     {
     goRight();

     }//else if
    
     detectFire();
     
  if(readRightUS() > 35 && readRight2US() > 35 ) //KAI TA DUO DEIXNOUN KENO strofi
   {
     Serial.println("mphkkkkeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeee");
     moveForward();
     delay(100);
     detectFire();
     turnRight();
     delay(205);
     detectFire();
     moveForward();
     delay(10* readRight2US ());
      counter=0;
   }//else if 
   
   if ( readRightUS() > 40 && readRight2US() > 40 && readFrontUS() > 10 )//DEKSIA XWROS MPROSTA XWROS LIGO  ARISTERAAAAA 
   { 
     goRight();
     delay(5);
   }//else if
   
   
   if (  readRight2US() < 40 && readRightUS() < 40 && readFrontUS() > 10 )// DEKSIA EXEI XWRO ,MPROSTA XWROS LIGO DEKSIAAA
   {
     goLeft();
     delay(5);
   }//else if
   
   
   
   if (readFrontUS() > 3000  ||  readRight2US()>3000) //|| (readFrontUS() > 60 && readRight2US() < readRightUS()))  //LOGIKO LATHOS panw sto toixo h skedasi KANEI LIGO PISW 
   {
     moveBackward();
     delay(300);
     detectFire();
     turnLeft();
     delay(100);
   
   }//else if
   
    detectFire();
   
  
}//wallFollow

//***********Fire detecttion method*****************//

void detectFire(){ 
  
  int RFS=analogRead(RightFS);
  int LFS=analogRead(LeftFS);
  int FFSL=analogRead(FrontLFS); 

  
   if ( analogRead(RightFS) <750 || analogRead(FrontLFS) < 500 || analogRead(LeftFS) <750){ //an uparxei fwtia

   do{
        
      if( analogRead(RightFS) < 750){ //an uparxei deksia
       
        digitalWrite(blower, HIGH);
        
        int i;
        for(i=100; i <= 2600; i+=100){ //stribei
          
          Serial.print(i);
          turnRight();
          delay(100);
          stopMotors();
          delay(100);
          
          
          if( analogRead(FrontLFS)>=1000 ){ //an exei sbisei
            
            stopMotors();
            digitalWrite(blower, LOW);
            delay(2000);         
            
            do {
              if( analogRead(FrontLFS)<750 ){ //anazwpirwsi
                
                do{
                  moveForward();
                }while( analogRead(FrontLFS) > 200 );
                
                digitalWrite(blower, HIGH);
                delay(400);
                stopMotors();
                digitalWrite(blower, LOW);              
              }//if
            }while ( analogRead(FrontLFS)>=1000 );
            
          }//if       
        }//for 
       }// if
             
        else if ( analogRead(LeftFS) < 750 ){
         
          int i;
        digitalWrite(blower, HIGH);
        
        for(i=100; i <= 2600; i+=100){ 
          
          Serial.print(i);
          turnLeft();
          delay(100);
          stopMotors();
          delay(100);
          
          if( analogRead(FrontLFS)>=1000 ){
            
            stopMotors();
            digitalWrite(blower, LOW);
            delay(2000);        
 
            do{
              if( analogRead(FrontLFS)<750 ){
                
                do{
                  moveForward();
                }while( analogRead(FrontLFS) > 200 );
                
                digitalWrite(blower, HIGH);
                delay(400);
                stopMotors();
                digitalWrite(blower, LOW);}
              }while ( analogRead(FrontLFS)>=1000 );
                
              }//if
            
                    
        }//for  
                 
        }//else if
  
       
       //ENALAKTIKA MONO GIA TON MPROSTA 
       
         else if(analogRead(FrontLFS) < 500 ){
           
            digitalWrite(blower, HIGH);     
            stopMotors();       
            do{
              delay(1);
            }while( analogRead(FrontLFS) <=1000 );
         }//elseif
            
        if(analogRead(FrontLFS)>1000 && analogRead(RightFS)>1000 && analogRead(LeftFS)>1000){      ///elegxos sbisimo
          digitalWrite(blower, LOW);
        }//if
     

    delay(5);
    }while(analogRead(FrontLFS)<1000 && analogRead(RightFS)<1000 && analogRead(LeftFS)<1000 );

   }
      
        digitalWrite(blower, LOW);
    
}//detectFire

//***********Motors methods************************//
void stopMotors(){

  digitalWrite(MotorLeftA, LOW);
  digitalWrite(MotorLeftB, LOW);
  digitalWrite(MotorRightA, LOW);
  digitalWrite(MotorRightB, LOW);
}//stopMotors

void moveBackward(){

  digitalWrite(MotorLeftA, HIGH);
  digitalWrite(MotorLeftB, LOW);
  digitalWrite(MotorRightA,HIGH);
  digitalWrite(MotorRightB, LOW);
  
}//moveBackward

void moveForward(){

  analogWrite(MotorLeftA, 0);
  analogWrite(MotorLeftB, 160);
  analogWrite(MotorRightA, 0);
  analogWrite(MotorRightB, 180);
}//moveForward

void turnLeft(){

  digitalWrite(MotorLeftA, HIGH);
  digitalWrite(MotorLeftB, LOW);
  digitalWrite(MotorRightA, LOW);
  digitalWrite(MotorRightB, HIGH);
  
  
}//turnLeft

void turnRight(){

  digitalWrite(MotorLeftA, LOW);
  digitalWrite(MotorLeftB, HIGH);
  digitalWrite(MotorRightA, HIGH);
  digitalWrite(MotorRightB, LOW);
}//turnRight

void goLeft(){

  analogWrite(MotorLeftA, 0);
  analogWrite(MotorLeftB, 170);
  analogWrite(MotorRightA, 0);
  analogWrite(MotorRightB, 180);
}//goRight

void goRight(){

  analogWrite(MotorLeftA,200);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, 170);
  analogWrite(MotorRightB, 0);
}//goLeft

//void goRight1() }

//***********Ultrasonic sensors methods*****************//
long readRight2US() {
  // Left ultrasonic
  digitalWrite(USRight2TrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(USRight2TrigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(USRight2TrigPin, LOW);

  USRight2Duration = pulseIn(USRight2EchoPin, HIGH); 
  return USRight2Distance = USRight2Duration/29 /2; 
} //readLeftUS

long readRightUS() {
  // Right ultrasonic
  digitalWrite(USRightTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(USRightTrigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(USRightTrigPin, LOW);

  USRightDuration = pulseIn(USRightEchoPin, HIGH); 
  return USRightDistance = USRightDuration/29 /2;             
} //readRightUS

long readFrontUS() {
  // Front ultrasonic
  digitalWrite(USFrontTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(USFrontTrigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(USRightTrigPin, LOW);

  USFrontDuration = pulseIn(USFrontEchoPin, HIGH); 
  return USFrontDistance = USFrontDuration/29 /2;             
} //readFrontUS


//***********Detect line method*****************//
int readQD(){
  pinMode( QRE1113_Pin, OUTPUT );
  digitalWrite( QRE1113_Pin, HIGH );  
  delayMicroseconds(10);
  pinMode( QRE1113_Pin, INPUT );

  long time = micros();
  int diff = micros() - time;

  return diff;
}//readQD





float Heading(float hx, float hy)
{
  float heading;
  
  if (hy > 0)
  {
    heading = 90 - (atan(hx / hy) * (180 / PI));
  }
  else if (hy < 0)
  {
    heading =  -(atan(hx / hy) * (180 / PI));
  }
  else // hy = 0
  {
    if (hx < 0) heading = 180;
    else heading = 0;
  }
  
  return heading;
}
