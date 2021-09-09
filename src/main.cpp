#include <Arduino.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

struct Payload
{
  float yaw;
  float pitch;
  float roll;

  short aquart_x;
  short aquart_y;
  short aquart_z;

  long pressure;
  int altitude;
  float temp;

  unsigned int vcc;
  unsigned int packet_id;
};

Payload payload;

RF24 radio(7, 8);

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup()
{
  Serial.begin(115200);
  radio.begin();
  radio.setDataRate(RF24_1MBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, pipe);
}

void loop()
{
  radio.startListening();
  if (radio.available())
  {
    radio.read(&payload, sizeof(Payload));

    Serial.print(payload.yaw);
    Serial.print(",");
    Serial.print(payload.pitch);
    Serial.print(",");
    Serial.print(payload.roll);
    Serial.print(",");
    Serial.print(payload.aquart_x);
    Serial.print(",");
    Serial.print(payload.aquart_y);
    Serial.print(",");
    Serial.print(payload.aquart_z);
    Serial.print(",");
    Serial.print(payload.pressure);
    Serial.print(",");
    Serial.print(payload.altitude);
    Serial.print(",");
    Serial.print(payload.temp);
    Serial.print(",");
    Serial.print(payload.vcc);
    Serial.print(",");
    Serial.print(payload.packet_id);

    Serial.println();
  }
}