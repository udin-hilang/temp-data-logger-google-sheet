/*
 * Project: IoT Data Logger to Google Sheets (ESP32)
 * Description: Connects to Wi-Fi, reads timestamped temperature data from a DS1302 RTC 
 * and a DS18B20 sensor, and sends the logs to a Google Sheet via a Google Apps Script Web App.
 */

#include <HTTPClient.h>
#include <WiFi.h>
#include <RtcDS1302.h>
#include <ThreeWire.h>
#include <DallasTemperature.h>
#include <OneWire.h>

// --- DS18B20 Temperature Sensor Setup ---
#define sensorPin 5
OneWire oneWire(sensorPin);
DallasTemperature sensor(&oneWire);

// --- DS1302 RTC Setup (IO, CLK, RST) ---
ThreeWire myWire(26, 25, 27);
RtcDS1302<ThreeWire> rtc(myWire);

// --- Wi-Fi Credentials ---
const char* ssid = "realme 12+ 5G";
const char* pw = "";

// Initialize HTTP client instance
HTTPClient http;

void setup() {
  Serial.begin(115200);
  
  // Initialize hardware peripherals
  pinMode(sensorPin, INPUT_PULLUP);
  rtc.Begin();
  sensor.begin();
  
  // Establish connection to the Wi-Fi network
  WiFi.begin(ssid, pw);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.printf("\nWIFI CONNECTED\n"); 
}

void loop() {
  // 1. Get current time from RTC module
  RtcDateTime now = rtc.GetDateTime();
  
  // Format the date/time string with URL-safe encodings:
  // %2F = '/' , + = ' ' (space) , %3A = ':'
  String waktu = String(now.Day()) + "%2F" + String(now.Month()) + "%2F" + String(now.Year()) + "+" + String(now.Hour()) + "%3A" + String(now.Minute()) + "%3A" + String(now.Second());
  
  // 2. Read temperature sensor value
  sensor.requestTemperatures();
  float temp = sensor.getTempCByIndex(0);
  
  // 3. Construct URL endpoint for the Google Apps Script Web App API
  String url = "https://script.google.com/macros/s/AKfycbw9qY0tEAtFemUyu93KNwsO9L-B94qugURo7Wn6WXko_osdJwZJoExr1HrFCwjRKYw9Kg/exec";
  url += "?waktu=" + waktu;
  url += "&value=" + String(temp);
  
  // 4. Initialize HTTP request and transmit data
  http.begin(url);
  Serial.println(waktu + " " + String(temp));
  
  // Send HTTP GET request and catch response status code (e.g., 200 or 302)
  int code = http.GET();
  Serial.println(code);
  
  // Close connection
  http.end();
  
  // Wait 5 seconds before logging the next sample
  delay(5000);
}
