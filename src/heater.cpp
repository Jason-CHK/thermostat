#include "heater.h"

#include <IRremote.hpp>

#define HEATER_FREQ_KHZ 38
#define HEATER_ADDR 0xCF10
#define HEATER_CMD 0xC
#define HEATER_REPEATS 5

Heater::Heater(pin_size_t pin) : pin_(pin) {}

void Heater::begin() {
  IrSender.begin(pin_, DISABLE_LED_FEEDBACK);
  IrSender.enableIROut(HEATER_FREQ_KHZ);
}

void Heater::toggle() {
  IrSender.sendNEC(HEATER_ADDR, HEATER_CMD, HEATER_REPEATS);
};
