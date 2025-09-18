/*
This works best by powering the mouse and arduino separately, otherwise the mouse may not be initialized

Optical Encoder uses blue and grey wires for A & B
BLue wire goes to VCC and grey wire goes to P1.0 on CY7C63743-PC
Middle pin of dual photoT connected to VCC

*/

/**
 * Reads X/Y values from a PS/2 mouse connected to an Arduino
 * using the PS2Mouse library available from
 *   http://github.com/kristopher/PS2-Mouse-Arduino/
 * Original by Kristopher Chambers <kristopher.chambers@gmail.com>
 * Updated by Jonathan Oxer <jon@oxer.com.au>
 */

// https://github.com/T-vK/ESP32-BLE-Mouse/pull/64 // Fix for ESP32 V3.0.0 Board update

#include <PS2Mouse.h>
#include <BleMouse.h>


BleMouse bleMouse;

#define MOUSE_DATA 5 // arduino pin 5 // white 
#define MOUSE_CLOCK 18 // arduino pin 6 // green

#define ENCODER_PIN_A 32
#define ENCODER_PIN_B 33

PS2Mouse mouse(MOUSE_CLOCK, MOUSE_DATA, STREAM);

volatile int scroll_count = 0;
volatile bool a_last = LOW;
volatile bool b_last = LOW;

// Interrupt Service Routine for reading encoder state changes
volatile uint8_t encoder_state = 0;
const int8_t encoder_table[16] = {
   0, -1,  1,  0,
   1,  0,  0, -1,
  -1,  0,  0,  1,
   0,  1, -1,  0
};

// Interrupt Service Routine for reading encoder state changes with full quadrature decoding
void IRAM_ATTR encoder_isr() {
  encoder_state = ((encoder_state << 2) |
    (digitalRead(ENCODER_PIN_A) << 1) |
     digitalRead(ENCODER_PIN_B)) & 0x0F;

  scroll_count += encoder_table[encoder_state];
}


/**
 * Setup
 */
void setup()
{
  Serial.begin(38400); //38400
  // Possibly use 115200 for BT
  
  Serial.println("I HAVE BEGUN");
  mouse.initialize();
  
  // NEW CODE BELOW
  mouse.write(0xf2); // Get device ID
  mouse.read_byte(); // ACK
  int id = mouse.read_byte(); // Should be 3 for IntelliMouse
  Serial.print("Mouse ID: ");
  Serial.println(id);

  // Setup encoder pins
  pinMode(ENCODER_PIN_A, INPUT);
  pinMode(ENCODER_PIN_B, INPUT);
  a_last = digitalRead(ENCODER_PIN_A);
  b_last = digitalRead(ENCODER_PIN_B);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_A), encoder_isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCODER_PIN_B), encoder_isr, CHANGE);
  Serial.println("Encoder initialized");

  //////////////////////////////

  Serial.println("MOUSE INITIALIZED SUCCESSULLY");

  Serial.println("Starting BLE work!");
  bleMouse.begin();

}

/**
 * Main program loop
 */
void loop()
{
  
  if(bleMouse.isConnected()) {
  
  
  int16_t data[4];
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
    // unsigned long startTime;

    bleMouse.move(data[1]*5,data[2]*-5);
    if (data[0] == 10){ // right
      bleMouse.press(MOUSE_RIGHT);
      delay(5);
    }
    if (data[0] == 9){//bleMouse.isPressed(MOUSE_LEFT)){ // left
      bleMouse.press(MOUSE_LEFT);
      delay(2);
    }
    if(data[0]==8){ // refresh 
      bleMouse.release(MOUSE_LEFT);
      bleMouse.release(MOUSE_RIGHT);
    }

    if (data[0] == 12){ // middle
      bleMouse.click(MOUSE_MIDDLE);
      delay(2);
    }

   // Scroll wheel handling via encoder with smoothing
    static int last_scroll = 0;
    static float scroll_accum = 0.0;
    int scroll_delta = scroll_count - last_scroll;
    scroll_accum += scroll_delta * 0.25f; // scale for fine resolution
    last_scroll = scroll_count;

    int8_t scroll_out = (int8_t)scroll_accum;
    scroll_accum -= scroll_out;

    if (scroll_out != 0) {
      bleMouse.move(0, 0, scroll_out);
    }

    delay(2);//delay(5);// delay(100);
    
    // delay(500);

    // Serial.println("Scroll down");
    // startTime = millis();
    // while(millis()<startTime+2000) {
    //   bleMouse.move(0,0,ymvmt);
    //   delay(100);
    // }
    // delay(500);

    // Serial.println("Scroll left");
    // startTime = millis();
    // while(millis()<startTime+2000) {
    //   bleMouse.move(0,0,0,-1);
    //   delay(100);
    // }
    // delay(500);

    // Serial.println("Scroll right");
    // startTime = millis();
    // while(millis()<startTime+2000) {
    //   bleMouse.move(0,0,0,1);
    //   delay(100);
    // }
    // delay(500);

    // Serial.println("Move mouse pointer up");
    // startTime = millis();
    // while(millis()<startTime+2000) {
    //   bleMouse.move(0,-1);
    //   delay(100);
    // }
    // delay(500);

    // Serial.println("Move mouse pointer down");
    // startTime = millis();
    // while(millis()<startTime+2000) {
    //   bleMouse.move(0,1);
    //   delay(100);
    // }
    // delay(500);

    // Serial.println("Move mouse pointer left");
    // startTime = millis();
    // while(millis()<startTime+2000) {
    //   bleMouse.move(-1,0);
    //   delay(100);
    // }
    // delay(500);

    // Serial.println("Move mouse pointer right");
    // startTime = millis();
    // while(millis()<startTime+2000) {
    //   bleMouse.move(1,0);
    //   delay(100);
    // }
    // delay(500);

  }
}
