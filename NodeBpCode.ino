#include <ESP8266WiFi.h>

const char *ssid = "Kesava datta's Galaxy M33 5G";
const char *password = "moyf27923";
const char *apiKey = "18I9GXANMRNWONCI";

String receivedData = "";

void setup() {
  Serial.begin(9600);
  pinMode(D5, OUTPUT);
  // digitalWrite(D5, LOW); // Initialize the LED to OFF state
  connectToWiFi();
}

void loop() {
  if (Serial.available() > 0) {
    char data = Serial.read();
    Serial.print(data);
    receivedData += data; // Append received character to the String
    if (receivedData.length() >= 8) { // Check if the data length is at least 8 characters
      sendDataToThingSpeak(receivedData);
      receivedData = ""; // Clear the received data after processing
    }
  }
}

void connectToWiFi() {
  Serial.println("Connecting to WiFi...");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting...");
  }

  Serial.println("Connected to WiFi");
  // digitalWrite(D5, LOW); // Turn off the light after WiFi connection is established
}

void sendDataToThingSpeak(String data) {
  int sys,dia,hr;
  if (data.length() == 8) { // Ensure the data is exactly 8 characters long
    // int sys = data.substring(0, 3).toInt();  // First 3 characters for systolic
    // int dia = data.substring(3, 6).toInt();  // Next 3 characters for diastolic
    // int hr = data.substring(6).toInt();      // Last 2 characters for heart rate

    // Serial.print("Sys: ");
    // Serial.println(sys);
    // Serial.print("Dia: ");
    // Serial.println(dia);
    // Serial.print("HR: ");
    // Serial.println(hr);
    // Find the positions of spaces
    // int space1 = data.indexOf(' ');
    // int space2 = data.lastIndexOf(' ');

    // // Extract substrings
    // String sys_str = data.substring(0, space1);
    // String dia_str = data.substring(space1 + 1, space2);
    // String hr_str = data.substring(space2 + 1);

    // // Convert strings to integers
    // int sys = sys_str.toInt();
    // int dia = dia_str.toInt();
    // int hr = hr_str.toInt();
    
    // Check if there are at least two spaces to ensure correct parsing
    int space1 = data.indexOf(' ');
    int space2 = data.lastIndexOf(' ');

    if (space1 != -1 && space2 != -1 && space1 != space2) {
      // Extract substrings based on space positions
      String sys_str = data.substring(0, space1);
      String dia_str = data.substring(space1 + 1, space2);
      String hr_str = data.substring(space2 + 1);  // Get the rest of the string as HR

      // Convert strings to integers
      sys = sys_str.toInt();
      dia = dia_str.toInt();
      hr = hr_str.toInt();
    }


    // // Update LED state based on 'sys' value
    // if (sys < 110) {
    //   digitalWrite(D5, HIGH); // Turn on the light if sys is less than 110
    // } else {
    //   digitalWrite(D5, LOW); // Otherwise, turn off the light
    // }

    // Update buzzer state based on 'sys' value
    if (sys < 100) {
      Serial.println("Low");

      // Low blood pressure: 2 beeps
      for (int i = 0; i < 2; i++) {
        digitalWrite(D5, HIGH); // Turn on the buzzer
        delay(200); // Delay for beep sound
        digitalWrite(D5, LOW); // Turn off the buzzer
        delay(200); // Delay between beeps
      }
    } else if (sys >= 100 && sys <= 120) {
      Serial.println("Normal");

      // Normal blood pressure: 4 beeps
      for (int i = 0; i < 4; i++) {
        digitalWrite(D5, HIGH); // Turn on the buzzer
        delay(200); // Delay for beep
        digitalWrite(D5, LOW); // Turn off the buzzer
        delay(200); // Delay between beeps
      }
    } else if (sys > 120 && sys <= 139) {
      Serial.println("High");

      for (int i = 0; i < 6; i++) {
        digitalWrite(D5, HIGH); // Turn on the buzzer
        delay(200); // Delay for beep
        digitalWrite(D5, LOW); // Turn off the buzzer
        delay(200); // Delay between beeps
      }
    }else{
      
    // Normal blood pressure
    digitalWrite(D5, LOW); // LED indication
    // No beep for normal BP
  } 
  

    WiFiClient client;
    if (client.connect("api.thingspeak.com", 80)) {
      String bp_data = "field1=" + String(sys) + "&field2=" + String(dia) + "&field3=" + String(hr);
      String request = "GET /update?" + bp_data + "&api_key=" + apiKey + " HTTP/1.1\r\n" +
                       "Host: api.thingspeak.com\r\n" +
                       "Connection: close\r\n\r\n";

      client.print(request);
      delay(500);
      client.stop();

      Serial.println("Data sent to ThingSpeak");
    } else {
      Serial.println("Error connecting to ThingSpeak");
    }
  } else {
    Serial.println("Invalid data length");
  }
}
