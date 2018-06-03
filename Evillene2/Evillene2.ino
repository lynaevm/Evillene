#include <Wire.h>
#include <SPI.h>
#include <Adafruit_CircuitPlayground.h>
#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>


//#define LED_PIN     9    //led strand is soldered to pin 6
#define NUM_LEDS_PER_STRIP 153

#define COLOR_ORDER GRB
#define NUM_MODES 3

CRGB leds[NUM_LEDS_PER_STRIP];

uint8_t brightness = 255;  //led strand brightness control
uint8_t cpbrightness = 30;  //circuit playground brightness control

int ledMode = -1;       //Initial mode 
bool leftButtonPressed;
bool rightButtonPressed;

void State2(); // Forward declaration to other file.

void setup() {
  
  Serial.begin(57600);
  CircuitPlayground.begin();
  FastLED.addLeds<NEOPIXEL, 6>(leds, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<NEOPIXEL, 9 >(leds, NUM_LEDS_PER_STRIP);
  set_max_power_in_volts_and_milliamps(5, 2000);               // FastLED 2.1 Power management set at 5V, 500mA
  FastLED.setBrightness( 200 );
  //FastLED.addLeds<WS2812B, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  
}

void loop()  {

  leftButtonPressed = CircuitPlayground.leftButton();
  rightButtonPressed = CircuitPlayground.rightButton();

  if (leftButtonPressed) {  //left button cycles through modes

    CircuitPlayground.clearPixels();
    //State1 = 0, 127, 254
    //State2 = 197, 104, 0
    //State3 = 254, 0, 127
    for(int i=0;i<5;i++) // change the 3 to however many pixels you want to show. 
    CircuitPlayground.setPixelColor(i, 127 * (ledMode % 3), 127 * ((ledMode + 1) % 3), 127 * ((ledMode + 2) % 3));





   
    //CircuitPlayground.clearPixels();
    ledMode = ledMode+1;
    ledMode = ledMode % NUM_MODES;

    
    CircuitPlayground.setPixelColor(6, 0, 20, 35);
    CircuitPlayground.setPixelColor(7, 0, 0, 125);
    CircuitPlayground.setPixelColor(8, 0, 20, 35);
   
    FastLED.delay(1000); // long delay to avoid button "debouncing" for debugging.
  }
    if (rightButtonPressed) {   // right button turns all leds off
      clearpixels();
      ledMode=-1;
    }
  
 switch (ledMode) {
       case 0:  //state 1 code here;
          State1();
          break; 
       case 1:  //state 2 code here; 
          State2();
          break;                    
       case 2:  //state 3 code here;
          State3();
          break; 
       default: 
          Mode99();
          break;      
  }
  //Note: If you want a variable delay based on mode, copy the delay code into the switch statement for each "case".
  FastLED.delay(1);
}

void clearpixels()
{
  for (int i = 0; i < NUM_LEDS_PER_STRIP; i++) leds[i] = CRGB::Black; 
  FastLED.show();
}

void Mode99()
{
  // static int is a fancy way of saying "keep this value at the same space in memory so it persists
  // between calls to the  method"
  static int state = 0;
  static int iterations = 0; 
  const int state0iterations = 100; // we want the lights to be on for 20 * 100 ms (2 seconds on) 
  const int state1iterations = 1000; // we want the lights to be off for 20 *1000 ms (20 seconds off) 

  // this method must exit back to the main loop() method quickly. 
  // outside this method call, in the parent loop is a delay(20) call. 
  // we expect this to run roughly every 20-25ms. (because of execution time, variable time between iterations. 
  // we could bypass this by using the millis() method call to get a more exact representation of time passed, but this should
  // suffice for our needs. 

  iterations++; 
  switch(state)
  {
    case 0: 
      if(iterations % state0iterations == 0) // % is the modulus operator. is basically says divide the two numbers and take the remainder.
      {
        // we are exiting the "on" state. turn off lights, restart counter.
        state = 1;
        iterations = 0; 
        fill_solid( leds, NUM_LEDS_PER_STRIP, CRGB(0,0,0));
        FastLED.show(); 
      }
      break;
      case 1:
      if(iterations % state1iterations == 0)
      {
        // we are exiting the "off" state. Turn on lights, and restart iteration counter. 
       state = 0; 
       iterations = 0; 
       fill_solid( leds, 3, CRGB(124,252,0)); //shows lawn green 
       FastLED.show(); 
      }
  }
  
  CircuitPlayground.clearPixels();
  CircuitPlayground.setPixelColor(2, 0, 55, 0);
  CircuitPlayground.setPixelColor(1, 0, 35, 0); 
  CircuitPlayground.setPixelColor(3, 0, 35, 0);
 } // end Mode99()

// variables specific to Mode 0 
float redStates[NUM_LEDS_PER_STRIP];
float blueStates[NUM_LEDS_PER_STRIP];
float greenStates[NUM_LEDS_PER_STRIP];
float fadeRate = .98;

void State1() {
  // Each time we enter, 1/20 chance of "priming" a light
  if (random(20) == 1) {
    uint16_t i = random(NUM_LEDS_PER_STRIP);
      if (redStates[i] < 1 && greenStates[i] < 1 && blueStates[i] < 1) { // Don't prime a light if its already lit. 
        
        redStates[i] = random(180);
        greenStates[i] = random(256);
        blueStates[i] = random(206);
    }
  }
    for(uint16_t l = 0; l < NUM_LEDS_PER_STRIP; l++) {
      if (redStates[l] > 1 || greenStates[l] > 1 || blueStates[l] > 1) {
        if (redStates[l] > 1) {
          redStates[l] = redStates[l] * fadeRate;
        } else {
          redStates[l] = 0;
        }
        
        if (greenStates[l] > 1) {
          greenStates[l] = greenStates[l] * fadeRate;
        } else {
          greenStates[l] = 0;
        }
        
        if (blueStates[l] > 1) {
          blueStates[l] = blueStates[l] * fadeRate;
        } else {
          blueStates[l] = 0;
        }
        CRGB newcolor;
        // setRGB requires integer values for parameters. 
        // cast to int to truncate float values. 
        newcolor.setRGB( (int)redStates[l], (int)greenStates[l], (int)blueStates[l]);
        leds[l] = newcolor;
      }
    }
    FastLED.show();
    // Note: We removed the delay here.
    // All delays should be handled in the main loop() method.
}

// specific for Mode2
#define COLORS 3

void State3()
{
  static int color = 0;
  static int index = 0;

  if( index == 0)
  {
    color++; 
    color = color % COLORS;
  }

  index++; 
  index = index%NUM_LEDS_PER_STRIP;
  
  switch(color){
    case 0:
      leds[index].setRGB( 0, 255, 0);
      //= CRGB::Chartreuse; // Green
      break;
    case 1:
      leds[index].setRGB( 0, 255, 255);
      //= CRGB::Green; // Cyan
      break;
    case 2:
      leds[index].setRGB( 0, 50, 255);
      //= CRGB::Cyan; // Blue
      break; 
  }
  FastLED.show();
}
