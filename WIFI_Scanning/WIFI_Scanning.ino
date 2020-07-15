// D1 ESP8266 WiFi scanner

#include "ESP8266WiFi.h"

void setup()
{

  Serial.begin(9600);
  // set WiFi to station mode and disconnect from previous connection
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
}

void loop()
{
  Serial.println("Start Scanning ..");
  // scan for nearby WiFi SSIDs & save their number into n
  int n = WiFi.scanNetworks();
  // in case no Network found , print info
  if (n == 0)
  {
    Serial.println("No SSIDs nearby seen...");
  }
  else
  {
    Serial.print(n);
    Serial.println(" WiFi network SSIDs nearby ....");
    // list all WiFi networks nearby
    // name, signal strength and security method
    for (int i = 0; i < n; ++i)
    {
      Serial.print(i + 1);
      Serial.print(":");
      Serial.print(WiFi.SSID(i));
      Serial.print("(");
      Serial.print(WiFi.RSSI(i));
      Serial.print(")");
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? "" : "*");
      delay(10);
    }
  }
  // Exit
  Serial.println("");
  // pause for 15 seconds before new scan
  delay(15000);
}
