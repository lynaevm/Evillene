#include "FastLED.h"

// ColorWavesWithPalettes
// Animated shifting color waves, with several cross-fading color palettes.
// by Mark Kriegsman, August 2015
//
// Color palettes courtesy of cpt-city and its contributors:
//   http://soliton.vm.bytemark.co.uk/pub/cpt-city/
//
// Color palettes converted for FastLED using "PaletteKnife" v1:
//   http://fastled.io/tools/paletteknife/
//

#if FASTLED_VERSION < 3001000
#error "Requires FastLED 3.1 or later; check github for latest code."
#endif

// ten seconds per color palette makes a good demo
// 20-120 is better for deployment
#define SECONDS_PER_PALETTE 2

// Forward declarations of an array of cpt-city gradient palettes, and 
// a count of how many there are.  The actual color palette definitions
// are at the bottom of this file.
extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
extern const uint8_t gGradientPaletteCount;

// Current palette number from the 'playlist' of color palettes
uint8_t gCurrentPaletteNumber = 0;

CRGBPalette16 gCurrentPalette( CRGB::Black);
CRGBPalette16 gTargetPalette( gGradientPalettes[0] );

void State2()
{
  EVERY_N_SECONDS( SECONDS_PER_PALETTE ) {
    gCurrentPaletteNumber = addmod8( gCurrentPaletteNumber, 1, gGradientPaletteCount);
    gTargetPalette = gGradientPalettes[ gCurrentPaletteNumber ];
  }

  EVERY_N_MILLISECONDS(20) {
    nblendPaletteTowardPalette( gCurrentPalette, gTargetPalette, 16);
  }
  
  colorwaves( leds, NUM_LEDS, gCurrentPalette);

  FastLED.show();
}


// This function draws color waves with an ever-changing,
// widely-varying set of parameters, using a color palette.
void colorwaves( CRGB* ledarray, uint16_t numleds, CRGBPalette16& palette) 
{
  static uint16_t sPseudotime = 0;
  static uint16_t sLastMillis = 0;
  static uint16_t sHue16 = 0;
 
  uint8_t sat8 = beatsin88( 87, 220, 250);
  uint8_t brightdepth = beatsin88( 341, 96, 224);
  uint16_t brightnessthetainc16 = beatsin88( 203, (25 * 256), (40 * 256));
  uint8_t msmultiplier = beatsin88(147, 23, 60);

  uint16_t hue16 = sHue16;//gHue * 256;
  uint16_t hueinc16 = beatsin88(113, 300, 1500);
  
  uint16_t ms = millis();
  uint16_t deltams = ms - sLastMillis ;
  sLastMillis  = ms;
  sPseudotime += deltams * msmultiplier;
  sHue16 += deltams * beatsin88( 400, 5,9);
  uint16_t brightnesstheta16 = sPseudotime;
  
  for( uint16_t i = 0 ; i < numleds; i++) {
    hue16 += hueinc16;
    uint8_t hue8 = hue16 / 256;
    uint16_t h16_128 = hue16 >> 7;
    if( h16_128 & 0x100) {
      hue8 = 255 - (h16_128 >> 1);
    } else {
      hue8 = h16_128 >> 1;
    }

    brightnesstheta16  += brightnessthetainc16;
    uint16_t b16 = sin16( brightnesstheta16  ) + 32768;

    uint16_t bri16 = (uint32_t)((uint32_t)b16 * (uint32_t)b16) / 65536;
    uint8_t bri8 = (uint32_t)(((uint32_t)bri16) * brightdepth) / 65536;
    bri8 += (255 - brightdepth);
    
    uint8_t index = hue8;
    //index = triwave8( index);
    index = scale8( index, 240);

    CRGB newcolor = ColorFromPalette( palette, index, bri8);

    uint16_t pixelnumber = i;
    pixelnumber = (numleds-1) - pixelnumber;
    
    nblend( ledarray[pixelnumber], newcolor, 128);
  }
}

