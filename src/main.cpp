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

  bool digital_channel_1;
  bool digital_channel_2;
};

Payload payload;

RF24 radio(7, 8);

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting setup");
  radio.begin();
  Serial.println("Started radio");
  radio.setDataRate(RF24_1MBPS);
  Serial.println("Set data rate");
  radio.setPALevel(RF24_PA_MAX);
  Serial.println("Set PA level");
  radio.openReadingPipe(1, pipe);
  Serial.println("Opened reading pipe");
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

    Serial.println();
  }
}