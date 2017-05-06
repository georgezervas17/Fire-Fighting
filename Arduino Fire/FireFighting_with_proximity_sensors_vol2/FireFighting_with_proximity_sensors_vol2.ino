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

  
  
}//setup

void loop(){

//stopMotors();
//digitalWrite(blower, HIGH);
//wallFollow();
  // turnRight();
  
  moveForward();
    //digitalWrite(blower, HIGH);
    
    //Serial.println(readQD());
    
  Serial.print(" mprosta: ");
 Serial.println(readFrontUS());  
  
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
  
  detectFire();
    
   if( readFrontUS() < 15 ) //mprosta toixos se logiki apostasi
    { 
     
     turnLeft();
     delay(80);  
    } //if 
    
    

   if( readRightUS() < 12 ) //APOSTASH APO TON MPROSTA DEKSIA LIGO ARISTERA
     {
      goLeft();
     }// if
   
   if (readRightUS() > 12 ) //APOSTASH APO TON PISW DEKSIA  LIGO ARISTERA  
     {
     goRight();

     }// if
     
     
     // detectFire();
      
  if(readLeftUS() < 10 )
     {
      goRight();
     }  
    
   //detectFire();
   
  if(readRightUS() > 35 && readRight2US() > 35 ) //KAI TA DUO DEIXNOUN KENO strofi
   {
     moveForward();
     delay(200);
    // detectFire();
     turnRight();
     delay(80);
     //detectFire();
     if( readFrontUS() < 10 ) //elenxos an th diarkeia pou kanei th strofh yparxei toixos
      { 
         turnLeft();
         delay(80);  //elegxo
      }//if
     moveForward();
     delay(400);
   }//if 
   if (readLeftUS()<7 || readLeftCornerUS()<6 || readLeftCornerUS()>300)
   {
     moveBackward();
     delay(120);
    // detectFire();
     turnRight();
     delay(250);
   }//if

   if (readFrontUS() > 300  || readRightUS()>300  || readRight2US()>300 || readFrontUS() < 7 || readRight2US()<6)   //LOGIKO LATHOS panw sto toixo h skedasi KANEI LIGO PISW 
   {
     moveBackward();
     delay(120);
   //  detectFire();
     turnLeft();
     delay(300);
   }//if
   
  //  detectFire();
   
  
}//wallFollow

//***********Fire detecttion method*****************//
void detectFire(){ 
  
  int RFS=analogRead(RightFS);
  int LFS=analogRead(LeftFS);
  int FFSL=analogRead(FrontLFS); 
  int k=0;
  int i2,i1;
  
   if ( analogRead(RightFS) <300 || analogRead(FrontLFS) < 300 || analogRead(LeftFS) <300 || analogRead(BackFS) <500){

   do{
        
      if( analogRead(RightFS) < 300){
       
        
        digitalWrite(blower, HIGH);
       
       
       for(i1=0;i1<3500;i1=i1+100){
          
          turnRight();
          delay(100);
          stopMotors();
          delay(50);
          
          if(analogRead(FrontLFS) <50 ){
            do{
            stopMotors();
            }while(analogRead (FrontLFS) < 1000);
            break;}
          
        }
        
         
          
          Label1:
          
          if( analogRead(FrontLFS) >= 900 ){
            digitalWrite(blower, LOW); 
            goto efuges1;    }
            
          else{
                  
              
             moveForward();
             delay(10);
      
        
             stopMotors();
             delay(1500);
             k=1;
             goto Label1;
          }
          

          
          }//if    
    
    
    
    
    
    else if( analogRead(LeftFS) < 300){
      
       
       digitalWrite(blower, HIGH);
        
     
       for(i2=0; i2 <= 3600; i2=i2+100){ 
          
         turnLeft();
          delay(100);
          stopMotors();
          delay(50);
          
          if(analogRead(FrontLFS) <50 ){
            stopMotors();
            delay(2000);
            break;}
        }
        
        
         
           
            Label2:
          
          if( analogRead(FrontLFS) >= 900 ){
            digitalWrite(blower, LOW);
            goto efuges2; }
          
          else{   
              
                moveForward();
                delay(10);
             
        
             stopMotors();
             delay(1500);
             k=2;
             goto Label2;
          
          }
  
  
      }//if  2     
    
    
       efuges1:
       if(k==1){
         turnRight();
         delay(600-i1);
         k=0;} 
       
      efuges2:
       if(k==2){
         turnLeft();
         delay(600-i2);
         k=0; }
      
     
         
         
         
         
    delay(5);
    
    }while(analogRead(FrontLFS)<1000 && analogRead(RightFS)<1000 && analogRead(LeftFS)<1000 );

   }
   
   
      
        digitalWrite(blower, LOW);
    
}//detectFire




//***********Motors methods************************//
void stopMotors(){

  analogWrite(MotorLeftA, 0);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, 0);
 analogWrite(MotorRightB, 0);
}//stopMotors

void moveBackward(){

  analogWrite(MotorLeftA,255);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA,255);
 analogWrite(MotorRightB, 0);
  
}//moveBackward

void moveForward(){
  analogWrite(MotorLeftA, 255);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA,255);
  analogWrite(MotorRightB, 0);
  
}//moveForward

void turnLeft(){

  analogWrite(MotorLeftA, 255);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, 0);
  analogWrite(MotorRightB, 255);
  
  
}//turnLeft

void turnRight(){

  analogWrite(MotorLeftA, 0);
  analogWrite(MotorLeftB, 255);
 analogWrite(MotorRightA, 255);
 analogWrite(MotorRightB,0);
}//turnRight

void goLeft(){

  analogWrite(MotorLeftA, 0);
  analogWrite(MotorLeftB, 110);
  analogWrite(MotorRightA, 0);
  analogWrite(MotorRightB, 140);
}//goRight

void goRight(){

  analogWrite(MotorLeftA,0);
  analogWrite(MotorLeftB, 140);
  analogWrite(MotorRightA, 0);
  analogWrite(MotorRightB, 120);
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
