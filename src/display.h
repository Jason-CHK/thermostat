#ifndef DISPLAY_H
#define DISPLAY_H

#include <ArducamSSD1306.h>

/**
 * Wrapper of an ArducamSSD1306 display over I2C bus (pins defined by hardware
 * I2C).
 *
 * HardWare I2C pins - Arduino Nano Every
 *   SDA - D18
 *   SCL - D19
 *
 * Unless explicitly specified, all methods expect the font size to be 1.
 */
class Display {
 public:
  /**
   * Structure to hold all status information to display.
   */
  typedef struct Status {
    bool heater_on;
    float set_temp_C;
    float set_temp_F;
    float room_temp_C;
    float room_temp_F;
    float humidity_pct;
    float heat_idx_C;
    float heat_idx_F;
    float max_diff_C;
    float max_diff_F;
  } Status;

  /**
   * Construct a new Display object.
   */
  Display();

  /**
   * Call this before using the display to clear the screen.
   */
  void begin();

  /**
   * Display the screen asking user the current heater status. Font size is 2.
   *
   * ----------------
   * |         Y -> |
   * | HEATER       |
   * |  ON?         |
   * |         N -> |
   * ----------------
   */
  void displaySetup();

  /**
   * Display the screen to show thermostat status. Font size is 1.
   *
   * -123456789012345678901-  <-- ruler
   * -----------------------
   * |Set temp: 30.5C/60.0F|  yellow zone
   * |Heat idx: 30.5C/60.0F|
   * -----------------------
   * |Heater:           off|
   * |Room temp:30.5C/60.0F|
   * |Humidity:      20.00%|  blue zone
   * |Max diff:   1.0C/1.0F|
   * |                     |
   * |  Tracking heat idx  |
   * -----------------------
   */
  void displayStatus(Status status);

 private:
  ArducamSSD1306 display_;

  /**
   * Place a text string on the screen and move the column to the end of the
   * text.
   *
   * @param[in, out] x Column in pixel to place the text. This will be moved to
   *     the end of the text.
   * @param[in] y Row in pixel to place the text.
   * @param[in] text Content to display.
   */
  void placeText(int &x, int y, String text);

  /**
   * Place a temperature on display.
   *
   * Example: 30.5°C/60.0°F
   *
   * Font size is 1. The "°" is replaced with a thin "." placed at the upper
   * left corner of C and F. The "." in temperatures is thin.
   *
   * @param x Column in pixel to place the temperature.
   * @param y Row in pixel to place the temperature.
   * @param celsius Degree in Celsius.
   * @param fahrenheit Degree in Fahrenheit.
   */
  void placeTemp(int x, int y, float celsius, float fahrenheit);

  /**
   * Place a thin dot (.) on display and move the column to the end of the dot.
   * The dot takes half the width of a character.
   *
   * @param x Column in pixel to place the dot. This will be moved to the end of
   *     the dot.
   * @param y Row in pixel to place the dot.
   */
  void placeThinDot(int &x, int y);

  /**
   * Place a degree sign (°) on display and move the column to the end of it.
   * The degree sign is a thin dot (.) placed with an offset to make it thin and
   * at the top of its character box.
   *
   * @param x Column in pixel to place the degree sign. This will be moved to
   *     the end of the degree sign.
   * @param y Row in pixel to place the degree sign.
   */
  void placeDegree(int &x, int y);

  /**
   * Calculate the width in pixel for a C/F temperature. Font size is 1.
   *
   * @param celsius Degree in Celsius.
   * @param fahrenheit Degree in Fahrenheit.
   * @return int Width in pixel.
   */
  static int calcTempWidth(float celsius, float fahrenheit);
};

#endif /* DISPLAY_H */
