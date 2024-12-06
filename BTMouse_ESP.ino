/*
This works best by powering the mouse and arduino separately, otherwise the mouse may not be initialized
*/

/**
 * Reads X/Y values from a PS/2 mouse connected to an Arduino
 * using the PS2Mouse library available from
 *   http://github.com/kristopher/PS2-Mouse-Arduino/
 * Original by Kristopher Chambers <kristopher.chambers@gmail.com>
 * Updated by Jonathan Oxer <jon@oxer.com.au>
 */

#include <PS2Mouse.h>
#define MOUSE_DATA 5 // arduino pin 5 // white 
#define MOUSE_CLOCK 18 // arduino pin 6 // green

PS2Mouse mouse(MOUSE_CLOCK, MOUSE_DATA, STREAM);

/**
 * Setup
 */
void setup()
{
  Serial.begin(38400); //38400
  Serial.println("I HAVE BEGUN");
  mouse.initialize();
  Serial.println("MOUSE INITIALIZED SUCCESSULLY");
}

/**
 * Main program loop
 */
void loop()
{
  int16_t data[3];
  mouse.report(data);
  int16_t button = data[0];
  int16_t xmvmt = data[1];
  int16_t ymvmt = data[2];
  Serial.println(String(button)+":"+String(xmvmt)+","+String(ymvmt));
  /*
  Serial.print(data[0]); // Status Byte
  Serial.print(":");
  Serial.print(data[1]); // X Movement Data
  Serial.print(",");
  Serial.print(data[2]); // Y Movement Data
  Serial.println();
  */
  // delay(500); debuging
}
