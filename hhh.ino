/*
    Please install FastLED library first.
    In arduino library manage search FastLED
*/
#include <M5StickC.h>
#include "FastLED.h"

/*  This sketch is a compilation of the FastLED DemoReel and a button press. 
  There are a bunch of mode options to choose from within the sketch. 
  
  Note: Codebender might not support some parts required by this sketch 
  as the version of FastLED may be out of date. 
  Copy to & upload via Java IDE if you run into this issue.
  
  Have fun!
*/

#include "colorutils.h"
#include "colorpalettes.h"

#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

//This is where we adjust things to match our unique project:
#define NUM_LEDS    20    // adjust this to the number of LEDs you have: 16 or more
#define LED_TYPE    WS2812B // adjust this to the type of LEDS. This is for Neopixels
#define DATA_PIN    2      // adjust this to the pin you've connected your LEDs to   
#define BRIGHTNESS  255      // 255 is full brightness, 128 is half, 32 is an eighth.
#define SATURATION  255     // 0-255, 0 is pure white, 255 is fully saturated color
#define BUTTON_PIN  37       // Connect the button to GND and one of the pins. 
#define UPDATES_PER_SECOND 100

#define COLOR_ORDER RGB     // Try mixing up the letters (RGB, GBR, BRG, etc) for a whole new world of color combinations

#define NUM_MODES 4        //Update this number to the highest number of "cases"

uint8_t gHue = 1;           // rotating "base color" used by many of the patterns

uint16_t STEPS = 30;        // STEPS set dynamically once we've started up
uint16_t SPEED = 30;        // SPEED set dynamically once we've started up

CRGB leds[NUM_LEDS];
CRGBPalette16 currentPalette;
TBlendType    currentBlending;

int ledMode = 0;

const TProgmemPalette16 MyColors_p PROGMEM =
{
  CRGB:: DarkBlue,
  CRGB:: HotPink,
  CRGB:: Teal,
  CRGB:: BlueViolet,
 
  CRGB:: DodgerBlue,    
  CRGB:: DeepPink,
  CRGB:: Turquoise,
  CRGB:: Indigo,
 
  CRGB:: DarkBlue,
  CRGB:: HotPink,
  CRGB:: Teal,
  CRGB:: BlueViolet,
 
  CRGB:: DodgerBlue,    
  CRGB:: DeepPink,
  CRGB:: Turquoise,
  CRGB:: Indigo,
};

byte prevKeyState = HIGH;         // button is active low


//------------------SETUP------------------
void setup() {
  delay( 2000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE,DATA_PIN,COLOR_ORDER>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
  FastLED.setBrightness(BRIGHTNESS);
  currentBlending;
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}


//------------------MAIN LOOP------------------
void loop() {
 
  byte currKeyState = digitalRead(BUTTON_PIN);

  if ((prevKeyState == LOW) && (currKeyState == HIGH)) {
    shortKeyPress();
  }
  prevKeyState = currKeyState;

  static uint8_t startIndex = 0;
  startIndex = startIndex + 20; /* motion speed */

  switch (ledMode) {

/*Here are the mode options. Paste these lines into the below to choose. Only 4-5 will fit.
 * case X:  rainbow(); break;
 * case X:  confetti();  break;
 * case X:  sinelon(); break;
 * case X:  rainbowWithGlitter();  break;
 * case X:  juggle();  break;
 */

       case 0:  rainbow(); break;

       case 1:  bpm();  break;

       case 2:  sinelon(); break;

       case 3:  confetti(); break;
       
       case 4:  rainbowWithGlitter();  break;


                     
  } 
  FastLED.show();
  FastLED.delay(1000 / SPEED);  
 
  EVERY_N_MILLISECONDS( 20 ) { gHue++; } // slowly cycle the "base color" through the rainbow

}

//------------------Button Press Setup------------------
void shortKeyPress() {
  ledMode++;
    if (ledMode > NUM_MODES){
    ledMode=0; 
  }  
}

//GLITTER
void addGlitter( fract8 chanceOfGlitter) 
{
  if( random8() < chanceOfGlitter) {
    leds[ random16(NUM_LEDS) ] += CRGB::Gray;
  }
}


void rainbow() 
{
  // FastLED's built-in rainbow generator
  fill_rainbow( leds, NUM_LEDS, gHue, 32);
  fadeToBlackBy( leds, NUM_LEDS, 255-BRIGHTNESS); // fade the whole strip down.
}

void rainbowWithGlitter() 
{
  // built-in FastLED rainbow, plus some random sparkly glitter
  rainbow();
  addGlitter(80);
}



void confetti() 
{
  // random colored speckles that blink in and fade smoothly
  fadeToBlackBy( leds, NUM_LEDS, 10);
  int pos = random16(NUM_LEDS);
  leds[pos] += CHSV( gHue + random8(64), 255, BRIGHTNESS*2); //Adjusted Brightness with variable
}

void juggle() {
  // eight colored dots, weaving in and out of sync with each other
  fadeToBlackBy( leds, NUM_LEDS, 16);
  byte dothue = 0;
  for( int i = 0; i < 8; i++) {
    leds[beatsin16(i+7,0,NUM_LEDS)] |= CHSV(dothue, 255, BRIGHTNESS*2);//Adjusted Brightness with variable
    dothue += 32;
  }
}

void sinelon()
{
  // a colored dot sweeping back and forth, with fading trails
  fadeToBlackBy( leds, NUM_LEDS, 16);
  int pos = beatsin16(13,0,NUM_LEDS);
  leds[pos] += CHSV( gHue, 255, BRIGHTNESS*2);
}

void bpm()
{
  // colored stripes pulsing at a defined Beats-Per-Minute (BPM)
  uint8_t BeatsPerMinute = 62;
  CRGBPalette16 palette = MyColors_p; //can adjust the palette here
  uint8_t beat = beatsin8( BeatsPerMinute, 64, 255); 
  for( int i = 0; i < NUM_LEDS; i++) { //9948
    leds[i] = ColorFromPalette(palette, gHue+(i*2), beat-gHue+(i*10));
  }
}
