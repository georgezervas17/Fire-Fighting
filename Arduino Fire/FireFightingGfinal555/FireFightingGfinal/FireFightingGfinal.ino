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


//QRE Line sensor
int QRE1113_Pin = 12;

//Blower 
int blower=13;

//Motor Pins
//Left
int MotorLeftA = 6;
int MotorLeftB = 9;
//Right
int MotorRightA = 11;
int MotorRightB = 10;

//Ultrasonic sensors Pins
//Apostasi se ekatosta
long USFrontDistance;
long USRightDistance;
long USRight2Distance;
long USLeftDistance;

long USFrontDuration;
long USRightDuration;
long USRight2Duration;
long USLeftDuration;

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
int FrontRFS = A3;
int RightLed = A5;
int LeftLed = A4;

//Loop
int DelayValue = 100;
int diff = 0;
int diff2 =0;
int diffFront = 0;
int range = 2;
 const int validDistance = 15;


//Flag
boolean flag = false;



void setup(){
  
  //GIA PINS 0-1
  UCSR0B=0;
  
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
  Serial.begin(9600); 
}//setup

void loop(){

   //Serial.println(" front ");
  
  //Serial.println(readFrontUS()); 
  //delay(1000);
  
   wallFollow();
   
   //  digitalWrite(blower, HIGH);
  //Serial.print(" mprosta: ");
  //Serial.println(readFrontUS());  
  
  //detectFire();
  // Serial.print("deksia mprosta: ");
  //Serial.println(readRight2US());
  
  //Serial.print("deksia pisw: ");
  //Serial.println(readRightUS());

/*
Serial.print("   Deksios: ");
  Serial.print(analogRead(RightFS ));
  
  Serial.print("   Mprosta Aristera: ");
  Serial.print(analogRead(FrontLFS ));
  
  Serial.print("   Aristera: ");
  Serial.println(analogRead(LeftFS));
  
  
   //loop 
  
  delay(1000);
  */
   }
   
   
   
   
void wallFollow(){
  
  
  delay(5);
  diff2 =  readRight2US() - validDistance;
  delay(5);
  diff  =  readRightUS() - validDistance;
  
  
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
    
   if( readFrontUS() < 15 )
    {
     turnLeft();
     delay(200 + readFrontUS());
    } //if 

   else if( diff < 0 ) //APOSTASH APO TON MPROSTA DEKSIA LIGO ARISTERA
     {
      goLeft();
     }//else if
   
   else if (diff > 0 ) //APOSTASH APO TON PISW DEKSIA  LIGO ARISTERA  
     {
     goRight();

     }//else if
  
  
      
   
   //NEA DIAVASMA TIMWN  
   
   //GIA STROFH  
  
  /*if ( readRightUS() <= 10 && readRight2US() > 15 ) //MPROSTA DEKSIA  KENO, PISW DEKSIA TOIXOS
   {
     moveForward();
     delay(350);
     turnRight();
     delay(260);
   }//else if
   */
   
   
   
    if(readRightUS() > 35 && readRight2US() > 35 ) //KAI TA DUO DEIXNOUN KENO
   {
     moveForward();
     delay(200);
     turnRight();
     delay(260);
     moveForward();
     delay(20* readRight2US ());
   }//else if 
   
  
  
  
   
    else if ( readRightUS() > 40 && readRight2US() > 40 && readFrontUS() > 10 )//&& readLeftUS()<25) //DEKSIA XWROS, ARISTERA TOIXOS,MPROSTA XWROS LIGO  ARISTERAAAAA 
   { 
     goRight();
     delay(50);
   }//else if
   
   
   else if (  readRight2US() < 40 && readRightUS() < 40 && readFrontUS() > 10 )//&& readLeftUS()>40 ) //ARISTERA TOIXOS , DEKSIA EXEI XWRO ,MPROSTA XWROS LIGO DEKSIAAA
   {
     goLeft();
     delay(50);
   }//else if
   
   
   
   else if (readFrontUS() > 3000   || (readFrontUS() > 60 && readRight2US() < readRightUS()))  //LOGIKO LATHOS KANEI LIGO PISW 
   {
     moveBackward();
     delay(500);
     turnLeft();
     delay(150);
   }//else if
   
   
   else  //ALLIWS MONO EUTHEIA  
   {
     moveForward();
   }//else
   //delay(5);// DELAY MEXRI TO EPOMENO DIAVASMA 
   
   
}//wallFollow








