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
 HC-SR04 Pin Trig -> Arduino Pin 5(Right2)(RightFront), 8(Right)(RightBack), 3(Front) , 22(Left)(LeftBack) , 25(LeftCorner)(LeftFront)
 HC-SR04 Pin Echo -> Arduino Pin 4(Right2)(RightFront), 7(Right)(RightBack), 2(Front) , 23(Left)(LeftBack) , 24(LeftCorner)(LeftFront)
 */

#include <PID_v1.h>


int wheelLeftS = 19;
int wheelRightS = 18;
//Counters for encoders
volatile int countLeft = 0;
volatile int countRight = 0;

int masterSpeed = 120;//Left
int slaveSpeed = 120;//Right

int error = 0;
int error1 = 0;
int kp = 2;
int times = 9 ;
int n;


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

//Loop
int DelayValue = 100;
const int validDistance = 15;

//Flag
boolean flag = false;



void setup(){
  
  //GIA PINS 0-1
  
  //Motor pins initialization

 // pinMode(MotorLeftA, OUTPUT);
  //pinMode(MotorLeftB, OUTPUT);
  ///pinMode(MotorRightA, OUTPUT);
  //pinMode(MotorRightB, OUTPUT);

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
  
  attachInterrupt(4, LeftCounts, RISING);
  attachInterrupt(5, RightCounts, RISING);



  Serial.begin(9600); 

  
  
}//setup

void loop(){

  wallFollow();
  
//stopMotors();
//digitalWrite(blower, HIGH);

  // turnRight();
    //digitalWrite(blower, HIGH);
    
    //Serial.println(readQD());
   /// Serial.print(" mprosta: ");
 //Serial.println(readFrontUS());  
  
  //detectFire();
  //Serial.print("deksia mprosta: ");
  //Serial.println(readRight2US());
  
  //Serial.print("deksia pisw: ");
  //Serial.println(readRightUS());
  
  //Serial.print("aristera: ");
  //Serial.println(readLeftUS());

  //Serial.print("aristera corner: ");
  //Serial.println(readLeftCornerUS());
/*
Serial.print("   Deksios: ");
  Serial.print(analogRead(RightFS ));
  
  Serial.print("   Mprosta Aristera: ");
  Serial.print(analogRead(FrontLFS ));
  
  Serial.print("   Aristera: ");
  Serial.println(analogRead(LeftFS));
  
  Serial.print("   Pisw: ");
  Serial.println(analogRead(BackFS));
*/
  //delay(1000);
  
}//loop 

//***********wallFollow method*****************//  

void wallFollow(){
  
 delay(5);
      
    
   if( readFrontUS() < 15 ) //mprosta toixos se logiki apostasi
    {
    TurnLeft90();
    
    } //if 
  
   if( readRight2US() <10 ) //APOSTASH APO TON MPROSTA DEKSIA LIGO ARISTERA
     {
        countRight = 0;

     while ( countRight < 2 ) {
     analogWrite(MotorRightB, 100);}
      
     }//else if
   
   if (readRight2US() > 10 ) //APOSTASH APO TON PISW DEKSIA  LIGO ARISTERA  
    {
         countRight = 0;
       while ( countRight < 2 ) {
       analogWrite(MotorLeftB, 100);}
    
     }//else if
     
   if(readRight2US() == readLeftUS() ){
     moveForward();
   }
  /* 
  if(readRightUS() > 35 && readRight2US() > 35 ) //KAI TA DUO DEIXNOUN KENO strofi
   {
    moveForward();
     delay(400);
     TurnRight90();
     if( readFrontUS() < 10 ) //elenxos an th diarkeia pou kanei th strofh yparxei toixos
      { 
         TurnLeft90();
          //elegxo
      }//if
    moveForward();
   }//if 
  if (readLeftUS()<5 || readLeftCornerUS()<3 || readLeftCornerUS()>300)
   {
     moveBackward();
     delay(200);
     TurnRight90();

   }//if

   if (readFrontUS() > 300  || readRightUS()>300  || readRight2US()>300 || readFrontUS() < 5 || readRight2US()<3)   //LOGIKO LATHOS panw sto toixo h skedasi KANEI LIGO PISW 
   {
     moveBackward();
     delay(180);
     TurnLeft90();
   }//if
   
   */
  
}//wallFollow


/*

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

*/



//***********Motors methods************************//
void stopMotors(){

 analogWrite(MotorLeftA, 0);
 analogWrite(MotorLeftB, 0);
 analogWrite(MotorRightA, 0);
 analogWrite(MotorRightB, 0);
}//stopMotors

void moveBackward(){

  analogWrite(MotorLeftA, 255);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA,255);
  analogWrite(MotorRightB, 0);
  
}//moveBackward

void moveForward(){
 
  
  analogWrite(MotorLeftA, masterSpeed);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, slaveSpeed);
  analogWrite(MotorRightB, 0);

  error = countLeft - countRight;
  slaveSpeed += error / kp;

  countLeft = 0;
  countRight = 0;

  delay(10);
  
}//moveForward


void TurnRight90(){
  
  countLeft = 0;

  while ( countLeft < times ) {
    turnLeft();
  }//while


}//Turn90



void TurnLeft90() {

  countRight = 0;

  while ( countRight < times ) {
   turnRight();
  }//while


}//Turn90






void LeftCounts() {
  countLeft++;
  //Serial.print("Left counter: ");
  //Serial.println(countLeft);

}//LeftCount

void RightCounts() {
  countRight++;
  //Serial.print("Right counter: ");
  //Serial.println(countRight);
}//RightCount

  
void turnRight(){

  analogWrite(MotorLeftA, 255);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, 0);
  analogWrite(MotorRightB, 255);
  
  
}//turnLeft

void turnLeft(){

  analogWrite(MotorLeftA,0);
  analogWrite(MotorLeftB, 255);
  analogWrite(MotorRightA, 255);
  analogWrite(MotorRightB, 0);
}//turnRight



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





