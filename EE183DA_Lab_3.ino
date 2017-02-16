#include <ESP8266WiFi.h>
#include <Servo.h>  

WiFiServer server(80); //Initialize the server on Port 80
const short int LED_PIN = 16;//GPIO16
String message = "Default";
String prevmsg = "Default";
const char* ssid     = "TeamShaggy1";
const char* password = "12345678";
const char* host = "192.168.4.1";
int nrequests = 0;
bool setupflag = false;
bool requestflag = false;

//------------------------------------Client----------------------------------------//
void clientSetup() {
  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  /* Explicitly set the ESP8266 to be a WiFi-client, otherwise, it by default,
     would try to act as both a client and an access-point and could cause
     network-issues with your other WiFi-devices on your WiFi-network. */
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void connServer(){
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
  client.print(String("GET ") + "/SEQ HTTP/1.1");
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
    message = client.readStringUntil('\r');
    Serial.print(message);
  }
  
  Serial.println();
  Serial.println("closing connection");
}
void runClient() {
  clientSetup();
  connServer();
  delay(500);
}
//------------------------------------Client----------------------------------------//

//------------------------------------Server----------------------------------------//
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
  WiFi.softAP("TeamShaggy1", "12345678"); // Provide the (SSID, password); .
  server.begin(); // Start the HTTP Server
  //Looking under the hood
  IPAddress HTTPS_ServerIP= WiFi.softAPIP(); // Obtain the IP of the Server
  Serial.print("Server IP is: "); // Print the IP to the monitor window
  Serial.println(HTTPS_ServerIP);
  pinMode(LED_PIN, OUTPUT); //GPIO16 is an OUTPUT pin;
  setupflag = true;
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
else if (request.indexOf("/SEQ") != -1 && requestflag) {
  client.flush();
  client.print(message);
  delay(1);
  Serial.println("Client Disconnected");
  nrequests++;
  return;
}
prevmsg = message;
if (nrequests == 2) {
  requestflag = false;
}
Serial.print("Playing ");
Serial.println(message);
  
//Serve the HTML document to the browser.
client.flush(); //clear previous info in the stream
client.print(s); // Send the response to the client

delay(1);
Serial.println("Client disconnected"); //Looking under the hood
}

void startClient(const char* ssid, const char* pass){
  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin("Not Your Network", "12345678");
  
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

void runServer() {
  if (!setupflag) {
  configureServer();
  }
  startServer();
}
//------------------------------------Server----------------------------------------//
/* CLIENT CONFIG USAGE EXAMPLE
void setup() {
  Serial.begin(115200);
  clientSetup();
  delay(10);
}
void loop() {

  connServer();
  delay(5000);
}
*/

/* SERVER CONFIG USAGE EXAMPLE
void setup() {
    Serial.begin(115200);
  delay(10);
}
void loop() {
  if (!setupflag) {
  configureServer();
  }
  startServer();
}*/

int rhythm_1[5] = {1000, 1000, 1000, 500, 500};
int rhythm_2[6] = {500, 500, 1000, 500, 500, 1000};
int rhythm_3[4] = {1000, 1000, 1000, 1000};

// Declare Servos
Servo servo_one;

//function to get the servo to play a single beat with some delay
void beat(int del) {
  servo_one.write(135);
  digitalWrite(D2, HIGH);
  delay(100);
  servo_one.write(180);
  digitalWrite(D2, LOW);
  delay(del); 
}

//function to play a preset rhythm
void playBeat(int rhythm[]) {
  for (int i = 0; i < sizeof(rhythm); i++) {
    beat(rhythm[i]);
    Serial.println(rhythm[i]/8000);   
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  servo_one.attach(D1);
  servo_one.write(180);
  pinMode(D2, OUTPUT);
  pinMode(D0, INPUT);
}

void loop() {
  int val = digitalRead(D0);
  if (val == 0) {
    
  }
  else {
    beat(50);
    beat(50);
  }
}
