#define PIN1 9
#define LEFTAUD A8
#define RIGHTAUD A6

// Our Global Sample Rate, ~5 kHz
#define SAMPLETIME 200 //100 //us
#define INCRPER 6*1000 //30*1000
#define REFRESHRATE 60 //6 //80fps
#define FILTERORDER 2
#define NUMSAMPLES 3 //FILTERORDER+1

#define NUMSTRIPS 1
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
#define PIXELOUT rand()%20 < 4 //Likelihood that pixel will be decimated each iter
#define BASSSHAKERATE effects->bassShakeLag > effects->bassShakeThresh /*random(0,8) < 1*/
#define BASSSHAKERATIO 0.26

//TODO: also time rate of change of overall amplitude (drop/end of drop)
#define BASSDROPCUTOFF(VARIABILITY, VARIABILITYAVG) ((VARIABILITY - VARIABILITYAVG)/VARIABILITYAVG > 0.7)
#define PIXELBLASTCUTOFF(VARIABILITY, VARIABILITYAVG) ((VARIABILITY - VARIABILITYAVG)/VARIABILITYAVG > 2.4)
#define RECHARGECUTOFF(VARIABILITY, VARIABILITYAVG) ((VARIABILITY - VARIABILITYAVG)/VARIABILITYAVG > 0.3)
#define VARAVGCUTOUT 0.1
#define CRAZYDECAY 140

#define BASSADVANTAGE (-2)
#define LOWMIDADVANTAGE (-1.5)
#define HIGHMIDADVANTAGE 0.5
#define TREBLEADVANTAGE 1

#define BASSPER10SEC 15
#define LOWMIDPER10SEC 25
#define HIGHMIDPER10SEC 10
#define TREBLEPER10SEC 10

#define PIXELSMASHMAX 380
#define PIXELSMASHDUR 10
#define PIXELSMASHATTACKSPEED 6
#define PIXELSMASHENVDECAYLONGNESS 12

#define COLOURPALATTETIMEOUTDUR 4
#define COLOURPALATTETIMEOUTVAR 2

#define TAPERDIV 5

#define BOLTSPACING 6

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
