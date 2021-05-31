#include <Wire.h>
#include <Adafruit_MCP9808.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define BUTTON_A  0
#define BUTTON_B 16
#define BUTTON_C  2

float maxTemp = 1;
float minTemp = 1000;
int measurements = 0;
float sum = 0; 
bool thresholdBreached;

Adafruit_MCP9808 tempsensor = Adafruit_MCP9808();
Adafruit_SH110X display = Adafruit_SH110X(64, 128, &Wire);

void setup() {
  Serial.begin(9600);
  while (!Serial); //waits for serial terminal to be open, necessary in newer arduino boards.

  tempsensor.begin(0x18);
  display.begin(0x3C, true); // Address 0x3C default
      
  tempsensor.setResolution(3); // sets the resolution mode of reading, the modes are defined in the table bellow:
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
  float avgTemp;
  int threshold = 70;
  
  Serial.println("wake up MCP9808.... "); // wake up MCP9808 - power consumption ~200 mikro Ampere
  tempsensor.wake();   // wake up, ready to read!

  // Read and print out the temperature to Serial, also shows the resolution mode used for reading.
  Serial.print("Resolution in mode: ");
  Serial.println (tempsensor.getResolution());
  float c = tempsensor.readTempC();
  float f = tempsensor.readTempF();
  Serial.print("Temp: "); 
  Serial.print(c, 4); Serial.print("*C\t and "); 
  Serial.print(f, 4); Serial.println("*F.");

  if(f > maxTemp) {
    Serial.println("NEW MAX TEMP FOUND");
    maxTemp = f;
  }
  if(f < minTemp) {
    Serial.println("NEW MIN TEMP FOUND");
    minTemp = f;
  }
  if(f > threshold) {
    thresholdBreached = true;
  }
  
  sum += f;
  measurements++;
  avgTemp = sum / measurements;
  
  // Print out to display
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0,0);
  display.print("Current Temp: ");
  display.print(f);
  display.println(" F");
  display.println("");
  
  // Print average, minimum & maximum temperatures
  display.print("Average Temp: ");
  display.print(avgTemp);
  display.println(" F");
  display.println("");
  display.print("Minimum Temp: ");
  display.print(minTemp);
  display.println(" F");
  display.print("Maximum Temp: ");
  display.print(maxTemp);
  display.println(" F");

  // Print threshold breached warning if true
  if(thresholdBreached) {
    display.println("");
    display.setTextColor(SH110X_RED);
    display.print("! HIGH TEMP WARNING !");
  }
  else {
    display.println("");
     display.setTextColor(SH110X_GREEN);
    display.print("No breaches detected.");
  }
  display.display();
  
  //delay(2000);
  //Serial.println("Shutdown MCP9808.... ");
  tempsensor.shutdown_wake(1); // shutdown MSP9808 - power consumption ~0.1 mikro Ampere, stops temperature sampling
  //Serial.println("");
  //delay(200);
}
