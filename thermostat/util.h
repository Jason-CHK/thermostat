#ifndef UTIL_H
#define UTIL_H

/**
 * Round a float number to the nearest unit.
 * 
 * Example: round_float(3.45, 0.5) = 3.5
 * 
 * @param number The number to round.
 * @param unit The unit.
 * @return float The rounded number.
 */
inline float round_float(float number, float unit) {
  return round(number / unit) * unit;
}

#endif /* UTIL_H */
