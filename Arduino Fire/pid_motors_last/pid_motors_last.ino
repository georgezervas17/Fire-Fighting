#include <PID_v1.h>

int wheelLeftS = 19;
int wheelRightS = 18;

//QRE Line sensor
int QRE1113_Pin = 12;

//Blower 
int blower=13;

//Ultrasonic sensors Pins
//Apostasi se ekatosta
long USFrontDistance;
long USRightDistance;
long USRight2Distance;
long USLeftDistance;
long USLeftCornerDistance;

long USFrontDuration;
long USRightDuration;
long USRight2Duration;
long USLeftDuration;
long USLeftCornerDuration;

//US trg pins
int USFrontTrigPin = 3;
int USRightTrigPin = 8;
int USRight2TrigPin = 5;
int USLeftTrigPin = 22;
int USLeftCornerTrigPin = 25;

//US Echo pins
int USFrontEchoPin = 2;
int USRightEchoPin = 7;
int USRight2EchoPin = 4;
int USLeftEchoPin = 23;
int USLeftCornerEchoPin = 24;

//Fire sensors
int RightFS = A3;
int FrontLFS = A2;
int LeftFS = A0;
int BackFS=A1;

//Motor Pins
//Left
int MotorLeftA = 6;
int MotorLeftB = 9;
//Right
int MotorRightA = 10;
int MotorRightB = 11;

//Counters for encoders
volatile int countLeft = 0;
volatile int countRight = 0;

int masterSpeed = 110;//Left
int slaveSpeed = 110;//Right

int error = 0;
int kp = 2;

int times = 18;

void setup()
{
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
  pinMode(USLeftTrigPin, OUTPUT);
  pinMode(USLeftEchoPin, INPUT);
  pinMode(USLeftCornerTrigPin, OUTPUT);
  pinMode(USLeftCornerEchoPin, INPUT);
  
  pinMode(QRE1113_Pin, INPUT);
  pinMode(blower, OUTPUT);
  
  Serial.begin(9600);

  attachInterrupt(4, LeftCounts, RISING);
  attachInterrupt(5, RightCounts, RISING);
}//setup

void loop() {
  //MoveForward();
  checkPosition();
  Serial.print("Right: ");
  Serial.println(readRightUS());
  Serial.print("Right-front: ");
  Serial.println(readRight2US());
  //delay(500);
  //TurnLeft90();
  //TurnRight90();
  
}//loop

//***********wallFollow method*****************//  

void wallFollow(){
  
  detectFire();
    
   if( readFrontUS() < 15 ){ //mprosta toixos se logiki apostasi
     turnLeft();
    } //if 
    
    
  
}//wallFollow

void checkPosition(){
  
  if( readRight2US() - readRightUS() > 10 ){
    do{
      analogWrite(MotorRightA, 100);
    }while( readRight2US() - readRightUS() > 5 );
  }//if
}//checkPosition

//***********Fire detecttion method*****************//
void detectFire(){ 

  
   if ( analogRead(RightFS) <300 || analogRead(FrontLFS) < 300 || analogRead(LeftFS) <300 || analogRead(BackFS) <300){
   
   do{
      
      if( analogRead(RightFS) < 300){
       
        int i;        
        for(i=100; i <= 3600; i=i+100){ 
          
          turnRight();
          delay(50);
          stopMotors();
          delay(200);
          
          if( analogRead(FrontLFS) <= 100 ){
            
            stopMotors();
            digitalWrite(blower, HIGH);
            delay(2000);  
            break;       
          }//if       
        }//for 
       }// if
             
     if ( analogRead(LeftFS) < 300 ){
         
        int i; 
        for(i=100; i <= 2600; i=i+100){ 
          
          turnLeft();
          delay(50);
          stopMotors();
          delay(200);
          
          if( analogRead(FrontLFS) <= 100 ){
            
            stopMotors();
            digitalWrite(blower, HIGH);
            delay(2000);     
            break;   
          }//if
          
           
         }//for  
                 
        }//else if
  
       //ENALAKTIKA MONO GIA TON MPROSTA 
      if(analogRead(FrontLFS) < 300 ){
           
            digitalWrite(blower, HIGH);   
            stopMotors();
            delay(2000);         
      }//elseif
      
      if( analogRead(BackFS) < 300 ){
        turnRight();
        delay(100);
      }//if Back
      
         
      digitalWrite(blower, LOW);   
           
    }while(analogRead(FrontLFS)<300 && analogRead(RightFS)<300 && analogRead(LeftFS)<300 );

   }//if
    
}//detectFire

void MoveForward() {

  analogWrite(MotorLeftA, masterSpeed);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, slaveSpeed);
  analogWrite(MotorRightB, 0);

  error = countLeft - countRight;
  slaveSpeed += error / kp;

  countLeft = 0;
  countRight = 0;

  delay(10);

}//MoveForward

void turnRight() {

  countLeft = 0;
  stopMotors();
  delay(1000);

  while ( countLeft < times ) {
   analogWrite(MotorLeftB, 100);
  }//while

  stopMotors();
  delay(1000);
}//Turn90

void turnLeft() {

  countRight = 0;
  stopMotors();
  delay(1

  while ( countRight < times ) {
   analogWrite(MotorRightB, 100);
  }//while

  stopMotors();
  delay(1000);
}//Turn90


void LeftCounts() {
    countLeft++;

}//LeftCount

void RightCounts() {
    countRight++;
}//RightCount

void stopMotors() {

  analogWrite(MotorLeftA, 0);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, 0);
  analogWrite(MotorRightB, 0);
}//stopMotors

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

long readLeftUS() {
  // Left ultrasonic
  digitalWrite(USLeftTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(USLeftTrigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(USLeftTrigPin, LOW);

  USLeftDuration = pulseIn(USLeftEchoPin, HIGH); 
  return USLeftDistance = USLeftDuration/29 /2;             
} //readLeftUS


long readLeftCornerUS() {
  // Left corner ultrasonic
  digitalWrite(USLeftCornerTrigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(USLeftCornerTrigPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(USLeftCornerTrigPin, LOW);

  USLeftCornerDuration = pulseIn(USLeftCornerEchoPin, HIGH); 
  return USLeftCornerDistance = USLeftCornerDuration/29 /2;             
} //readLeftCornerUS


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

