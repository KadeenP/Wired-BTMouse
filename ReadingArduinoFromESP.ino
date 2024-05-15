/*
This is for the ESP32 module
*/

//#include <BleMouse.h>
//BleMouse bleMouse;

#define RX_PIN 16
#define TX_PIN 17

void setup(){
  Serial.begin(38400);
  Serial.println("Starting BLE work!");

  Serial2.begin(38400, SERIAL_8N1, RX_PIN, TX_PIN);

  //bleMouse.begin();

}

void loop(){
    // String raw_input = Serial2.readString();
    // //int index = raw_input.indexOf(";");
    // String button_str = Serial2.readStringUntil(':');
    // String x_str = Serial2.readStringUntil(',');
    // int y_str = Serial2.parseInt(); 

    // Serial.print(raw_input + "<arduino<");
    // Serial.println(button_str + "poo" + x_str + "aed" + y_str);
 
      Serial.println(Serial2.readString());
    
}