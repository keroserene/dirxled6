/*

Directional Indicator for 6 LEDs on Arduino
Copyright (C) 2012 Serene Han
  
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/
#define TOTAL_LEDS 6
// Arduino Uno has 6 analog outputs, so this code
// must deal with 6 analog LEDs (which works decently well given
// the fading gradient)
#define LED_N 3
#define LED_NE 5
#define LED_SE 6
#define LED_S 9
#define LED_SW 10
#define LED_NW 11

#define DEGREE_SEGMENT (360/TOTAL_LEDS)
#define ANALOG_MAX 255

// Simplify access to output LEDs
int leds[] = {LED_N, LED_NE, LED_SE, LED_S, LED_SW, LED_NW};
int oAlpha, oOmega = 0; // Store previously lit LEDs to optimize reset

float d = 0; // Temporary.

void setup() {}

// The following is a test that just runs the light in
// a circle.
void loop() {
  directionToLED(d);
  
  d += 6;
  if (d > 360) 
    d -= 360;
  delay(20);
}

// Light up the LEDs corresponding to direction.
//
// Angular bearing starts at 0-degrees north and increases clockwise.
//
// There is a maximum combined intensity of 255, which
// is spread over two leds (alpha and omega) which border
// an target directional segment. (In this case, 60-degree slices)
// 
// For example, 30-degrees would light up N and NE equally at 127
// because it is between the two LEDs at 0 and 60 degrees.
// However, 240-degrees would light up SW maximally at 255.
//
// Args:
//   angle - Input angle (in degrees) 
//
// Returns nothing
void directionToLED(float angle) {
  
  // Determine boundary LEDs of target angle segment.
  int seg = (int)angleWrap(angle)/DEGREE_SEGMENT;
  int alpha = leds[seg];
  int omega = leds[(seg + 1) % TOTAL_LEDS];
  
  // Reset previously lit LEDs if they in use with current indication.
  if (oAlpha != alpha && oAlpha != omega)
    analogWrite(oAlpha, 0);
  if (oOmega != alpha && oAlpha != omega)
    analogWrite(oOmega, 0);
  
  // Determine percent offset of angle within target segment,
  // which determines intensity of boundary P.W.M. LEDs.
  float raw = floatModulus(angle, DEGREE_SEGMENT);
  float intensity = ANALOG_MAX * (raw/DEGREE_SEGMENT);
  analogWrite(alpha, ANALOG_MAX - intensity);
  analogWrite(omega, intensity);
  
  // Remember LEDs.
  oAlpha = alpha; 
  oOmega = omega;
}
  
// Force an angle (in degrees) between 0-359
float angleWrap(float angle) { return floatModulus(angle, 360); }

// Helper: Forces a floating point modulo'd within an int range,
// without losing the fractional bit.
//
// Args:
//   x - Input floating point
//   m - Modulus (must be positive)
//
// Returns remainder of x divided by m
float floatModulus(float x, int m) {
  float frac = x - (int)x; 
  return frac + ((int)x % m);
}
