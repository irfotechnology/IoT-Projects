#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
// TRACE output simplified, can be deactivated here
#define TRACE(...) Serial.printf(__VA_ARGS__)

// name of the server. You reach it using http://webserver
#define HOSTNAME "smartvalve"

// local time zone definition (Berlin)
#define TIMEZONE "IST-5:30"
bool devicestatus = false;
// need a WebServer for http access on port 80.
ESP8266WebServer server(80);

String p_ssid = "";
String p_key = "";

void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(A0, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  delay(3000);  // wait for serial monitor to start completely.
  // Use Serial port for some trace information from the example
  Serial.begin(115200);
  Serial.setDebugOutput(false);

  // set Wifi mode AP + Station
  WiFi.mode(WIFI_AP_STA);
  setup_softAP();
  setup_httpserver();
  setup_wifi();
}

void setup_softAP() {
  TRACE("Setting soft-AP configuration ... ");
  IPAddress local_IP(192, 168, 4, 4);
  IPAddress gateway(192, 168, 4, 1);
  IPAddress subnet(255, 255, 255, 0);
  TRACE(WiFi.softAPConfig(local_IP, gateway, subnet) ? "Ready" : "Failed!");
  // allow to address the device by the given name e.g. http://webserver
  TRACE("Setting soft-AP ... ");
  char deviceID[20];
  sprintf(deviceID, "SmartVavle-%08X", ESP.getChipId());
  TRACE(WiFi.softAP(deviceID) ? "Ready" : "Failed!");

  TRACE("Soft-AP IP address = 192.168.4.4");

  WiFi.setHostname(HOSTNAME);
}

bool setup_wifi() {
  if (p_ssid.length() > 0 && p_key.length()) {
    Serial.println("Connect to WiFi...\n");
    WiFi.begin(p_ssid, p_key);
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    Serial.println("connected.\n");
    return true;
    // Ask for the current time using NTP request builtin into ESP firmware.
    // TRACE("Setup ntp...\n");
    //configTime(TIMEZONE, "pool.ntp.org");
  }
  return false;
}

// This function is called when the sysInfo service was requested.
void getDeviceInfoHandler() {
  char deviceID[20];
  sprintf(deviceID, "SmartValve-%08X", ESP.getChipId());
  String ptr = "{\"device\":{\"id\":\"";
  ptr += deviceID;
  if (devicestatus) {
    ptr += "\"},\"server\":{\"status\":\"started\",\"localip\":\"192.168.4.4\"";
  }
  else {
    ptr += "\"},\"server\":{\"status\":\"not started\",\"localip\":\"192.168.4.4\"";
  }

  ptr += ",\"gateway\":\"192.168.4.1\"" ;
  ptr += ",\"subnet\":\"255,255,255,0\"}";
  ptr += ",\"ap\":{\"ssid\":\"" + (String) p_ssid + "\"}}";

  server.send(200, "application/json", ptr);
}  // handleSysIn

void setWifiHandler() {
  Serial.println("setWifiHandler : " + server.arg("plain"));
  if (server.hasArg("plain") == false) { //Check if body received
    server.send(400, "text/plain", "Body not received");
  }
  if (setWifi(server.arg("plain"))) {
    server.send(200, "text/plain", "Wifi connection has been set sucessfully.");
  }
  else {
    server.send(200, "text/plain", "Unable to set wifi connection due to unknown error.");
  }
}

void startWifiHandler() {
  Serial.println("startWifiHandler ");
  if (setup_wifi()) {
    server.send(200, "text/plain", "Wifi has been connected sucessfully.");
  }
  else {
    server.send(200, "text/plain", "Unable to connect to Wifi " + p_ssid);
  }
}

bool setWifi(String json) {
  DynamicJsonDocument doc(1024);
  deserializeJson(doc, json);

  const char* __ssid = doc["ssid"];
  const char* __passkey = doc["passkey"];

  if (strlen(__ssid) > 0 && strlen(__passkey)>0) {
    p_ssid = (String)  __ssid;
    p_key = (String) __passkey;
  }
  setup_wifi();
  return true;
}


void getValvedataHandler() {
  //Serial.println("handle_OnConnect");
  int i = analogRead(A0);
  float out =0;
  if(i>0){
     i=i-137;
     out = ((float)i/82.4) * 10;
  }
  String ptr = "{\"value\":\"";
  ptr += (int) i;
  ptr += "\"}";
  server.send(200, "application/json", ptr); 
}


void setup_httpserver() {
  Serial.println("Starting http server...");
  server.on("/", HTTP_GET, getDeviceInfoHandler);
  server.on("/getValveData", HTTP_GET, getValvedataHandler);
  server.on("/setWifi", HTTP_POST, setWifiHandler);
  server.on("/startWifi", HTTP_POST, startWifiHandler);
  
  // enable CORS header in webserver results
  server.enableCORS(true);

  // enable ETAG header in webserver results from serveStatic handler
  server.enableETag(true);

  // handle cases when file is not found
  server.onNotFound([]() {
    // standard not found in browser.
    server.send(404, "text/plain", "Not found");
  });

  server.begin();
  devicestatus = true;
  Serial.println("http server started. ");
  TRACE("hostname=%s\n", WiFi.getHostname());
}

void send_httpRequest() {
  if (WiFi.status() == WL_CONNECTED)
  {
    Serial.println("Connected to WIFI...");

    WiFiClient client;
    HTTPClient http; //Object of class HTTPClient
    http.begin(client, "http://jsonplaceholder.typicode.com/posts/24");
    int httpCode = http.GET();

    if (httpCode > 0)
    {
      // Get the request response payload
      String payload = http.getString();
      Serial.println(payload);

      // Parsing
      DynamicJsonDocument doc(1024);
      deserializeJson(doc, payload);

      int id = doc["id"];
      const char* title = doc["title"];
      const char* username = doc["userId"];
      //const char* email = doc["body"];

      Serial.print("Id:");
      Serial.println(id);
      Serial.print("Username:");
      Serial.println(username);
      Serial.print("title:");
      Serial.println(title);
    }
    http.end(); //Close connection
  }
}

void loop() {
  // put your main code here, to run repeatedly after 10 sec:
  digitalWrite(LED_BUILTIN, LOW);
  delay(10000);
  digitalWrite(LED_BUILTIN, HIGH);
  server.handleClient();
  send_httpRequest();
}
