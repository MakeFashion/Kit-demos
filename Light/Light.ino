/* MakeFashion Kit Light Sensor demo
 * -------------------------------------------
 *  
 * Requires:
 * ---------
 *  MakeFashion controller board
 *  Light Sensor ( http://wiki.seeedstudio.com/Grove-Sunlight_Sensor/ )
 *  Motor ( http://wiki.seeedstudio.com/Grove-Vibration_Motor/ )
 *  
 * Dependencies:
 * -------------
 *  SI114X ( https://github.com/SeeedDocument/Grove-Sunlight_Sensor/raw/master/res/Grove_Sunlight_Sensor-master.zip )
 */

#include <Wire.h>

#include "Arduino.h"
#include "SI114X.h"

#define THRESHOLD   260
#define MOTOR_PIN   4

SI114X SI1145 = SI114X();

void setup() {
  pinMode( MOTOR_PIN, OUTPUT );
  while (!SI1145.Begin()) {
    delay(1000);
  }
}

void loop() {
  if( SI1145.ReadVisible() > THRESHOLD ) {
    digitalWrite(MOTOR_PIN, HIGH);
  } else {
    digitalWrite(MOTOR_PIN, LOW);
  }
  
  delay(100);

 
}


