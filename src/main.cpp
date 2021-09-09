#include <Arduino.h>

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

struct Payload // MAX 32 BYTES
{
  // 12 bytes of MPU data
  int16_t ax, ay, az;  // acceleration values in all 3 axis
  int16_t gx, gy, gz;  // gyroscope acceleration values in all 3 axis


  long pressure; // 4 bytes
  int altitude;  // 2 bytes
  float temp;  // 4 bytes

  unsigned int vcc;  // 2 bytes
  unsigned int packet_id;  // 2 bytes

  // we have 26 bytes total
};

Payload payload;

RF24 radio(7, 8);

const uint64_t pipe = 0xE8E8F0F0E1LL;

void setup()
{
  Serial.begin(115200);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.openReadingPipe(1, pipe);
}

unsigned int last_packet_id;
long last_recv_id;
long recv_id;
long timedelta;
long last_start_time = micros();
int loss_percent;

void loop()
{
  radio.startListening();
  if (radio.available())
  {
    radio.read(&payload, sizeof(Payload));

    ++recv_id;
  }

  Serial.print(payload.ax);
  Serial.print(",");
  Serial.print(payload.ay);
  Serial.print(",");
  Serial.print(payload.az);
  Serial.print(",");
  Serial.print(payload.gx);
  Serial.print(",");
  Serial.print(payload.gy);
  Serial.print(",");
  Serial.print(payload.gz);
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
  Serial.print(",");
  Serial.print(loss_percent);

  Serial.println();


  timedelta = micros() - last_start_time;
  if (timedelta >= 1000000) {
    if (payload.packet_id >= last_packet_id) {
      
      unsigned int packet_delta = payload.packet_id - last_packet_id;
      //Serial.print("packet_delta: ");
      //Serial.print(packet_delta);
      unsigned int recv_delta = recv_id - last_recv_id;
      //Serial.print(" | recv_delta: ");
      //Serial.print(recv_delta);

      // if there was some packet loss, packet_delta will be higher than recv_delta and
      // there will be some difference `diff`

      int diff = packet_delta - recv_delta;
      //Serial.print(" | diff: ");
      //Serial.print(diff);
      if (packet_delta == 0 && recv_delta == 0) {
        loss_percent = 100;
      } else {
        loss_percent = (diff * 100) / packet_delta;
      }
      //Serial.print(" | loss_percent: ");
      //Serial.println(loss_percent);
    }

    last_recv_id = recv_id;
    last_start_time = micros();
    last_packet_id = payload.packet_id;
  }
}