int flex_pin = A0;
int temp_pin = A1;
int accel_pin = A3;
float flexADC;
float tempADC;
float accelADC;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  flexADC = analogRead(flex_pin);
  accelADC = analogRead(accel_pin);
  tempADC = analogRead(temp_pin);
  tempADC = tempADC / 1024;   //find percentage of input reading
  tempADC = tempADC * 5;      //multiply by 5V to get voltage
  tempADC = tempADC - 0.5;    //Subtract the offset 
  tempADC = tempADC * 100;    //Convert to degrees 
  Serial.print("Flex: ");
  Serial.print(flexADC);
  Serial.print(" Accel: ");
  Serial.print(accelADC);
  Serial.print(" Temp: ");
  Serial.println(tempADC);
  delay(100);
  
}
