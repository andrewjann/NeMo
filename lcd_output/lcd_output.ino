/*********************************************************************
  This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

  This example is for a 128x32 size display using I2C to communicate
  3 pins are required to interface (2 I2C and one reset)

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!

  Written by Limor Fried/Ladyada  for Adafruit Industries.
  BSD license, check license.txt for more information
  All text above, and the splash screen must be included in any redistribution
*********************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2


#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16
static const unsigned char PROGMEM logo16_glcd_bmp[] =
{ B00000000, B11000000,
  B00000001, B11000000,
  B00000001, B11000000,
  B00000011, B11100000,
  B11110011, B11100000,
  B11111110, B11111000,
  B01111110, B11111111,
  B00110011, B10011111,
  B00011111, B11111100,
  B00001101, B01110000,
  B00011011, B10100000,
  B00111111, B11100000,
  B00111111, B11110000,
  B01111100, B11110000,
  B01110000, B01110000,
  B00000000, B00110000
};

#if (SSD1306_LCDHEIGHT != 32)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

// ***flex sensor***
const int FLEX_PIN = A0; // Pin connected to voltage divider output
const int TEMP_PIN = A1;
const int ACCEL_PIN = A3;

// Measure the voltage at 5V and the actual resistance of your
// 47k resistor, and enter them below:
const float VCC = 4.98; // Measured voltage of Ardunio 5V line
const float R_DIV = 47500.0; // Measured resistance of 3.3k resistor

// Upload the code, then try to adjust these values to more
// accurately calculate bend degree.
const float STRAIGHT_RESISTANCE = 37300.0; // resistance when straight
const float BEND_RESISTANCE = 90000.0; // resistance at 90 deg

float flexADC = analogRead(FLEX_PIN);
float accelADC = analogRead(ACCEL_PIN);
float tempADC = analogRead(TEMP_PIN);

float flexV = flexADC * VCC / 1023.0;
float flexR = R_DIV * (VCC / flexV - 1.0);
float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 90.0);

int LED = 17;
unsigned long time = millis();
unsigned long ref_time = 0;
unsigned long current_time = time - ref_time;
unsigned long previous_time = 0;
unsigned long measureTime = 65000;

float sum;
float value;
float baseline;
float thresholdValue = baseline + 4;
float previousValue = 0;
int count = 0;
int measure = 1;
int countAgain = 0;

int resetPin = 12;


void setup()   {
  digitalWrite(resetPin, HIGH);
  Serial.begin(9600);
  pinMode(FLEX_PIN, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(resetPin, OUTPUT);

  // by default, we'll generate the high voltage from the 3.3v line internally! (neat!)
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)
  display.clearDisplay();
  // init done

  // text display tests
  // function list
/*  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("PLACE");
  display.println("ON BABY"); 
  //  display.setTextColor(BLACK, WHITE); // 'inverted' text 
  display.display(); */

  //findBaseline();
 
/*  delay(2000);
  display.clearDisplay(); */

  /*  // miniature bitmap display
    display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
    display.display();
    delay(1);

    // invert the display
    display.invertDisplay(true);
    delay(1000);
    display.invertDisplay(false);
    delay(1000);
    display.clearDisplay();

    // draw a bitmap icon and 'animate' movement
    testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);*/
}


void loop() {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println("Respiratory Rate:");
  display.display();
  time = millis();
  current_time = time - ref_time;
  if (current_time < measureTime && measure == 1) {
    display.println("Measuring...");
    display.display();
    readFlex();
    Serial.print(flexADC);
    Serial.print(" ");
    Serial.print(accelADC);
    Serial.print(" ");
    Serial.println(tempADC);
//    display.setTextSize(1);
//    display.print("Bend: ");
//    display.println(angle);
//    display.display();
//    delay(100);
//    display.clearDisplay();

    if (angle >= thresholdValue && previousValue < thresholdValue)  {
      count++;
      digitalWrite(LED,HIGH);
      delay(50);
      digitalWrite(LED,LOW);
      delay(50);
      previousValue = angle;
    }
    else {
    previousValue = angle;
    }
  }
  else if (current_time > measureTime && measure == 1)  {
    display.clearDisplay();
    display.setTextSize(2);
//    display.print("Final Count: ");
    display.print(String(count));

    if (count > 5)  {
      display.print(" Unsafe");
      display.display();
    }
    else if (count <= 5)  {
      display.print(" Safe :)");
      display.display();
    }
    ref_time = current_time;
    count = 0;
    measure = 0;
  }
}

void readFlex() {
  // Read the ADC, and calculate voltage and resistance from it
/*  flexADC = analogRead(FLEX_PIN);
  flexV = flexADC * VCC / 1023.0;
  flexR = R_DIV * (VCC / flexV - 1.0); */
  //  Serial.println("Resistance: " + String(flexR) + " ohms");

  // Use the calculated resistance to estimate the sensor's
  // bend angle:
/*  angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
              0, 90.0); */
  //  Serial.println("Bend: " + String(angle) + " degrees");

//  delay(100);
  flexADC = analogRead(FLEX_PIN);
  accelADC = analogRead(ACCEL_PIN);
  tempADC = analogRead(TEMP_PIN);
  tempADC = tempADC / 1024;   //find percentage of input reading
  tempADC = tempADC * 5;                     //multiply by 5V to get voltage
  tempADC = tempADC - 0.5;                   //Subtract the offset 
  tempADC = tempADC * 100;                   //Convert to degrees 
}

void findBaseline() {
  for (int i = 0; i < 10; i++)  {
    readFlex();
    value = angle;
    sum += value;
    Serial.println(angle);
  }
  baseline = sum/10;
  thresholdValue = baseline + 4;
  Serial.print("Baseline: ");
  Serial.println(baseline);
}


void dotDotDot()  {
  display.setTextSize(2);
  display.println(".");
  display.display();
  delay(500);
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Flex Sensor readings:");
  display.setTextSize(2);
  display.println("..");
  display.display();
  delay(500);
  display.clearDisplay();
  display.setCursor(0,0);
  display.setTextSize(1);
  display.println("Flex Sensor readings:");
  display.setTextSize(2);
  display.println("...");
  display.display();
  delay(500);
  display.clearDisplay();
}
