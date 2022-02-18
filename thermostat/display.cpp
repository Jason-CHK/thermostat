#include "display.h"

#define OLED_RESET 16  // This OLED display has no reset.
#define OLED_WIDTH 128
#define OLED_HEIGHT 64
#define OLED_TEXT_1_WIDTH 6
#define OLED_TEXT_1_HEIGHT 8
#define OLED_TEXT_2_WIDTH 12
#define OLED_TEXT_2_HEIGHT 16

Display::Display() : display_(OLED_RESET) {}

void Display::begin() {
  display_.begin();
  display_.setTextColor(WHITE);
  display_.clearDisplay();
  display_.display();
}

void Display::displaySetup() {
  display_.clearDisplay();
  display_.setTextSize(2);

  display_.setCursor(10, 16);
  display_.println("HEATER");
  display_.setCursor(29, 32);
  display_.println("ON?");

  display_.setCursor(84, 0);
  display_.println("Y");
  display_.setCursor(104, 0);
  display_.println("-");
  display_.setCursor(108, 0);
  display_.println(">");

  display_.setCursor(84, 48);
  display_.println("N");
  display_.setCursor(104, 48);
  display_.println("-");
  display_.setCursor(108, 48);
  display_.println(">");

  display_.display();
}

void Display::displayStatus(Status status) {
  display_.clearDisplay();
  display_.setTextSize(1);

  // Row 1 - Set temp.
  int x = 0;
  int y = 0;
  placeText(x, y, "Set temp:");
  x = OLED_WIDTH - calcTempWidth(status.set_temp_C, status.set_temp_F);
  placeTemp(x, y, status.set_temp_C, status.set_temp_F);

  // Row 2 - Heat index.
  x = 0;
  y += OLED_TEXT_1_HEIGHT;
  placeText(x, y, "Heat idx:");
  x = OLED_WIDTH - calcTempWidth(status.heat_idx_C, status.heat_idx_F);
  placeTemp(x, y, status.heat_idx_C, status.heat_idx_F);

  // Row 3 - Heater status.
  x = 0;
  y += OLED_TEXT_1_HEIGHT;
  placeText(x, y, "Heater:");
  String heaterStr = status.heater_on ? "ON" : "OFF";
  x = OLED_WIDTH - OLED_TEXT_1_WIDTH * heaterStr.length();
  placeText(x, y, heaterStr);

  // Row 4 - Room temp.
  x = 0;
  y += OLED_TEXT_1_HEIGHT;
  placeText(x, y, "Room temp:");
  x = OLED_WIDTH - calcTempWidth(status.room_temp_C, status.room_temp_F);
  placeTemp(x, y, status.room_temp_C, status.room_temp_F);

  // Row 5 - Humidity.
  x = 0;
  y += OLED_TEXT_1_HEIGHT;
  placeText(x, y, "Humidity:");
  String humidityStr = String(status.humidity_pct, 2);
  x = OLED_WIDTH - OLED_TEXT_1_WIDTH * (humidityStr.length() + 1);
  placeText(x, y, humidityStr);
  placeText(x, y, "%");

  // Row 6 - Max diff.
  x = 0;
  y += OLED_TEXT_1_HEIGHT;
  placeText(x, y, "Max diff:");
  x = OLED_WIDTH - calcTempWidth(status.max_diff_C, status.max_diff_F);
  placeTemp(x, y, status.max_diff_C, status.max_diff_F);

  // Row 7 - Battery voltage.
  x = 0;
  y += OLED_TEXT_1_HEIGHT;
  placeText(x, y, "Battery:");
  String batteryStr = String(status.battery_V, 2);
  x = OLED_WIDTH - OLED_TEXT_1_WIDTH * (batteryStr.length() + 1);
  placeText(x, y, batteryStr);
  placeText(x, y, "V");

  // Row 8 - Empty.
  x = 0;
  y += OLED_TEXT_1_HEIGHT;

  display_.display();
}

void Display::placeText(int &x, int y, String text) {
  display_.setCursor(x, y);
  display_.print(text);
  x += text.length() * OLED_TEXT_1_WIDTH;
}

void Display::placeTemp(int x, int y, float celsius, float fahrenheit) {
  String celsiusStr = String(celsius, 1);
  String fahrenheitStr = String(fahrenheit, 1);

  String celsiusInteger = celsiusStr.substring(0, celsiusStr.length() - 2);
  String celsiusTenth =
      celsiusStr.substring(celsiusStr.length() - 1, celsiusStr.length());

  String fahrenheitInteger =
      fahrenheitStr.substring(0, fahrenheitStr.length() - 2);
  String fahrenheitTenth = fahrenheitStr.substring(fahrenheitStr.length() - 1,
                                                   fahrenheitStr.length());

  display_.setTextSize(1);
  placeText(x, y, celsiusInteger);
  placeThinDot(x, y);
  placeText(x, y, celsiusTenth);
  placeDegree(x, y);
  placeText(x, y, "C/");
  placeText(x, y, fahrenheitInteger);
  placeThinDot(x, y);
  placeText(x, y, fahrenheitTenth);
  placeDegree(x, y);
  placeText(x, y, "F");
}

void Display::placeThinDot(int &x, int y) {
  x -= 2;
  placeText(x, y, ".");
  x -= 1;
}

void Display::placeDegree(int &x, int y) {
  display_.setCursor(x - 2, y - 5);
  display_.print(".");
  x += 3;
}

int Display::calcTempWidth(float celsius, float fahrenheit) {
  return OLED_TEXT_1_WIDTH *
         (String(celsius, 1).length() + String(fahrenheit, 1).length() + 3);
}
