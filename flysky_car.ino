/*  
A basic receiver using the nRF24L01 module to receive 4 channels and convert them to PPM.
 */

#include <SPI.h>
//#include <nRF24L01.h>
//#include <RF24.h>

byte PWM_PIN1 = 9;
byte PWM_PIN2 = 10;
int pwm_value1, pwm_value2;
//const uint64_t pipeIn =  0xE8E8F0F0E1LL;

//RF24 radio(9, 10);

// The sizeof this struct should not exceed 32 bytes
struct MyData {
  int throttle;
  int yaw;
  int pitch;
  int roll;
  byte AUX1;
  byte AUX2;
  byte AUX3;
  byte AUX4;
};

MyData data;

// Motor A
 
int enA = 3;
int in1 = 2;
int in2 = 4;
 
// Motor B
 
int enB = 6;
int in3 = 5;
int in4 = 7;
 
// Joystick Input
 
//int joyVert = A0; // Vertical  
//int joyHorz = A1; // Horizontal
 
// Motor Speed Values - Start at zero
 
int MotorSpeed1 = 0;
int MotorSpeed2 = 0;
 
// Joystick Values - Start at 512 (middle position)
 
int joyposVert = 512;
int joyposHorz = 512;  
 
 


void resetData() 
{
  // 'safe' values to use when no radio input is detected
  data.throttle = 0;
  data.yaw = 127;
  data.pitch = 127;
  data.roll = 127;
  
//  setPPMValuesFromData();
}




void setup()
{  
  resetData();
  //setupPPM();
  
  // Set up radio module
  //radio.begin();
  //radio.setDataRate(RF24_250KBPS); // Both endpoints must have this set the same
  //radio.setAutoAck(false);

  //radio.openReadingPipe(1,pipeIn);
  //radio.startListening();
  pinMode(PWM_PIN1, INPUT);
  pinMode(PWM_PIN2, INPUT);
  
  Serial.begin(9600);

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

unsigned long lastRecvTime = 0;

void recvData()
{  
  pwm_value1 = pulseIn(PWM_PIN1, HIGH);
  pwm_value2 = pulseIn(PWM_PIN2, HIGH);
  data.pitch =pwm_value2;
  data.roll =pwm_value1;
  
}

/**************************************************/

void loop()
{
  recvData();

    
    Serial.print(data.pitch);
    Serial.print("\t");
    Serial.print(data.roll);
    Serial.print("\t");
    
    

  
  data.pitch=map(data.pitch,990,1990,0,255);
  data.pitch=constrain(data.pitch,0,255);
   data.roll=map(data.roll,990,1990,0,255);
  data.roll=constrain(data.roll,0,255);

    Serial.print("\t");
    Serial.print(data.pitch);
    Serial.print("\t");
    Serial.print(data.roll);
    Serial.print("\t\t");
   
   
  
  // Read the Joystick X and Y positions
 if(data.AUX1==1)
 {
    data.pitch-=255;
    data.pitch*=-1;
    data.roll-=255;
    data.roll*=-1;
 }
 joyposVert = map(data.pitch,0,255,0,1023); 
 joyposHorz = map(data.roll,0,255,0,1023);
 // Determine if this is a forward or backward motion
 // Do this by reading the Verticle Value
 // Apply results to MotorSpeed and to Direction
    Serial.print(joyposVert);
    Serial.print("\t");
    Serial.println(joyposHorz);
if(joyposVert>490&&joyposVert<530&&(joyposHorz<480||joyposHorz>520))
{
  if(joyposHorz>520)
   {
      digitalWrite(in1,LOW);
      digitalWrite(in2,HIGH);

      digitalWrite(in3,HIGH);
      digitalWrite(in4,LOW);
      MotorSpeed1=255;
      MotorSpeed2=255;
   }
   else if(joyposHorz<480)
   {
      digitalWrite(in1,HIGH);
      digitalWrite(in2,LOW);

      digitalWrite(in3,LOW);
      digitalWrite(in4,HIGH);
      MotorSpeed1=255;
      MotorSpeed2=255; 
   }

 
}
 else if (joyposVert < 460)
 {
 // This is Backward
 
 // Set Motor A backward
 
 digitalWrite(in1, LOW);
 digitalWrite(in2, HIGH);
 
 // Set Motor B backward
 
 digitalWrite(in3, LOW);
 digitalWrite(in4, HIGH);
 
 //Determine Motor Speeds
 
 // As we are going backwards we need to reverse readings
 
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
 
 // Now do the steering
 // The Horizontal position will "weigh" the motor speed
 // Values for each motor
 
 if (joyposHorz < 460)
 {
 // Move Left
 
 // As we are going left we need to reverse readings
 
 joyposHorz = joyposHorz - 460; // This produces a negative number
 joyposHorz = joyposHorz * -1;  // Make the number positive
 
 // Map the number to a value of 255 maximum
 
 joyposHorz = map(joyposHorz, 0, 460, 0, 255);
   
 
 MotorSpeed1 = MotorSpeed1 - joyposHorz;
 MotorSpeed2 = MotorSpeed2 + joyposHorz;
 
 // Don't exceed range of 0-255 for motor speeds
 
 if (MotorSpeed1 < 0)MotorSpeed1 = 0;
 if (MotorSpeed2 > 255)MotorSpeed2 = 255;
 
 }
 else if (joyposHorz > 564)
 {
 // Move Right
 
 // Map the number to a value of 255 maximum
 
 joyposHorz = map(joyposHorz, 564, 1023, 0, 255);
   
 
 MotorSpeed1 = MotorSpeed1 + joyposHorz;
 MotorSpeed2 = MotorSpeed2 - joyposHorz;
 
 // Don't exceed range of 0-255 for motor speeds
 
 if (MotorSpeed1 > 255)MotorSpeed1 = 255;
 if (MotorSpeed2 < 0)MotorSpeed2 = 0;   
 
 }
 
 
 // Adjust to prevent "buzzing" at very low speed
 
 if (MotorSpeed1 < 30)
    MotorSpeed1 = 0;
 if (MotorSpeed2 < 30 )
    MotorSpeed2 = 0;
 
 // Set the motor speeds
 
 analogWrite(enA, MotorSpeed1);
 analogWrite(enB, MotorSpeed2);
  //setPPMValuesFromData();
  
}


