#include <FastLED.h>
//#include <Adafruit_NeoPixel.h>
#include <TimerOne.h>
#include <TimerThree.h>
#ifdef __AVR__
#include <avr/power.h>
#endif
//#include <SdFatUtil.h>

#include "JungleDefines.h"
#include "Effects.h"
#include "LEDRail.h"
#include "Biquad.h"
#include "BoltSet.h"

unsigned int startupTimeout;

unsigned char numBolts = 0;
int memForBolts = 0;
unsigned int quotaTotal = 0;
short netQuota = 0;
float fastBoltAdvantage = 4.0;
unsigned char prevSetNum = 0;
bool refreshing = false;

CRGB strip[NUMSTRIPS][STRIPLEN];

Effects effects;

LEDRail *oldestRail;
LEDRail rail[NUMRAILS];

Biquad eqFilters[4];
Biquad eqFilters2[4];

bool boltlock;

BoltSet eqBolts[NUMBOLTSETS];

// interrupt handlers
void ledwork();
void signalaquire();
// utility functions
uint32_t FreeRam();

void setup() {
  startupTimeout = millis();
  delay(100);
  Serial.begin(115200);
  boltlock = false;
  //Seerial.print("boltset size: ");
  //Seerial.println(sizeof(BoltSet));
  //Seerial.print("bolt size: ");
  //Seerial.println(sizeof(Bolt));
  //Seerial.print("filter size: ");
  //Seerial.println(sizeof(Filter));
  //Seerial.print("led rail size: ");
  //Seerial.println(sizeof(LEDRail));

  FastLED.addLeds<NEOPIXEL, PIN1>(strip[0], STRIPLEN);
  FastLED.clear();
  FastLED.show();

  for (unsigned char c = 0; c < NUMRAILS; c++) {
    rail[c].callInit(c / RAILSPERSTRIP, c, &strip, &effects);
  }

  oldestRail = NULL;

  //bass bolts: 1250Hz sample, 300Hz peak, LPF, 2.0 Q, 6dB Gain
  eqFilters[0].callInit(bq_type_lowpass, 300.0 / 1250.0, 2.0, 6, 4);
  //bass bolts cascade: 1250Hz sample, 240Hz peak, LPF, 2.0 Q, 6dB Gain
  eqFilters2[0].callInit(bq_type_lowpass, 240.0 / 1250.0, 2.0, 6, 4);

  //low mid bolts: 2.5k sample, 700Hz peak, 2 Q, 6dB Gain
  eqFilters[1].callInit(bq_type_bandpass, 700.0 / 2500.0, 2.0, 6, 2);
  //low mid bolts cascade: 2.5k sample, 660Hz peak, 2 Q, 6dB Gain
  eqFilters2[1].callInit(bq_type_bandpass, 660.0 / 2500.0, 2.0, 6, 2);

  //high mid bolts: 2.5k sample, 1.2k peak, 2 Q, 6dB Gain
  eqFilters[2].callInit(bq_type_bandpass, 1200.0 / 2500.0, 2.0, 6, 2);
  //high mid bolts cascade: 2.5k sample, 1.1k peak, 2 Q, 6dB Gain
  eqFilters2[2].callInit(bq_type_bandpass, 1100.0 / 2500.0, 2.0, 6, 2);

  //high bolts: 5k sample, 2k peak, HPF, 2 Q, 6dB Gain
  eqFilters[3].callInit(bq_type_highpass, 2000.0 / 5000.0, 2.0, 6, 1);
  //high bolts cascade: 5k sample, 2.1k peak, HPF, 2 Q, 6dB Gain
  eqFilters2[3].callInit(bq_type_highpass, 2100.0 / 5000.0, 2.0, 6, 1);

  //Envelope filters: 78.125Hz sample, LPF, 10Hz peak, .707 Q, 6dB Gain
  Biquad tempFilter;
  tempFilter.callInit(bq_type_lowpass, 10.0 / 78.125, 0.707, 6, 64);

  char minColour, maxColour;
  minColour = 0;
  maxColour = minColour + COLOURRANGEWIDTH;
  eqBolts[0].callInit(7, minColour, maxColour, 24, 36, tempFilter, 20, 1, &quotaTotal, &netQuota);

  minColour = SLOWCOLOURSEP;
  maxColour = minColour + COLOURRANGEWIDTH;
  eqBolts[1].callInit(6, minColour, maxColour, 18, 24, tempFilter, 15, 2, &quotaTotal, &netQuota);

  minColour = SLOWCOLOURSEP + COLOURGROUPSEP;
  maxColour = minColour + COLOURRANGEWIDTH;
  eqBolts[2].callInit(2, minColour, maxColour, 14, 20, tempFilter, 10, 3, &quotaTotal, &netQuota);

  minColour = SLOWCOLOURSEP + COLOURGROUPSEP + FASTCOLOURSEP;
  maxColour = minColour + COLOURRANGEWIDTH;
  eqBolts[3].callInit(1, minColour, maxColour, 8, 12, tempFilter, 20, 4, &quotaTotal, &netQuota);

  fastBoltAdvantage = 2.0 * (eqBolts[0].getBoltPeriod() - eqBolts[3].getBoltPeriod());

  pinMode(RIGHTAUD, INPUT);
  Timer1.initialize(SAMPLETIME);
  Timer1.attachInterrupt(signalaquire, SAMPLETIME);

  Timer3.initialize(INCRPER);
  Timer3.attachInterrupt(ledwork, INCRPER);

}

