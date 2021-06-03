#include <Wire.h>
#include <SPI.h>
#include <Adafruit_PCT2075.h>
#include <Adafruit_MCP9808.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define BUTTON_A  0
#define BUTTON_B 16
#define BUTTON_C  2

float maxTemp1 = 1;
float minTemp1 = 1000;
float maxTemp2 = 1;
float minTemp2 = 1000;
int measurements1 = 0;
int measurements2 = 0;
float sum1 = 0; 
float sum2 = 0; 
bool thresholdBreached1;
bool thresholdBreached2;

Adafruit_MCP9808 tempSensor1 = Adafruit_MCP9808();
Adafruit_MCP9808 tempSensor2 = Adafruit_MCP9808();

Adafruit_SH110X display = Adafruit_SH110X(64, 128, &Wire);

void setup() {
  Serial.begin(9600);
  while (!Serial); //waits for serial terminal to be open, necessary in newer arduino boards.

  tempSensor1.begin(0x18);
  tempSensor2.begin(0x19);
  display.begin(0x3C, true); // Address 0x3C default
      
  tempSensor1.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
  // Mode Resolution SampleTime
  //  0    0.5°C       30 ms
  //  1    0.25°C      65 ms
  //  2    0.125°C     130 ms
  //  3    0.0625°C    250 ms

  display.clearDisplay();
  display.setRotation(1);

  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  pinMode(BUTTON_C, INPUT_PULLUP); 

  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);   
}

void loop() {
  float avgTemp1;
  float avgTemp2;
  int threshold1 = 70;
  int threshold2 = 70;
  
  tempSensor1.wake();
  tempSensor2.wake();

  // Read and print out the temperature to Serial, also shows the resolution mode used for reading.
  Serial.print("Resolution in mode: ");
  Serial.println (tempSensor1.getResolution());
  float c1 = tempSensor1.readTempC();
  float c2 = tempSensor2.readTempC();
  float f1 = tempSensor1.readTempF();
  float f2 = tempSensor2.readTempF();
  Serial.print("Temp1: "); 
  Serial.print(c1, 4); Serial.print("*C\t and "); 
  Serial.print(f1, 4); Serial.println("*F.");
  Serial.print("Temp2: "); 
  Serial.print(c2, 4); Serial.print("*C\t and "); 
  Serial.print(f2, 4); Serial.println("*F.");

  //////////////////////////////////////////////////////// CALCULATE STATS FOR SENSOR 1 ////////////////////////////////////////////////////////
  if(f1 > maxTemp1) {
    Serial.println("NEW MAX TEMP1 FOUND");
    maxTemp1 = f1;
  }
  if(f1 < minTemp1) {
    Serial.println("NEW MIN TEMP1 FOUND");
    minTemp1 = f1;
  }
  if(f1 > threshold1) {
    thresholdBreached1 = true;
  }

  //////////////////////////////////////////////////////// CALCULATE STATS FOR SENSOR 2 ////////////////////////////////////////////////////////
  if(f2 > maxTemp2) {
    Serial.println("NEW MAX TEMP2 FOUND");
    maxTemp2 = f2;
  }
  if(f2 < minTemp2) {
    Serial.println("NEW MIN TEMP2 FOUND");
    minTemp2 = f2;
  }
  if(f2 > threshold2) {
    thresholdBreached2 = true;
  } 
  
  sum1 += f1;
  sum2 += f2;
  measurements1++;
  measurements2++;
  avgTemp1 = sum1 / measurements1;
  avgTemp2 = sum2 / measurements2;

  //////////////////////////////////////////////////////// BEGIN PRINTING SENSOR 1 ////////////////////////////////////////////////////////
  // Print out to display from sensor 1
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.print("currentTemp1: ");
  display.print(f1);
  display.println(" F");
  display.println("");
  
  // Print average, minimum & maximum temperatures from sensor 1
  display.print("averageTemp1: ");
  display.print(avgTemp1);
  display.println(" F");
  display.println("");
  display.print("minimumTemp1: ");
  display.print(minTemp1);
  display.println(" F");
  display.print("maximumTemp1: ");
  display.print(maxTemp1);
  display.println(" F");

  // Print threshold breached warning if true for sensor 1
  if(thresholdBreached1) {
    display.println("");
    //display.setTextColor(SH110X_RED);
    display.print("! HIGH TEMP BREACH !");
  }
  else {
    display.println("");
    //display.setTextColor(SH110X_GREEN);
    display.print("No breaches detected.");
  }
  display.display();
  delay(5000);
  
  //////////////////////////////////////////////////////// BEGIN PRINTING SENSOR 2 ////////////////////////////////////////////////////////
  // Print out to display from sensor 2
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.print("currentTemp2: ");
  display.print(f2);
  display.println(" F");
  display.println("");
  
  // Print average, minimum & maximum temperatures from sensor 2
  display.print("averageTemp2: ");
  display.print(avgTemp2);
  display.println(" F");
  display.println("");
  display.print("minimumTemp2: ");
  display.print(minTemp2);
  display.println(" F");
  display.print("maximumTemp2: ");
  display.print(maxTemp2);
  display.println(" F");

  // Print threshold breached warning if true for sensor 1
  if(thresholdBreached2) {
    display.println("");
    //display.setTextColor(SH110X_RED);
    display.print("! HIGH TEMP BREACH !");
  }
  else {
    display.println("");
    //display.setTextColor(SH110X_GREEN);
    display.print("No breaches detected.");
  }
  display.display();
  delay(5000);

  // Complete, sleep sensor and repeat
  tempSensor1.shutdown_wake(1);
  tempSensor2.shutdown_wake(1);
}
