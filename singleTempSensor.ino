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

Adafruit_MCP9808 tempSensor = Adafruit_MCP9808();
Adafruit_SH110X display = Adafruit_SH110X(64, 128, &Wire);

void setup() {
  Serial.begin(9600);
  while (!Serial);

  tempSensor.begin(0x18);
  display.begin(0x3C, true);
      
  tempSensor.setResolution(3);
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
}

void loop() {
  // SET threshold VALUE TO TEMPERATURE TO ALERT WHEN EXCEEDED
  int threshold = 73;
  float avgTemp;

  // PUSHING BUTTON A RESETS AVERAGE
  if(!digitalRead(BUTTON_A)) sum = measurements = 0; 
  // PUSHING BUTTON B RESETS MIN/MAX
  if(!digitalRead(BUTTON_A)) {
    minTemp = 1000;
    maxTemp = 1;
  }  
  // PUSHING BUTTON C RESETS HIGH TEMP WARNING
  if(!digitalRead(BUTTON_C)) {
    !thresholdBreached;
  }
  
  tempSensor.wake();

  // Read and print out the temperature to Serial, also shows the resolution mode used for reading.
  Serial.print("Resolution in mode: ");
  Serial.println (tempSensor.getResolution());
  float c = tempSensor.readTempC();
  float f = tempSensor.readTempF();
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
  display.setTextColor(SH110X_WHITE, SH110X_BLACK);
  display.setCursor(0,0);
  display.print("Current Temp: "); display.print(f); display.println(" F");
  display.println("");
  
  // Print average, minimum & maximum temperatures
  display.print("Average Temp: "); display.print(avgTemp); display.println(" F");
  display.println("");
  display.print("Minimum Temp: "); display.print(minTemp); display.println(" F");
  display.print("Maximum Temp: "); display.print(maxTemp); display.println(" F");

  // Print threshold breached warning if true
  if(thresholdBreached) {
    display.println("");
    display.setTextColor(SH110X_BLACK, SH110X_WHITE);
    display.print("! HIGH TEMP WARNING !");
  }
  else {
    display.println("");
    display.print("No breaches detected.");
  }
  display.display();
  
  tempSensor.shutdown_wake(1);
}
