#include "Pseudo_DHT22.h"
#include <Arduino.h>
#include <random>

std::default_random_engine generator;
std::uniform_real_distribution<float> distribution(20.0, 100.0);

void setupPDHT22() {
  // Nothing to initialize for pseudo
}

float readTemperature() {
  return distribution(generator);
}

float readHumidity() {
  return distribution(generator);
}
