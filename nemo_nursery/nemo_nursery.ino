
//***Nursery Study Device Firmware***
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

float flexV = flexADC * VCC / 1023.0;
float flexR = R_DIV * (VCC / flexV - 1.0);
float angle = map(flexR, STRAIGHT_RESISTANCE, BEND_RESISTANCE,
                   0, 90.0);

float flexADC = analogRead(FLEX_PIN);
float accelADC = analogRead(ACCEL_PIN);
float tempADC = analogRead(TEMP_PIN);


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

  //wait 10 seconds before measuring and turning LED high
  delay(10000);

  //findBaseline();
}


void loop() {
  time = millis();
  current_time = time - ref_time;
// Measure for Respiratory Rate (5x 65sec tests)
  if (count < 5)  {
    if (current_time < previousTime + measureTime) {
      digitalWrite(LED, HIGH)
      measureResp();
      Serial.print(flexADC);
      Serial.print(" ");
      Serial.print(accelADC);
    }
    else if (current_time > previousTime + measureTime)  {
      previousTime = current_time + 100000;
      count = count + 1;
      Serial.print("END SAMPLE");
      digitalWrite(LED,LOW);
      delay(10000);
    }
  }
  
// Measure for Temperature (1x 65sec test)
  else if (count == 5) {
    if (current_time < previousTime + measureTime) {
      digitalWrite(LED, HIGH)
      measureTemp();
      Serial.print(tempADC);
    }
    else if (current_time > previousTime + measureTime)  {
      count = count + 1;
      Serial.print("END SAMPLE");
      digitalWrite(LED,LOW);
    }
  }
}

void measureResp() {
  flexADC = analogRead(FLEX_PIN);
  accelADC = analogRead(ACCEL_PIN);
}

void measureTemp()  {
  tempADC = analogRead(TEMP_PIN);
  tempADC = tempADC / 1024;   //find percentage of input reading
  tempADC = tempADC * 5;      //multiply by 5V to get voltage
  tempADC = tempADC - 0.5;    //Subtract the offset 
  tempADC = tempADC * 100;    //Convert to degrees 
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

// Thresholding count algorithm
//    if (angle >= thresholdValue && previousValue < thresholdValue)  {
//      count++;
//      digitalWrite(LED,HIGH);
//      delay(50);
//      digitalWrite(LED,LOW);
//      delay(50);
//      previousValue = angle;
//    }
//    else {
//    previousValue = angle;
//    }
