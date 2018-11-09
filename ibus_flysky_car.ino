#include "FlySkyIBus.h"
// Motor A
 
int enA = 3;
int in1 = 2;
int in2 = 4;
 
// Motor B
 
int enB = 6;
int in3 = 5;
int in4 = 7;
 
// Motor Speed Values - Start at zero

float MotorSpeed1 = 0;
float MotorSpeed2 = 0;

// The sizeof this struct should not exceed 32 bytes
struct MyData {
  int pitch;
  int roll;
  int s;
};

MyData data;

void resetData() 
{

  data.pitch = 512;
  data.roll = 512;
  data.s=0;

}



/**************************************************/



void setup()
{  
  
  resetData();
  Serial.begin(9600);
  IBus.begin(Serial);

  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
   
  // Start with motors disabled and direction forward
  
  // Motor A
  
  digitalWrite(enA, LOW);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  
  // Motor B
  
  digitalWrite(enB, LOW);
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);

}

/**************************************************/



void recvData()
{  
  IBus.loop();
  data.pitch=IBus.readChannel(1);
  data.roll=IBus.readChannel(0);
  data.s=IBus.readChannel(4);
}

/**************************************************/
int joyposVert=512;
int joyposHorz=512;
float ratio;
void loop()
{
  recvData();

  
        joyposVert =map(data.pitch,1000,2000,0,1023);
        joyposHorz = map(data.roll,1000,2000,0,1023);
      
 // Serial.print("\n");
 // Serial.print(joyposVert);
 // Serial.print("\t");
 // Serial.print(joyposHorz);
  if (joyposVert < 460)
  {
    // This is Backward
 
    // Set Motor A backward
 
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
 
    // Set Motor B backward
 
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
 
 
    //going backwards we need to reverse readings
 
    joyposVert = joyposVert - 460; // This produces a negative number
    joyposVert = joyposVert * -1;  // Make the number positive
 
    MotorSpeed1 = map(joyposVert, 0, 460, 0, 255);
    MotorSpeed2 = map(joyposVert, 0, 460, 0, 255);
 
  }
  else if (joyposVert > 564)
  {
    // This is Forward
 
    // Set Motor A forward
 
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
 
    // Set Motor B forward
 
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
 
    //Determine Motor Speeds
 
    MotorSpeed1 = map(joyposVert, 564, 1023, 0, 255);
    MotorSpeed2 = map(joyposVert, 564, 1023, 0, 255); 
 
  }
  else
  {
    // This is Stopped
 
    MotorSpeed1 = 0;
    MotorSpeed2 = 0; 
 
  }

 
  if (joyposHorz < 460)
  {
    // Move Left
 
    //going left we need to reverse readings
 
    joyposHorz = joyposHorz - 460; // This produces a negative number
    joyposHorz = joyposHorz * -1;  // Make the number positive
 
    // 255 maximum
 
    joyposHorz = map(joyposHorz, 0, 460, 0, 255);
        
 
    MotorSpeed1 =( MotorSpeed1 - joyposHorz);
    MotorSpeed2 = (MotorSpeed2 + joyposHorz);
 
    // 0-255 for motor speeds
 
    if (MotorSpeed1 < 0)MotorSpeed1 = 0;
    if (MotorSpeed2 > 255)MotorSpeed2 = 255;
 
  }
  else if (joyposHorz > 564)
  {
    // Move Right
 
    //255 maximum
 
    joyposHorz = map(joyposHorz, 564, 1023, 0, 255);
        
 
    MotorSpeed1 =( MotorSpeed1 + joyposHorz);
    MotorSpeed2 =( MotorSpeed2 - joyposHorz);
 
    // 0-255 for motor speeds
 
    if (MotorSpeed1 > 255)MotorSpeed1 = 255;
    if (MotorSpeed2 < 0)MotorSpeed2 = 0;      
 
  }
 
 
  // Adjust to prevent "buzzing" at very low speed

  if (MotorSpeed1 < 40)MotorSpeed1 = 0;
  if (MotorSpeed2 < 40)MotorSpeed2 = 0;
  data.s-=1000;
  ratio=data.s/100;
  MotorSpeed1=(float)MotorSpeed1*ratio/10;
  MotorSpeed2=(float)MotorSpeed2*ratio/10;
  analogWrite(enA, (int)MotorSpeed1);
  analogWrite(enB, (int)MotorSpeed2);  



  Serial.print("\n");
    
    Serial.print(data.pitch);
    Serial.print("\t");
    Serial.print(data.roll);
    Serial.print("\t");
    Serial.print(data.s);
    Serial.print("\t");
    Serial.print(MotorSpeed1);
    Serial.print("\t");
    Serial.print(MotorSpeed2);
    Serial.print("\t");
    Serial.print(ratio);
}

/**************************************************/
