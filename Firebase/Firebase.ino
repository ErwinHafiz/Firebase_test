#include <Arduino.h>
#include "kredensial.h"
#include "Send_database.h"
#include "Pseudo_DHT22.h"

void setup() {
  Serial.begin(115200);
  setupSend();
  setupPDHT22(); // Initialize DHT22 Pseudo
}

void loop() {
  loopSend();
}
