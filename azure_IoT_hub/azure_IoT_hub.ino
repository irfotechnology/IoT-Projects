#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <String.h>
#include "sha256.h"
#include "Base64.h"

// START: Azure Evet Hub settings
   
const char* KEY = "zGjqPVcew4bgRXPIKXrHQz7h5txKOwWf6HFpvcHlfYo=";    // Device key, not hub key
const char* HOST = "aliiothub.azure-devices.net";              // iot hub name (name of service bus)
const char* END_POINT = "/devices/ESPEdge/messages/events?api-version=2018-06-30";  
// END: Azure Evet Hub settings 
// START: WiFi settings
const char* SSID = "Honor 10 Lite";
const char* PASSWORD = "9221992655";
// END: WiFi settings
String request;
String data;
String fullSas;
WiFiClientSecure client;


void setup() {
  Serial.begin(115200);

  // START: Naive URL Encode
  String url = "AliIoTHub.azure-devices.net%2fdevices%2fESPEdge"; 
  // + (String)END_POINT;
  url.replace(":", "%3a");
  url.replace("/", "%2f");
  //Serial.println(url);
  // END: Naive URL Encode

  // START: Create SAS
  // https://azure.microsoft.com/en-us/documentation/articles/service-bus-sas-overview/
  //https://docs.microsoft.com/en-us/rest/api/eventhub/send-event
  // Where to get secods since the epoch: local service, SNTP, RTC


  //Always remember to get Unix Time for one hour later time https://www.unixtimestamp.com/index.php
  // Best practice is to get the current time stamp and add 300 like Token expires now+300
  //Either use Time server call or use clock hardware module or get time from the host pc
  //for more details: https://github.com/gloveboxes/Arduino-ESP8266-Secure-Http-Azure-IoT-Hub-Client-V2/blob/master/AzureClient/AzureClient.ino
  int expire = 1627139763;
  String stringToSign = url + "\n" + expire;
  Serial.println(stringToSign);

  char *key = (char *)KEY; //.c_str();
  int keyLength = strlen(key);
  //Need to convert from Base 64 String
    int decodedKeyLength = base64_dec_len(key, keyLength);
    char decodedKey[decodedKeyLength];

    base64_decode(decodedKey, key, keyLength);
  
  // START: Create signature
  Sha256.initHmac((const uint8_t*)decodedKey, (size_t)decodedKeyLength);
  Sha256.print(stringToSign);
  char* sign = (char*) Sha256.resultHmac();
  

  Serial.println(sign);
  int signLen = 32;
  // END: Create signature

  // START: Get base64 of signature
  //Convert To base 64 string
  int encodedSignLen = base64_enc_len(signLen);
  char encodedSign[encodedSignLen];
  base64_encode(encodedSign, sign, signLen); 
  String encodedSas = (String) encodedSign;
  Serial.println(encodedSas);
  // Naive URL encode
  encodedSas.replace("=", "%3d");
  Serial.println(encodedSas);
  // END: Get base64 of signature

  // SharedAccessSignature
   fullSas = "sr=" + url + "&sig=" + encodedSas + "&se=" + expire;
  // END: create SAS
//Serial.println("SAS below");
  Serial.println(fullSas);
//Serial.println();
  // START: Wifi connection
  Serial.print("connecting to ");
  Serial.println(SSID);
  
  WiFi.begin(SSID, PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  // END: Wifi connection

}

void loop() {
  //delay(100);
   WiFiClientSecure client;
   client.setInsecure();
  if (!client.connect(HOST, 443)) {
    Serial.println("connection failed");
    return;
  }
  int r=random(100,10000);
  int temp=random(25 , 45);
  int light= random(10 , 100);
  int soilm = random (10 , 70 );
  data = "{'device_internal_id':'esp8266_d1'";
   data+= ", 'temperature' : ";
   data+= temp;
   data+=" , 'soilmoisture' : ";
   data+=soilm;
   data+=" , 'lightintensity' :";
   data+=light;
   data+="}"; 
   //request = String("POST ") + "https://" + HOST + END_POINT + "/messages HTTP/1.1\r\n" +
   request = String("POST ") + "https://" + HOST + END_POINT + " HTTP/1.1\r\n" +
               "Host: " + HOST + "\r\n" +
               "Authorization: SharedAccessSignature " + fullSas + "\r\n" +                
               "Content-Type: application/x-www-form-urlencoded\r\n" +  
               "Content-Length: " + data.length() + "\r\n\r\n" +
               data;
    Serial.println(request);
  client.print(request);
 

  //String response = "";
  //while (client.connected()) {
    //response += client.readStringUntil('\n');
  //}
  //Serial.print("Response: ");
  //Serial.println(response);
  //Serial.print("Response code: ");
  //Serial.println(response.substring(9, 12));

  delay(1000);

}
