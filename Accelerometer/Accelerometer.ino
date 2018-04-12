/* MakeFashion Kit Accelerometer demo
 * -------------------------------------------
 *  
 * Requires:
 * ---------
 *  MakeFashion controller board
 *  SeeedStudio 3-Axis Accelerometer ( http://wiki.seeedstudio.com/Grove-3-Axis_Digital_Accelerometer-1.5g/ )
 *  FastLED-compatible LEDs ( https://github.com/FastLED/FastLED/wiki/Overview#chipsets )
 *  
 * Dependencies:
 * -------------
 *  FastLED ( Available via Arduino package manager )
 */

#include <Wire.h>
#include "MMA7660.h"
#include "FastLED.h"

MMA7660 accelemeter;

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define PIN_LEDARRAY1  5
#define PIN_LEDARRAY2  6

#define LED_TYPE           WS2812B
#define COLOR_ORDER        GRB
#define NUM_LEDS           29
#define BRIGHTNESS         48
#define FRAMES_PER_SECOND  30
#define MAX_AXIS           24
#define SAMPLE_SIZE        5

CRGB leds_array1[NUM_LEDS];
CRGB leds_array2[NUM_LEDS];
uint8_t xAxis;
uint8_t yAxis;
int8_t lastX[SAMPLE_SIZE];
int8_t lastY[SAMPLE_SIZE];
uint8_t counter;

void setup() {
  delay(3000); // 3 second delay for recovery

  Serial.begin(9600);
  FastLED.addLeds<LED_TYPE, PIN_LEDARRAY1, COLOR_ORDER>(leds_array1, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.addLeds<LED_TYPE, PIN_LEDARRAY2, COLOR_ORDER>(leds_array2, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  accelemeter.init(); 

  xAxis = NUM_LEDS / 2;
  yAxis = xAxis;
  counter = 0;

  for(int i=0;i<SAMPLE_SIZE;i++) {
    lastX[i] = 0;
    lastY[i] = 0;
  }
}

void loop()
{
  FastLED.delay(1000 / FRAMES_PER_SECOND);
  int8_t z;
  int8_t xAverage = 0;
  int8_t yAverage = 0;
  uint8_t index = counter % SAMPLE_SIZE;
  
  if(counter < 255) { counter++; }
  else { counter = 0; }

  accelemeter.getXYZ(&lastX[index],&lastY[index],&z);
  //accelemeter.getAcceleration(&ax,&ay,&az);

  for(int i=0;i<SAMPLE_SIZE;i++) {
    if(abs(lastX[i]) < MAX_AXIS) { xAverage = xAverage + lastX[i]; }
    else { xAverage = xAverage + MAX_AXIS; }
    
    if(abs(lastY[i]) < MAX_AXIS) { yAverage = yAverage + lastY[i]; }
    else { yAverage = yAverage + MAX_AXIS; }
  }

  xAverage = xAverage / SAMPLE_SIZE;
  yAverage = yAverage / SAMPLE_SIZE;

  if(xAverage / SAMPLE_SIZE > 0.5) {
    if(xAverage < 0) {
      if(xAxis > 0) { xAxis--; }  
      else{ xAxis = 0; }
    }
    else { 
      if( xAxis < NUM_LEDS ) { xAxis++; }  
      else{ xAxis = NUM_LEDS; }
    }
  } else {
    if(xAverage < 0) {
      if(xAxis > 1) { xAxis = xAxis - 2; }  
      else{ xAxis = 0; }
    }
    else {
      if( xAxis < NUM_LEDS - 1) { xAxis = xAxis + 2; }  
      else{ xAxis = NUM_LEDS; }
    }
  }

  if(yAverage / SAMPLE_SIZE > 0.5) {
    if(yAverage < 0) {
      if(yAxis > 0) { yAxis--; }  
      else{ yAxis = 0; }
    }
    else { 
      if( yAxis < NUM_LEDS ) { yAxis++; }  
      else{ yAxis = NUM_LEDS; }
    }
  } else {
    if(yAverage < 0) {
      if(yAxis > 1) { yAxis = yAxis - 2; }  
      else{ yAxis = 0; }
    }
    else {
      if( yAxis < NUM_LEDS - 1) { yAxis = yAxis + 2; }  
      else{ yAxis = NUM_LEDS; }
    }
  }

  FastLED.clear();
  for(int i=0;i<xAxis;i++) { leds_array1[i] = CRGB::Red; }
  for(int i=0;i<yAxis;i++) { leds_array2[i] = CRGB::Red; }
  FastLED.show();

  delay(250);
}

