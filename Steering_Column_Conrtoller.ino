/*
  This code is designed for the translation of a steering column controller
  to a pioneer radio wired remote input.
  For layout schematics see attached files.
  ~Brainiac
*/
/*==========Included Libraries==========*/
#include <SPI.h>

/*==========Pin Definitions==========*/
int powerLED = 7;
int signalLED = 6;
const int analogInPin = A0;  // Analog input pin that the controller is attached to
int CS = 10;       //Digital output pin that the SPI slave selector is connected to
//Not on this list are the pre-defined SPI pins for CLK/SCLK and MOSI/DI

/*==========Variable Definitions==========*/
int sensorValue = 0;        // value read from the controller
int outputValue = 6;        // selector value for the outputValueMap function
int refreshRate = 2;       //milliseconds between polls
byte command = 0x11;        //command byte for MC41100
int value = 0;              //value byte for MC41100
int debugMode = 1;          //selector for debugging mode.  Change to 1 for debugging
String buttonPressed = "Error"; //string for debugging through Serial Monitor

/*==========Function Definitions==========*/
int digitalPotWrite()  //define the digitalPotWrite function with a "value" paramater
{
  digitalWrite(CS, LOW);  //tell the digital pot to listen
  SPI.transfer(command);  //call the type of message we are sending
  SPI.transfer(value);    //whatever we want to set the wiper between 0 and 255
  digitalWrite(CS, HIGH); //okay, message complete.
}

int outputValueMap() //define the outputValueMap function
{
  sensorValue = analogRead(analogInPin); //read the value of the analog input
  // The following if/else statements map the sensorValue to its corresponding outputValue.
  // It then calls the debugMap function to translate it for debugging
  if ((sensorValue >= 320) && (sensorValue <= 420)) 
  {
    outputValue = 1;
    debugMap();
  }
  else if ((sensorValue >= 560) && (sensorValue <= 660)) 
  {
    outputValue = 2;
    debugMap();
  }
  else if ((sensorValue >= 710) && (sensorValue <= 810)) 
  {
    outputValue = 3;
    debugMap();
  }
  else if ((sensorValue >= 811) && (sensorValue <= 880)) 
  {
    outputValue = 4;
    debugMap();
  }
  else if ((sensorValue >= 881) && (sensorValue <= 970)) 
  {
    outputValue = 5;
    debugMap();
  }
  else if ((sensorValue >= 971) && (sensorValue <= 1024)) 
  {
    outputValue = 6;
    debugMap();
  }  
}

int debugMap() //defines the debugMap function
{
  switch (outputValue) //The following switch statement translates outputValue to a string
  { 
  case 1:
    buttonPressed = "Volume Down";
    break;
  case 2:
    buttonPressed = "Volume Up";
    break;
  case 3:
    buttonPressed = "Seek Back";
    break;
  case 4:
    buttonPressed = "Seek Forward";
    break;
  case 5:
    buttonPressed = "Menu";
    break;
  case 6:
    buttonPressed = "No Input";
    break;
  default:
    buttonPressed = "Error";
    break;
  };
}

int debugOutput() //defines the debugOutput function
{
  switch(debugMode)
  {
    case 0:
      break;
    case 1:
      Serial.print("sensor = ");
      Serial.print(sensorValue);
      Serial.print("\t output = ");
      Serial.print(outputValue);
      Serial.print("\t button = ");
      Serial.print(buttonPressed);
      Serial.print("\t wiper = ");
      Serial.println(value);
      break;
  };
}

/*==========Setup==========*/
void setup() {
  switch (debugMode) {    //check if we are in debug mode
  case 0:                 //if not,
    break;                //do nothing.
  case 1:                 //if so,
    Serial.begin(9600);   //start the Serial Monitor
    break;
  };
  pinMode (powerLED, OUTPUT);
  pinMode (signalLED, OUTPUT);
  pinMode (CS, OUTPUT);   //set the slave select pin to an output
  SPI.begin();            //begin the SPI protocol
  /* Calibrate the potentiometer */
  value = 0x00;
  digitalPotWrite();  //set the wiper to the lowest point
  delay(1000);
  value = 0x80;
  digitalPotWrite();  //set the wiper to the midpoint
  delay(1000); 
  value = 0xFF;
  digitalPotWrite();  //set the wiper to the highest point
  delay(1000);
}

/*==========Loop==========*/
void loop() {
  digitalWrite(powerLED, HIGH);
  switch (outputValue)
  {
    case 1:
      outputValueMap();
      value = 70;
      digitalPotWrite();
      debugOutput();
      digitalWrite(signalLED, HIGH);
      break;
    case 2:
      outputValueMap();
      value = 46;
      digitalPotWrite();
      debugOutput();
      digitalWrite(signalLED, HIGH);
      break;
    case 3:
      outputValueMap();
      value = 32;
      digitalPotWrite();
      debugOutput();
      digitalWrite(signalLED, HIGH);
      break;
    case 4:
      outputValueMap();
      value = 23;
      digitalPotWrite();
      debugOutput();
      digitalWrite(signalLED, HIGH);
      break;
    case 5:
      outputValueMap();
      value = 3;
      digitalPotWrite();
      debugOutput();
      digitalWrite(signalLED, HIGH);
      break;
    case 6:
      outputValueMap();
      value = 255;
      digitalPotWrite();
      debugOutput();
      digitalWrite(signalLED, LOW);
      break;   
  };
  delay(refreshRate);
}

