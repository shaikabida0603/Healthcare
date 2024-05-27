# Healthcare
Our approach is to continuously monitor the patient condition with IOT device and give updates to the person who is in the charge of taking care of him like doctor or nurse.
#include <Wire.h>  // i2c library for Arduino.


// MACRO Definitions
#define VALVE_PIN (5)          // checks if the measurement is done.
#define MEASURE_BEGIN_PIN (4)  // indicates that a measurement should start. this can be connected to switch or another MCU or Raspberry Pi.

volatile byte i2c_data_rx;      // indicates there are available data from the i2c bus.
volatile uint16_t count;        // indicates the total number of data collected.
volatile uint8_t sys, dia, hr;  // stored the measure values: systolic, diastolic, and heart rate.

// the setup function runs once when you press reset or power the board
void setup() {
  Serial.begin(9600); // Initialize SoftwareSerial for communication with ESP8266
  pinMode(6, OUTPUT);
  pinMode(5, INPUT);
  pinMode(4, INPUT_PULLUP);
  Wire.begin(0x50);              // the address of the EEPROM is 0x50. The Arduino should be the same.
  Wire.onReceive(receiveEvent);  // this is the interrupt initialization for the i2c data.
  Serial.print("Start");
  digitalWrite(6,0);
  delay(2000);
}

// the loop function runs over and over again forever
void loop() {
  delay(3000);
  digitalWrite(6, 0);  // turn the LED on (HIGH is the voltage level)
  delay(1000);                      // wait for a second
  digitalWrite(6, 126);   // turn the LED off by making the voltage LOW
  delay(1000);                      // wait for a second
  Serial.println("Start BP check...");
  delay(2000);  // need to secure that the value is already closed.
  while (digitalRead(VALVE_PIN) == 0) {
    Serial.println("BP On-going...");
    delay(1000);
  }
  delay(3000);
  digitalWrite(6, 0);
  delay(1200);
  digitalWrite(6, 126);
delay(1000);
  delay(500);

  if (count == 0) {
    Serial.println("No data");
  } else if (count == 35) {
    Serial.println("Blood Pressure Data: ");

    Serial.println(sys);
    Serial.println(dia);
    Serial.println(hr);

    // Send data to ESP82  
  } else {
    Serial.println("error");
  }

  count = 0;
}

void receiveEvent(int iData)  // Interrupt service routine.
{
  if (iData > 0) {
    while (iData--) {
      i2c_data_rx = Wire.read();
      count++;

      if (count == 28) {
        sys = i2c_data_rx;
      }
      if (count == 29) {
        dia = i2c_data_rx;
      }
      if (count == 30) {
        hr = i2c_data_rx;
      }
    }
  }
}
