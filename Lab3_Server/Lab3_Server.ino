

#include <ESP8266WiFi.h>

WiFiServer server(80); //Initialize the server on Port 80
const short int LED_PIN = 16;//GPIO16
String message = "Default";
String prevmsg = "Default";
const char* ssid_client_1     = "Hello_IoT2";
const char* password_client_1 = "12345678";
const char* ssid_client_2     = "Hello_IoT3";
const char* password_client_2 = "12345678";
const char* host = "192.168.4.1";
bool requestflag = false;

void setup() {
  Serial.begin(115200);
  delay(10);
  }

void blinkLED (int ntimes) {
  for (int a = 0; a < ntimes; a++) {
  digitalWrite(LED_PIN, HIGH); 
  delay(100);
  digitalWrite(LED_PIN, LOW); 
  delay(100);
  }
  digitalWrite(LED_PIN, HIGH);
}
void configureServer() {
  // Configure web server
  WiFi.mode(WIFI_AP); //Our ESP8266-12E is an AccessPoint
  WiFi.softAP("Hello_IoT1", "12345678"); // Provide the (SSID, password); .
  server.begin(); // Start the HTTP Server
  //Looking under the hood
  IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server
  Serial.print("Server IP is: "); // Print the IP to the monitor window
  Serial.println(HTTPS_ServerIP);
  pinMode(LED_PIN, OUTPUT); //GPIO16 is an OUTPUT pin;
}

void startServer(){
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

  // Prepare the HTML document to respond and add buttons:
  String s = "HTTP/1.1 200 OK\r\n";
  s += "Content-Type: text/html\r\n\r\n";
  s += "<!DOCTYPE HTML>\r\n<html>\r\n";
  s += "<br><input type=\"button\" name=\"b1\" value=\"Play Sequence 1\"";
  s += " onclick=\"location.href='/Sequence1'\">";
  s += "<br><br><br>";
  s += "<br><input type=\"button\" name=\"b1\" value=\"Play Sequence 2\"";
  s += " onclick=\"location.href='/Sequence2'\">";
  s += "<br><br><br>";
  s += "<br><input type=\"button\" name=\"b1\" value=\"Play Sequence 3\"";
  s += " onclick=\"location.href='/Sequence3'\">";
  s += "</html>\n";

if (request.indexOf("/Sequence1") != -1){
  message = "Sequence1";
  blinkLED(1);
}
else if (request.indexOf("/Sequence2") != -1){
  message = "Sequence2";
  blinkLED(2);
}
else if (request.indexOf("/Sequence3") != -1){
  message = "Sequence3";
  blinkLED(3);
}
if (message != prevmsg) {
    requestflag = true;
  }
prevmsg = message;
Serial.print("Playing ");
Serial.println(message);
  
//Serve the HTML document to the browser.
client.flush(); //clear previous info in the stream
client.print(s); // Send the response to the client

delay(1);
Serial.println("Client disonnected"); //Looking under the hood
}

void startClient(char* ssid, char* pass){
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    return;
  }
  
  // This will send the request to the server
  client.print(message);
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");
  requestflag = false;
}

void loop() {
  configureServer();
  while (requestflag == false) {
  startServer();
  delay(1000);
  }
  delay(300);
  Serial.println("Connecting to Client");
  //startClient(ssid_client_1, password_client_1);
  //startClient(ssid_client_2, password_client_2);
  requestflag = false;
}

