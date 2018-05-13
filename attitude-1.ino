#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BNO055.h>
#include <utility/imumaths.h>
#include <Servo.h>

/* This driver reads raw data from the BNO055

   Connections
   ===========
   Connect SCL to analog 5      (teensy pin 19)
   Connect SDA to analog 4      (teensy pin 18)
   Connect VDD to 3.3V DC
   Connect GROUND to common ground
*/

//uncomment this line if using a Common Anode LED
#define COMMON_ANODE

//Define LED Pins
#define redPin 20 
#define greenPin 17
#define bluePin 16
#define SERVO1PIN 22 //Servo 1 Controls Roll
#define SERVO2PIN 23 //Servo 2 controls Pitch

//Setup Servos
Servo myservo;  // create servo object to control a servo ROLL
int valy;
Servo myservo2;  // create servo object to control a servo PITCH
int valz;

/* Set the delay between fresh samples */
#define BNO055_SAMPLERATE_DELAY_MS (10)

Adafruit_BNO055 bno = Adafruit_BNO055();

/**************************************************************************/
/*
    Arduino setup function (automatically called at startup)
*/
/**************************************************************************/
void setup(void)
{
  /* Initialise the sensor */
  if(!bno.begin())
  {
    /* There was a problem detecting the BNO055 ... check your connections */
    ledStatusError(); //show blue led for error
    while(1);
  }

  delay(500);

  bno.setExtCrystalUse(true);

  myservo.attach(SERVO1PIN);  // attaches the ROLL servo 
  myservo2.attach(SERVO2PIN);  // attaches the PITCH Servo
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  ledStartup(); //if we've got this far, everything's probably good so play happy lights
    
}

/**************************************************************************/
/*
    Arduino loop function, called once 'setup' is complete (your own code
    should go here)
*/
/**************************************************************************/
void loop(void)
{
  // Possible vector values can be:
  // - VECTOR_ACCELEROMETER - m/s^2
  // - VECTOR_MAGNETOMETER  - uT
  // - VECTOR_GYROSCOPE     - rad/s
  // - VECTOR_EULER         - degrees
  // - VECTOR_LINEARACCEL   - m/s^2
  // - VECTOR_GRAVITY       - m/s^2
  imu::Vector<3> euler = bno.getVector(Adafruit_BNO055::VECTOR_EULER);

  valy = euler.y(); //roll
  valy = map(valy, -90, 90, 0, 180); //map roll to work with servos
  myservo.write(valy); 

  valz = euler.z(); //pitch
  valz = map(valz, -90, 90, 0, 180);
  myservo2.write(valz);

  //show status of roll (in limit etc)
  if (valy > 60 && valy < 120) {
    ledStatusOK();
  } 
  else {
    ledTooMuchRoll();
  }

  delay(BNO055_SAMPLERATE_DELAY_MS);
}


void ledStartup()
{
  // make the LED cycle through some colors to show it's powered up
  setColor(255, 0, 0);
  delay(250);
  setColor(0, 255, 0);
  delay(250);
  setColor(0, 0, 255);
  delay(250);
}

void ledTooMuchRoll()
{
  setColor(255, 0, 0);
}

void ledStatusOK()
{
  setColor(0, 255, 0);
}
void ledStatusError()
{
  setColor(0, 0, 255);
}

// easy way to set colors on LED
void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif
  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);  
}

//the end
