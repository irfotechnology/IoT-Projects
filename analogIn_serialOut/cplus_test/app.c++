//#include <String.h>
#include "sha256.h"
#include "Base64.h"

// START: Azure Evet Hub settings
   
const char* KEY = "NMoM2ENQB69bFvpfqkxaZCuShCnzyAEREph/wMk83o0=";    // event hub access key 
const char* KEY_NAME = "RootManageSharedAccessKey";                 // event hub key name  ( policy name) 
const char* HOST = "aliioteventhub.servicebus.windows.net";              // event hub name (name of service bus)
const char* END_POINT = "/device1";                 // name of the evnthub which we create inside eventhub namespace
// END: Azure Evet Hub settings 
// START: WiFi settings
// END: WiFi settings
String request;
String data;
String fullSas;


void main() {
  

  // START: Naive URL Encode
  char* url = HOST + END_POINT;
  url.replace(":", "%3A");
  url.replace("/", "%2F");
  //Serial.println(url);
  // END: Naive URL Encode

  // START: Create SAS
  // https://azure.microsoft.com/en-us/documentation/articles/service-bus-sas-overview/
  // Where to get secods since the epoch: local service, SNTP, RTC
  int expire = 1595360502;
  String stringToSign = url + "\n" + expire;
  cout << stringToSign;
  // START: Create signature
  Sha256.initHmac((const uint8_t*)KEY, 44);
  Sha256.print(stringToSign);
  char* sign = (char*) Sha256.resultHmac();
  

  cout << sign;
  int signLen = 32;
  // END: Create signature

  // START: Get base64 of signature
  int encodedSignLen = base64_enc_len(signLen);
  char encodedSign[encodedSignLen];
  base64_encode(encodedSign, sign, signLen); 
  String encodedSas = (String) encodedSign;
  cout << encodedSas;
  // Naive URL encode
  encodedSas.replace("=", "%3D");
  //Serial.println(encodedSas);
  // END: Get base64 of signature

  // SharedAccessSignature
   fullSas = "sr=" + url + "&sig="+ encodedSas + "&se=" + expire +"&skn=" + KEY_NAME;
  // END: create SAS
//Serial.println("SAS below");
  cout << fullSas;

}