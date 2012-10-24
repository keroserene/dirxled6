Assumption: we're on Arduino Uno.

--- Wiring ---
This code requires wires going from each analog output pin on LED in order starting from North.
So for Uno, which has 6 Analog outputs on pins 3, 5, 6, 9, 10, and 11:

3 --> N

5 --> NE

6 --> SE

9 --> S

10 --> SW

11 --> NW

Then, each LED can go through *some* resistor and back to ground.
