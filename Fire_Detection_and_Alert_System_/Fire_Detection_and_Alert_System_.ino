/*
 * 🔥 Fire Detection and Alert System (Base Code)
 *
 * ⚠️ Disclaimer / Notes:
 * This code was generated with the help of AI (ChatGPT) and is intended
 * to serve as a **base / reference implementation** for educational purposes.
 * 
 * - Users are encouraged to modify and adapt this code according to their 
 *   project requirements, hardware configurations, and safety needs.
 * - Threshold values (smoke, temperature, flame detection) should be tested 
 *   and calibrated carefully in real-world conditions.
 * - This is NOT production-ready fire safety software. It is only a 
 *   learning resource for academic projects (Semester IV).
 *
 * 📚 Project: Fire Detection and Alert System
 * 🎓 Institute: Don Bosco Institute of Technology, Mumbai
 * 📅 Academic Year: 2023-24
 */

#include <DHT.h>
#include <SoftwareSerial.h>

// Sensor Pins
#define FLAME_SENSOR_1 2
#define FLAME_SENSOR_2 3
#define FLAME_SENSOR_3 4
#define FLAME_SENSOR_4 5
#define MQ2_SENSOR A0
#define DHTPIN 6
#define DHTTYPE DHT11

// GSM
#define GSM_RX 7
#define GSM_TX 8
SoftwareSerial gsmSerial(GSM_RX, GSM_TX);

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  gsmSerial.begin(9600);
  dht.begin();

  pinMode(FLAME_SENSOR_1, INPUT);
  pinMode(FLAME_SENSOR_2, INPUT);
  pinMode(FLAME_SENSOR_3, INPUT);
  pinMode(FLAME_SENSOR_4, INPUT);
}

void sendSMS(String message) {
  gsmSerial.println("AT+CMGF=1");
  delay(1000);
  gsmSerial.println("AT+CMGS=\"+91XXXXXXXXXX\""); // Enter the phone number
  delay(1000);
  gsmSerial.print(message);
  delay(1000);
  gsmSerial.write(26);  // Ctrl+Z to send SMS
  delay(1000);
}

void loop() {
  // Read flame sensor values
  int flame1 = digitalRead(FLAME_SENSOR_1);
  int flame2 = digitalRead(FLAME_SENSOR_2);
  int flame3 = digitalRead(FLAME_SENSOR_3);
  int flame4 = digitalRead(FLAME_SENSOR_4);

  // Read smoke sensor (analog)
  int smokeLevel = analogRead(MQ2_SENSOR);

  // Read temperature
  float temp = dht.readTemperature();

  bool fireDetected = false;
  String alertMsg = "";

  if (flame1 == LOW || flame2 == LOW || flame3 == LOW || flame4 == LOW) {
    fireDetected = true;
    alertMsg += "Flame detected! ";
  }

  if (smokeLevel > 300) {   // Threshold to be adjusted
    fireDetected = true;
    alertMsg += "High smoke level! ";
  }

  if (temp > 50.0) {        // Temperature threshold
    fireDetected = true;
    alertMsg += "High temperature: " + String(temp) + " C ";
  }

  if (fireDetected) {
    sendSMS("ALERT! Fire detected: " + alertMsg);
    delay(60000); // Wait 1 min before next alert
  }

  delay(1000);
}
