

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "Honor 10 Lite";
const char* password = "9221992655";

void setup() 
{
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.println("Connecting to WIFI...");
  }
}

void loop() 
{
  if (WiFi.status() == WL_CONNECTED) 
  {
    Serial.println("Connected to WIFI...");
        
    HTTPClient http; //Object of class HTTPClient
    http.begin("http://jsonplaceholder.typicode.com/posts/24");
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
  delay(60000);
}
