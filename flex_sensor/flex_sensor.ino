/******************************************************************************
Flex_Sensor_Example.ino
Example sketch for SparkFun's flex sensors
  (https://www.sparkfun.com/products/10264)
Jim Lindblom @ SparkFun Electronics
April 28, 2016

Create a voltage divider circuit combining a flex sensor with a 47k resistor.
- The resistor should connect from A0 to GND.
- The flex sensor should connect from A0 to 3.3V
As the resistance of the flex sensor increases (meaning it's being bent), the
voltage at A0 should decrease.

Development environment specifics:
Arduino 1.6.7
******************************************************************************/
const int FLEX_PIN = A0; // Pin connected to voltage divider output

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE = 37300.0; // resistance when straight
const float BEND_RESISTANCE = 90000.0; // resistance at 90 deg

int flexADC = analogRead(FLEX_PIN);
float flexV = flexADC * VCC / 1023.0;
float flexR = R_DIV * (VCC / flexV - 1.0);
float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 90.0);

unsigned long time = millis();
unsigned long ref_time = 0;
unsigned long current_time = time - ref_time;
unsigned long previous_time = 0;

float thresholdValue = 70;
int count = 0;
int measure = 1;
int countAgain = 0;

void setup() 
{
  Serial.begin(9600);
  pinMode(FLEX_PIN, INPUT);
}

void loop() 
{
  time = millis();
  current_time = time - ref_time;
  if (current_time < 10000 && measure == 1) {
    readFlex();
    Serial.print("Bend: ");
    Serial.println(angle);
    Serial.println();
    if (angle >= thresholdValue)  {
      count++;
      Serial.println("Count: ");
      Serial.println(count);
    }
  }
  else if (current_time > 10000 && measure ==1)  {
    Serial.print("Final Count: ");
    Serial.println(count);
    ref_time = current_time;
    count = 0;
    measure = 0;
  }
}

void readFlex() {
    // Read the ADC, and calculate voltage and resistance from it
  flexADC = analogRead(FLEX_PIN);
  flexV = flexADC * VCC / 1023.0;
  flexR = R_DIV * (VCC / flexV - 1.0);
//  Serial.println("Resistance: " + String(flexR) + " ohms");

  // Use the calculated resistance to estimate the sensor's
  // bend angle:
  angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 90.0);
//  Serial.println("Bend: " + String(angle) + " degrees");
  
  delay(500);
}