//***********Fire detecttion method*****************//

void detectFire(){ 
  
  int RFS=analogRead(RightFS);
  int LFS=analogRead(LeftFS);
  int FFSL=analogRead(FrontLFS); 
  
  
  int error = 10;
  int sideToTurn = FFSL;
  
  int k=0;
  int r=0;
  int l=0;
  
   if ( analogRead(RightFS) <750 || analogRead(FrontLFS) <750 || analogRead(LeftFS) <750){
    
     k=1;
   
   do{
       
       digitalWrite(blower, HIGH);
       
        
      if( analogRead(RightFS) < 650)
       { 
         r=1;
         
         do{
           
         
         turnRight();
         delay(2);
        
         if(analogRead(FrontLFS) >1000);
         {
           break;}
           
         
         }while(analogRead(FrontLFS) >100);
         
       }//else if
         
        
        else if ( analogRead(LeftFS) < 650 )
        {
          l=1;
          
          do{
            delay(2);
            turnLeft();  
          
          }while(  analogRead(FrontLFS) >100);
          
          
          
        }//else if
         delay(15);
       
       //ENALAKTIKA MONO GIA TON MPROSTA 
        /*
         if(analogRead(FrontLFS) <300 ){
            digitalWrite(blower, HIGH);}
            
            */
         
    if(analogRead(FrontLFS)>1000 && analogRead(RightFS)>1000 && analogRead(LeftFS)>1000){// && (r==1 || l==1) )
  
        digitalWrite(blower, LOW);
        flag=false;
        turnRight();
        delay(1000);  }//else if
     

    delay(5);
    
    }while(analogRead(FrontLFS)<1000 && analogRead(RightFS)<1000 && analogRead(LeftFS)<1000 );

   }
   
   
    
     //else if(analogRead(FrontLFS)>1000 && analogRead(RightFS)>1000 && analogRead(LeftFS)>1000 && (k==0) ){ //MPAINEI PANTA EDW
        //digitalWrite(blower, LOW);
     //} //else if
       
   
   
   
 
    
  
}//detectFire






//***********Motors methods************************//
void stopMotors(){

  digitalWrite(MotorLeftA, LOW);
  digitalWrite(MotorLeftB, LOW);
  digitalWrite(MotorRightA, LOW);
  digitalWrite(MotorRightB, LOW);
}//stopMotors

void moveBackward(){

  analogWrite(MotorLeftA, 0);
  analogWrite(MotorLeftB, 255);
  analogWrite(MotorRightA, 0);
  analogWrite(MotorRightB, 243);
  
}//moveBackward

void moveForward(){

  analogWrite(MotorLeftA, 255);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, 233);
  analogWrite(MotorRightB, 0);
}//moveForward

void turnLeft(){

  digitalWrite(MotorLeftA, LOW);
  digitalWrite(MotorLeftB, HIGH);
  digitalWrite(MotorRightA, HIGH);
  digitalWrite(MotorRightB, LOW);
  
  
}//turnLeft

void turnRight(){

  digitalWrite(MotorLeftA, HIGH);
  digitalWrite(MotorLeftB, LOW);
  digitalWrite(MotorRightA, LOW);
  digitalWrite(MotorRightB, HIGH);
}//turnRight

void goRight(){

  analogWrite(MotorLeftA, 255);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, 220);
  analogWrite(MotorRightB, 0);
}//goRight

void goLeft(){

  analogWrite(MotorLeftA, 255);
  analogWrite(MotorLeftB, 0);
  analogWrite(MotorRightA, 200);
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
  //Returns value from the QRE1113 
  //Lower numbers mean more refleacive
  //More than 3000 means nothing was reflected.
  pinMode( QRE1113_Pin, OUTPUT );
  digitalWrite( QRE1113_Pin, HIGH );  
  delayMicroseconds(10);
  pinMode( QRE1113_Pin, INPUT );

  long time = micros();

  //time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(QRE1113_Pin) == HIGH && micros() - time < 3000); //la8os
  int diff = micros() - time;

  return diff;
}//readQD
