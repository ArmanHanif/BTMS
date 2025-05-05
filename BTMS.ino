#include <OneWire.h>

#include <DallasTemperature.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Blynk Configuration
#define BLYNK_TEMPLATE_ID "TMPL3QDQohSjZ"
#define BLYNK_TEMPLATE_NAME "BTMS"
#define BLYNK_AUTH_TOKEN "RndOcf_d93zbslcxbIsGq8zcep-prdpI"

// Blynk defines to resolve the template error
#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

// WiFi Credentials
const char* ssid = "Arman";
const char* password = "arman12345";

// DS18B20 configuration
const int oneWireBus = 4; // GPIO where DS18B20 is connected
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

// MQ-135 configuration
#define ANALOG_PIN 34  // Analog pin connected to AO
#define DIGITAL_PIN 14 // Digital pin connected to DO

// Relay configuration
#define RELAY_PIN 12   // GPIO to control the relay (to control motor)
#define BUZZER_PIN 13  // Digital pin connected to the buzzer (GPIO13 in this case)

// I2C LCD (Address: Update this to the correct address after scanning)
LiquidCrystal_I2C lcd(0x27, 16, 2); // Update with the correct address

// Threshold values
const float temperatureThreshold = 80.0;  // Temperature threshold in °C
const float smokeThreshold = 600;         // Smoke threshold in PPM

// State variables to track the threshold status
bool temperatureAlarmActive = false;
bool smokeAlarmActive = false;

void setup() {
  // Start serial communication
  Serial.begin(115200);

  // Initialize DS18B20
  sensors.begin();

  // MQ-135 pin setup
  pinMode(DIGITAL_PIN, INPUT);

  // Initialize I2C (SDA on GPIO19, SCL on GPIO23)
  Wire.begin(19, 23); // SDA on GPIO19, SCL on GPIO23

  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("System Initializing");
  delay(2000);
  lcd.clear();

  // Initialize Buzzer (Active Buzzer)
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(BUZZER_PIN, LOW);  // Ensure the buzzer is off initially

  // Initialize Relay (set initial state to LOW, i.e., relay off)
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, LOW); // Motor OFF initially

  // Connect to WiFi and Blynk
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    lcd.setCursor(0, 1);
    lcd.print("Connecting WiFi");
  }
  Serial.println("\nWiFi Connected");

  // Update LCD with WiFi status
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("WiFi Connected");

  // Initialize Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);
}

void loop() {
  // Run Blynk
  Blynk.run();

  // DS18B20 Reading
  sensors.requestTemperatures();
  float temperatureC = sensors.getTempCByIndex(0);
  if (temperatureC == -127) {
    Serial.println("Temperature sensor disconnected!");
    temperatureC = 0;
  }
  Serial.print("Temperature: ");
  Serial.print(temperatureC);
  Serial.println("°C");

  // MQ-135 Reading
  int analogValue = analogRead(ANALOG_PIN);
  float ppm = analogValue  / 4.095;  // Approximate conversion to PPM
  int digitalState = digitalRead(DIGITAL_PIN);

  // Send data to Blynk
  Blynk.virtualWrite(V0, temperatureC);
  Blynk.virtualWrite(V1, ppm);

  // Check if temperature threshold is exceeded
  if (temperatureC >= temperatureThreshold) {
    digitalWrite(RELAY_PIN, HIGH);  // Motor ON
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer ON
    temperatureAlarmActive = true;
    Serial.println("Temperature Alarm: Relay ON, Buzzer Activated");
    blinkWarning("Temp High!"); // Blinking message for high temp
  }
  // Check if smoke threshold is exceeded
  else if (ppm >= smokeThreshold) {
    digitalWrite(RELAY_PIN, HIGH);  // Motor ON
    digitalWrite(BUZZER_PIN, HIGH); // Buzzer ON
    smokeAlarmActive = true;
    Serial.println("Smoke Alarm: Relay ON, Buzzer Activated");
    blinkWarning("Smoke High!"); // Blinking message for high smoke
  }
  // If no thresholds are exceeded
  else {
    digitalWrite(RELAY_PIN, LOW);   // Motor OFF
    digitalWrite(BUZZER_PIN, LOW);  // Buzzer OFF

    // Reset alarm states if they were previously active
    if (temperatureAlarmActive || smokeAlarmActive) {
      Serial.println("Alarm Conditions Cleared");
      temperatureAlarmActive = false;
      smokeAlarmActive = false;
    }

    // Reset LCD display
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Temp: ");
    lcd.print(temperatureC, 1); // Display temperature with 1 decimal place
    lcd.print(" C");

    lcd.setCursor(0, 1);
    lcd.print("PPM: ");
    lcd.print(ppm, 1); // Display PPM with 1 decimal place
  }

  delay(1000); // Wait for a second before repeating
}

// Function to blink warning on LCD
void blinkWarning(String message) {
  unsigned long currentMillis = millis();
  static unsigned long previousMillis = 0;
  const long interval = 500;  // Interval for blinking (500 ms)

  // Toggle LCD display
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
    lcd.setCursor(0, 1);
    lcd.print("WARNING!");
  } else {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print(message);
  }
}
