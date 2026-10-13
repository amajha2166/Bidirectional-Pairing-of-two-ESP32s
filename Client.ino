#include <WiFi.h> 1
10
#include <HTTPClient.h>
const char* ssid = "ESP32_Server";
const char* password = "12345678";
#define TRIG 5
#define ECHO 18
void setup() {
Serial.begin(115200);
pinMode(TRIG, OUTPUT);
pinMode(ECHO, INPUT);
WiFi.begin(ssid, password);
Serial.print("Connecting to server AP");
while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("\nConnected to server!");
}
float getDistance() {
digitalWrite(TRIG, LOW);
delayMicroseconds(2);
digitalWrite(TRIG, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG, LOW);
float duration = pulseIn(ECHO, HIGH);
float distance = duration * 0.034 / 2;
return distance;
}
void loop() {
if (WiFi.status() == WL_CONNECTED) {
float distance = getDistance();
Serial.print("Sending Distance: ");
Serial.println(distance);
HTTPClient http;
http.begin("http://192.168.4.1/distance");
http.addHeader("Content-Type", "application/x-www-form-urlencoded");
String postData = "distance=" + String(distance, 2);
int code = http.POST(postData);
Serial.print("POST Code: ");
Serial.println(code);
http.end();
}
delay(1000);
}