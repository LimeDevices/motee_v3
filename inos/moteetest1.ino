#include <SoftwareSerial.h>

#define SSerialRX         4  //Serial Receive pin
#define SSerialTX         3  //Serial Transmit pin
#define backSurface       A0 //back surface sensor high when surface is near
#define frontLeftSurface  A1 //front left surface sensor vcc when surface is near
#define frontRightSurface A2 //front right surface sensor vcc when surface is near 
#define SSerialTxControl 2   //RS485 Direction control

#define RS485Transmit    HIGH
#define RS485Receive     LOW

SoftwareSerial RS485Serial(SSerialRX, SSerialTX);

char bytesReceived[10];
char moteeR[5];
char moteeL[5]; 
char dir = 0; 
int speedrcv; 
int frontl; 
int frontr; 
int back; 

void setup()
{
  
  Serial.begin(115200);
    
  pinMode(SSerialTxControl, OUTPUT);

  pinMode(backSurface, INPUT); 
  pinMode(frontRightSurface, INPUT);
  pinMode(frontLeftSurface, INPUT);
  
  digitalWrite(SSerialTxControl, RS485Receive);  // Init Transceiver   
  
  // Start the software serial port, to another device
  RS485Serial.begin(9600);   // set the data rate 

  moteeR[2] = (char)20; 
  moteeR[3] = 0x0A; 
  moteeR[4] = '\0'; 
  moteeR[0] = '0'; 
  moteeR[1] = 'f';

  moteeL[2] = (char)20; 
  moteeL[3] = 0x0A; 
  moteeL[4] = '\0'; 
  moteeL[0] = '1'; 
  moteeL[1] = 'f';
}


void loop()
{


  frontr = analogRead(frontRightSurface);
  frontl = analogRead(frontLeftSurface); 
  back = digitalRead(backSurface); 
 

  if (Serial.available())  {

    dir = Serial.read(); 
    
  }

    //Serial.println(frontl); 

  
    if(frontl<300){
      reverse(150); 
      delay(70); 
      stop();  
    } 
    
    if(frontr<300){
      reverse(150); 
      delay(70); 
      stop();  
    } 

    
    if(back==1){
      forward(150); 
      delay(70); 
      stop(); 
    }

    if(dir=='w'){
      //Serial.println("prosto"); 
      forward(255); 
      //dir = 'o'; 
    }
    if(dir=='a'){
      //Serial.println("lweo"); 
      left(200);
      //dir = 'o'; 
    }
    if(dir=='s'){
      //Serial.println("tyl"); 
      reverse(255); 
      //dir = 'o'; 
    }
    if(dir=='d'){
      //Serial.println("prawo"); 
      right(200); 
      //dir = 'o'; 
    }

    if(dir=='n'){
      //Serial.println("stop"); 
      stop();  
      dir = 'o'; 
    }


}

void updatemotees(){

    digitalWrite(SSerialTxControl, RS485Transmit);  // Enable RS485 Transmit   
    RS485Serial.write(moteeR);
    delay(5); 
    RS485Serial.write(moteeL);
    delay(5);
    digitalWrite(SSerialTxControl, RS485Receive);  // Disable RS485 Transmit     

  
}

void forward(int value){

  moteeR[2] = (char)value; 
  moteeR[1] = 'f';

  moteeL[2] = (char)value; 
  moteeL[1] = 'f';
  updatemotees(); 
  delay(20);
}

void reverse(int value){

  moteeR[2] = (char)value; 
  moteeR[1] = 'r';

  moteeL[2] = (char)value; 
  moteeL[1] = 'r';
  updatemotees(); 
  delay(20);
}

void left(int value){

  moteeR[2] = (char)value; 
  moteeR[1] = 'f';

  moteeL[2] = (char)value; 
  moteeL[1] = 'r';
  updatemotees(); 
  delay(20);
}

void right(int value){

  moteeR[2] = (char)value; 
  moteeR[1] = 'r';

  moteeL[2] = (char)value; 
  moteeL[1] = 'f';
  updatemotees(); 
  delay(20);
}

void stop(){
  
  moteeR[1] = 'n'; 
  moteeL[1] = 'n';
  updatemotees(); 
  delay(50);
}


