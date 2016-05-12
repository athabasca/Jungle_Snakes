#define PIN1 3
#define PIN2 5
#define PIN3 7
#define PIN4 9
#define MIC A8

// Our Global Sample Rate, ~5 kHz
#define SAMPLETIME 200 //100 //us
#define INCRPER 6*1000 //30*1000
#define REFRESHRATE 6 //80fps
#define FILTERORDER 2
#define NUMSAMPLES 3 //FILTERORDER+1

#define NUMSTRIPS 4
#define RAILSPERSTRIP 2
#define STRIPLEN 100
#define NUMRAILS (NUMSTRIPS*RAILSPERSTRIP) //Rails for snakes, 2/strand in final rev
#define RAILLEN ((STRIPLEN*NUMSTRIPS)/NUMRAILS)
#define NUMBOLTSETS 4

#define OVERRIDEDECAY 0.92
#define PIXELSMASHDECAY 0.99992
#define BASSSHAKEDECAY 0.88
#define BASSSHAKELAG 2.0

#define BASSSHAKEMAX 250
#define BASSSHAKEDUR 60
#define BASSSHAKEATTACKSPEED 7
#define BASSSHAKEENVDECAYLONGNESS 12

#define BASSBENDMAX 30
#define BASSBENDDUR 5
#define BASSBENDATTACKSPEED 6
#define BASSBENDENVDECAYLONGNESS 3
#define PIXELOUT random(0, 20) < 4 //Likelihood that pixel will be decimated each iter
#define BASSSHAKERATE effects->bassShakeLag > effects->bassShakeThresh /*random(0,8) < 1*/
#define BASSSHAKERATIO 0.12

#define BASSDROPCUTOFF 2.6
#define CRAZYDECAY 140

#define PIXELSMASHMAX 380
#define PIXELSMASHDUR 10
#define PIXELSMASHATTACKSPEED 6
#define PIXELSMASHENVDECAYLONGNESS 12

#define COLOURPALATTETIMEOUTDUR 12
#define COLOURPALATTETIMEOUTVAR 6

#define BOLTSPACING 20

#define MINCOLOURPALATTEDISP 70
#define COLOURRANGEWIDTH 12
#define SLOWCOLOURSEP 45
#define FASTCOLOURSEP 25
#define COLOURGROUPSEP 70



// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

// defines for setting and clearing register bits
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
