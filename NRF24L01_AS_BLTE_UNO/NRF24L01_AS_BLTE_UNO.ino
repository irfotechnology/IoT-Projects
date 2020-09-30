
//https://www.instructables.com/id/Send-Data-to-Mobile-Using-NRF24L01-Using-NRF-As-a-/
#include "SPI.h"
#include "RF24.h"
#include "BTLE.h"

RF24 radio(9, 10);
BTLE btle(&radio);

void setup() {
  Serial.begin(9600);
  while (!Serial) { }
  Serial.println("BTLE advertisement sender");
  btle.begin("MyDevice");
}

void loop() {
  float temp = 26;
  nrf_service_data buf;
  //buf.service_uuid = NRF_DEVICE_INFORMATION_SERVICE_UUID; //0x180A
  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID; //0x1809
  //buf.service_uuid =NRF_BATTERY_SERVICE_UUID; //0x180F
  buf.value = BTLE::to_nRF_Float(temp);
  btle.advertise(0x16, &buf, sizeof(buf));
  //btle.advertise(0,0);
  btle.hopChannel();
  Serial.print(".");
}
