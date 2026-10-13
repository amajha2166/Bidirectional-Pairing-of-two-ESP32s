#include <WiFi.h>
#include <WebServer.h>
const char* ssid = "ESP32_Server";
const char* password = "12345678";
#define LED1 2
#define LED2 4
#define LED3 5
#define LED4 19
#define BUZZER 23
WebServer server(80);
float lastDistance = 0;
void updateOutputs(float distance) {
digitalWrite(LED1, LOW);
digitalWrite(LED2, LOW);
digitalWrite(LED3, LOW);
digitalWrite(LED4, LOW);
digitalWrite(BUZZER, LOW);
if (distance > 30) digitalWrite(LED1, HIGH);
else if (distance > 20) digitalWrite(LED2, HIGH);
else if (distance > 10) digitalWrite(LED3, HIGH);
else if (distance > 5) digitalWrite(LED4, HIGH);
else {
digitalWrite(LED4, HIGH);
digitalWrite(BUZZER, HIGH);
}
}
void handleRoot() {
String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>ESP32 Distance Monitor</title>
<style>
body {
font-family: 'Poppins', sans-serif;
background: #0b132b;
color: #fff;
text-align: center;
padding-top: 50px;
}
h1 { color: #00d1ff; }
#distanceBox {
font-size: 2.5em;
margin-top: 30px;
padding: 20px;
background: #1c2541;
display: inline-block;
border-radius: 15px;
box-shadow: 0 0 20px #00d1ff55;
}
.status {
margin-top: 40px;
font-size: 1.2em;
color: #08f7fe;
}
</style> 19
</head>
<body>
<h1>ESP32 Distance Monitor</h1>
<div id="distanceBox">--</div>
<div class="status" id="status">Waiting for data...</div>
<script>
async function updateDistance() {
const res = await fetch('/distance');
const data = await res.json();
document.getElementById('distanceBox').innerHTML = data.distance.toFixed(2) + " cm";
}
setInterval(updateDistance, 1000);
updateDistance();
</script>
</body>
</html>
)rawliteral";
server.send(200, "text/html", html);
}
void handleDistancePost() {
if (server.hasArg("distance")) {
lastDistance = server.arg("distance").toFloat();
updateOutputs(lastDistance);
server.send(200, "text/plain", "OK");
} else {
server.send(400, "text/plain", "Missing distance");
}
}
void handleDistanceGet() {
String json = "{\"distance\":" + String(lastDistance, 2) + "}";
server.send(200, "application/json", json);
}
void setup() {
Serial.begin(115200);
pinMode(LED1, OUTPUT);
pinMode(LED2, OUTPUT);
pinMode(LED3, OUTPUT);
pinMode(LED4, OUTPUT);
pinMode(BUZZER, OUTPUT);
WiFi.softAP(ssid, password);
Serial.print("AP IP: ");
Serial.println(WiFi.softAPIP());
server.on("/", handleRoot);
server.on("/distance", HTTP_GET, handleDistanceGet);
server.on("/distance", HTTP_POST, handleDistancePost);
server.begin();
}
void loop() {
server.handleClient();
}er