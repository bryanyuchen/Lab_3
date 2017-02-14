#include <ESP8266WiFi.h>
WiFiServer server(80); //Initialize the server on Port 80
const short int LED_PIN = 16;//GPIO16
String message = "Default";

void setup() {
WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
WiFi.softAP("Hello_IoT2", "12345678"); // Provide the (SSID, password); .
server.begin(); // Start the HTTP Server
//Looking under the hood
Serial.begin(115200);
IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server
Serial.print("Server IP is: "); // Print the IP to the monitor window
Serial.println(HTTPS_ServerIP);
pinMode(LED_PIN, OUTPUT); //GPIO16 is an OUTPUT pin;
digitalWrite(LED_PIN, LOW); //Initial state is ON
}
void loop() {
WiFiClient client = server.available();
if (!client) {
return;
}
//Looking under the hood
Serial.println("Somebody has connected :)");
//Read what the browser has sent into a String class

//and print the request to the monitor
String request = client.readStringUntil('\r');
//Looking under the hood
Serial.println(request);


// Handle the Request

if (request.indexOf("/Sequence1") != -1){
  message = "Sequence1";
}
else if (request.indexOf("/Sequence2") != -1){
  message = "Sequence2";
}
else if (request.indexOf("/Sequence3) != -1){
  message = "Sequence3";
}
Serial.println(message);
delay(1);
Serial.println("Client disonnected"); //Looking under the hood
}