// Alternate rendering function just scrolls the current palette 
// across the defined LED strip.
void palettetest( CRGB* ledarray, uint16_t numleds, const CRGBPalette16& gCurrentPalette)
{
  static uint8_t startindex = 0;
  startindex--;
  fill_palette( ledarray, numleds, startindex, (256 / NUM_LEDS) + 1, gCurrentPalette, 255, LINEARBLEND);
}





// Gradient Color Palette definitions for 33 different cpt-city color palettes.
//    956 bytes of PROGMEM for all of the palettes together,
//   +618 bytes of PROGMEM for gradient palette code (AVR).
//  1,494 bytes total for all 34 color palettes and associated code.


// Gradient palette "Blue_Green_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/Blue_Green.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 12 bytes of program space.

DEFINE_GRADIENT_PALETTE( Blue_Green_gp ) {
    0,   0,  0,255,
  127,   0, 55, 45,
  255,   0,255,  0};

// Gradient palette "es_emerald_dragon_08_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/emerald_dragon/tn/es_emerald_dragon_08.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_emerald_dragon_08_gp ) {
    0,  97,255,  1,
  101,  47,133,  1,
  178,  13, 43,  1,
  255,   2, 10,  1};

// Gradient palette "Blue_Cyan_Yellow_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/nd/basic/tn/Blue_Cyan_Yellow.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 20 bytes of program space.

DEFINE_GRADIENT_PALETTE( Blue_Cyan_Yellow_gp ) {
    0,   0,  0,255,
   63,   0, 55,255,
  127,   0,255,255,
  191,  42,255, 45,
  255, 255,255,  0};


  // Gradient palette "rainfall_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/jjg/misc/tn/rainfall.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 56 bytes of program space.

DEFINE_GRADIENT_PALETTE( rainfall_gp ) {
    0, 192,118,  3,
   36, 192,118,  3,
   36, 222,118, 24,
   72, 222,118, 24,
   72, 224,209, 37,
  109, 224,209, 37,
  109,  58,159, 43,
  145,  58,159, 43,
  145,   7,133, 52,
  182,   7,133, 52,
  182,   4,118, 50,
  218,   4,118, 50,
  218,   1, 85,  8,
  255,   1, 85,  8};

// Gradient palette "summer_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/h5/tn/summer.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 64 bytes of program space.

DEFINE_GRADIENT_PALETTE( summer_gp ) {
    0,   0, 55, 25,
   17,   1, 62, 25,
   33,   1, 72, 25,
   51,   3, 82, 25,
   68,   8, 92, 25,
   84,  14,104, 25,
  102,  23,115, 25,
  119,  35,127, 25,
  135,  48,141, 25,
  153,  67,156, 25,
  170,  88,169, 25,
  186, 112,186, 25,
  204, 142,201, 25,
  221, 175,217, 25,
  237, 210,235, 25,
  255, 255,255, 25};

// Gradient palette "es_emerald_dragon_12_gp", originally from
// http://soliton.vm.bytemark.co.uk/pub/cpt-city/es/emerald_dragon/tn/es_emerald_dragon_12.png.index.html
// converted for FastLED with gammas (2.6, 2.2, 2.5)
// Size: 16 bytes of program space.

DEFINE_GRADIENT_PALETTE( es_emerald_dragon_12_gp ) {
    0,   1, 10,  1,
  153, 104,156,  7,
  229, 206,237,138,
  255, 213,233,158};


// Single array of defined cpt-city color palettes.
// This will let us programmatically choose one based on
// a number, rather than having to activate each explicitly 
// by name every time.
// Since it is const, this array could also be moved 
// into PROGMEM to save SRAM, but for simplicity of illustration
// we'll keep it in a regular SRAM array.
//
// This list of color palettes acts as a "playlist"; you can
// add or delete, or re-arrange as you wish.
const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
 
  
  es_emerald_dragon_08_gp,
  summer_gp,  
  rainfall_gp,
  es_emerald_dragon_12_gp,
  Blue_Cyan_Yellow_gp,
  };



// Count of how many cpt-city gradients are defined:
const uint8_t gGradientPaletteCount = 
  sizeof( gGradientPalettes) / sizeof( TProgmemRGBGradientPalettePtr );