void loop() {
  static int stabilityTimeout = millis() / 200;
  if (millis() / 200 - stabilityTimeout > 1) {
    float tempStab = 0.0;
    for (unsigned char i = 0; i < NUMBOLTSETS; i++)
      tempStab += eqBolts[i].getStability();
    Serial.println(tempStab);
    stabilityTimeout = millis() / 200;
  }

  while (boltlock) { //Wait for boltlock to turn off
    delay(1); //Poll every 1 ms
  }

  static unsigned char c;
  static bool resetQuotas;
  resetQuotas = true; //Assert true
  for (c = 0; c < NUMBOLTSETS || c < NUMRAILS; c++) {
    if (c < NUMBOLTSETS) {
      if (!eqBolts[c].checkIfAddNewBolt(&fastBoltAdvantage, &oldestRail, &rail, &prevSetNum, &quotaTotal, &numBolts, &memForBolts, &netQuota))
        delay(REFRESHRATE / NUMBOLTSETS);
    }
  }
  for (c = 0; c < NUMBOLTSETS; c++) {
    resetQuotas = eqBolts[c].checkQuota();
  }
  if (resetQuotas) {
    for (c = 0; c < NUMBOLTSETS; c++) {
      eqBolts[c].resetQuota();
    }
    quotaTotal /= 10;
  }
}

void ledwork() {
  //Seerial.println("ledwork");
  boltlock = true;

  static unsigned char c;
  for (c = 0; c < NUMBOLTSETS; c++) {
    eqBolts[c].incrBolts(&rail);
  }
  for (c = 0; c < NUMRAILS; c++) {
    if (rail[c].incrRail()) {
      //Seerial.println("railtestsuccess");
      if (oldestRail == NULL) {
        //Seerial.println("nooldest");
        oldestRail = &(rail[c]);
      }
      rail[c].setRailAvail(true);
    }
  }

  effects.checkBassShake();
  effects.checkPixelSmash();
  effects.checkEffectLevels(eqBolts[3].getAvg(), eqBolts[1].getAvg(), eqBolts[0].getAvg());
  FastLED.show();
  
  boltlock = false;
}


void signalaquire() {
  //Seerial.println("signalaquire");
  static float sample;
  sample = (float)analogRead(RIGHTAUD);
  static float longavg = sample;
  static short i = 0;
  i++;
  if (i > 100) {
    //effects.checkSongAvg(abs(longavg-sample));
    /*Serial.print(effects.sectionAvg);
      Serial.print(" / ");
      Serial.println(effects.songAvg);*/
    longavg = 0.999 * longavg + 0.001 * sample;
    i = 0;
  }
  sample = sample - longavg;

  static unsigned char c;
  for (c = 0; c < NUMBOLTSETS; c++) {
    // Serial.println(eqFilters[c].process(sample));
    if (!(i % eqBolts[c].getDivider()))
      eqBolts[c].trackBoltFiring(eqFilters[c].process(eqFilters2[c].process(sample)), &effects);
    else if (!(i % eqFilters[c].getDivider()))
      eqFilters[c].process(eqFilters2[c].process(sample));
  }
}

uint32_t FreeRam() { // for Teensy 3.0
  uint32_t stackTop;
  uint32_t heapTop;

  // current position of the stack.
  stackTop = (uint32_t)&stackTop;

  // current position of heap.
  void* hTop = malloc(1);
  heapTop = (uint32_t)hTop;
  free(hTop);

  // The difference is the free, available ram.
  return stackTop - heapTop;
}
