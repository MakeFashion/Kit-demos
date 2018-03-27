/* MakeFashion Kit Ultrasonic Rangefinder demo
 * -------------------------------------------
 *  
 * Requires:
 * ---------
 *  MakeFashion controller board
 *  SeeedStudio Ultrasonic Ranger ( http://wiki.seeed.cc/Grove-Ultrasonic_Ranger/ )
 *  FastLED-compatible LEDs ( https://github.com/FastLED/FastLED/wiki/Overview#chipsets )
 *  
 * Dependencies:
 * -------------
 *  FastLED ( Available via Arduino package manager )
 *  SeeedStudio Ultrasonic library ( https://github.com/Seeed-Studio/Grove_Ultrasonic_Ranger/archive/master.zip )
 */

#include "Ultrasonic.h"
#include "FastLED.h"

FASTLED_USING_NAMESPACE

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define PIN_LEDARRAY    4
#define PIN_ULTRASONIC   2

#define LED_TYPE           WS2812B
#define COLOR_ORDER        GRB
#define NUM_LEDS           4
#define BRIGHTNESS         96
#define FRAMES_PER_SECOND  30
#define MAX_RANGE          20

CRGB leds_array[NUM_LEDS];
Ultrasonic ultrasonic(PIN_ULTRASONIC);

void setup() {
  delay(3000); // 3 second delay for recovery
  
  FastLED.addLeds<LED_TYPE, PIN_LEDARRAY, COLOR_ORDER>(leds_array, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
}

void loop()
{
  long range = ultrasonic.MeasureInCentimeters();
  long lit_leds; 

  if(range >= MAX_RANGE) {
    lit_leds = NUM_LEDS;
  } else {
    lit_leds = NUM_LEDS * range / MAX_RANGE;
  }

  for(int i=0;i<NUM_LEDS;i++) {
    if(i<lit_leds) {
      leds_array[i] = CRGB::White;
    } else {
      leds_array[i] = CRGB::Black;
    }
  }

  FastLED.show(); 
  FastLED.delay(1000 / FRAMES_PER_SECOND);
}



