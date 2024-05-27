#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Servo.h>

// Inisialisasi objek servo
Servo myservo;

/* Put your SSID & Password */
const char* ssid = "NodeMCU";       // Enter SSID here
const char* password = "12345678";  //Enter Password here

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

uint8_t GATEpin = D8;
bool GATEstatus = true;

void setup() {
  Serial.begin(115200);
  myservo.attach(GATEpin);

  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);

  server.on("/", handle_OnConnect);
  server.on("/gateopen", handle_gateOpen);
  server.on("/gateclose", handle_gateClose);
  server.onNotFound(handle_NotFound);

  server.begin();
  Serial.println("HTTP server started");
}
void loop() {
  server.handleClient();

  if (GATEstatus) {
    myservo.write(0);
  } else {
    myservo.write(180);
  }
}

void handle_OnConnect() {
  GATEstatus = false;
  server.send(200, "text/html", SendHTML(GATEstatus));
}
void handle_gateOpen() {
  // Mengatur posisi awal servo ke 0 derajat
  GATEstatus = true;
  server.send(200, "text/html", SendHTML(true));
}

void handle_gateClose() {
  // Mengatur posisi awal servo ke 180 derajat
  GATEstatus = false;
  server.send(200, "text/html", SendHTML(false));
}

void handle_NotFound() {
  server.send(404, "text/plain", "Not found");
}

String SendHTML(uint8_t gatestat) {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>LED Control</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += ".button {display: block;width: 80px;background-color: #1abc9c;border: none;color: white;padding: 13px 30px;text-decoration: none;font-size: 25px;margin: 0px auto 35px;cursor: pointer;border-radius: 4px;}\n";
  ptr += ".button-on {background-color: #1abc9c;}\n";
  ptr += ".button-on:active {background-color: #16a085;}\n";
  ptr += ".button-off {background-color: #34495e;}\n";
  ptr += ".button-off:active {background-color: #2c3e50;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h1>ESP8266 Web Server</h1>\n";
  ptr += "<h3>Using Access Point(AP) Mode</h3>\n";

  if (gatestat) {
    ptr += "<p>Gate Status: Open</p><a class=\"button button-off\" href=\"/gateclose\">Close</a>\n";
  } else {
    ptr += "<p>Gate Status: Close</p><a class=\"button button-on\" href=\"/gateopen\">Open</a>\n";
  }

  ptr += "</body>\n";
  ptr += "</html>\n";
  return ptr;
}