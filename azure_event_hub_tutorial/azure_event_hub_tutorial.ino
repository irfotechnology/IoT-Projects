#include <ESP8266WiFi.h>
#include <WiFiClientSecure.h>
#include <String.h>
#include "sha256.h"
#include "Base64.h"

// START: Azure Evet Hub settings
   
const char* KEY = "NMoM2ENQB69bFvpfqkxaZCuShCnzyAEREph/wMk83o0=";    // event hub access key 
const char* KEY_NAME = "RootManageSharedAccessKey";                 // event hub key name  ( policy name) 
const char* HOST = "aliioteventhub.servicebus.windows.net";              // event hub name (name of service bus)
const char* END_POINT = "/device1";                 // name of the evnthub which we create inside eventhub namespace
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
  String url = (String)HOST + (String)END_POINT;
  url.replace(":", "%3A");
  url.replace("/", "%2F");
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
  int expire = 1595540340;
  String stringToSign = url + "\n" + expire;
  //Serial.println(stringToSign);
  // START: Create signature
  Sha256.initHmac((const uint8_t*)KEY, 44);
  Sha256.print(stringToSign);
  char* sign = (char*) Sha256.resultHmac();
  

  //Serial.println(sign);
  int signLen = 32;
  // END: Create signature

  // START: Get base64 of signature
  int encodedSignLen = base64_enc_len(signLen);
  char encodedSign[encodedSignLen];
  base64_encode(encodedSign, sign, signLen); 
  String encodedSas = (String) encodedSign;
  //Serial.println(encodedSas);
  // Naive URL encode
  encodedSas.replace("=", "%3D");
  //Serial.println(encodedSas);
  // END: Get base64 of signature

  // SharedAccessSignature
   fullSas = "sr=" + url + "&sig="+ encodedSas + "&se=" + expire +"&skn=" + KEY_NAME;
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
  data = "{'deviceid':'esp8266'";
   data+= ", 'temperature' : ";
   data+= temp;
   data+=" , 'soilmoisture' : ";
   data+=soilm;
   data+=" , 'lightintensity' :";
   data+=light;
   data+="}"; 
   request = String("POST ") + "https://" + HOST + END_POINT + "/messages HTTP/1.1\r\n" +
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

  //Serial.println();
  //Serial.print("Response code: ");
  //Serial.println(response.substring(9, 12));

  delay(1000);

}
