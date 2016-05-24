#ifndef LEDRail_H
#define LEDRail_H
#include "JungleDefines.h"
#include <FastLED.h>
#include "Effects.h"

class LEDRail {
  private:
    char stripNum;
    char railNum;
    bool flipStrip;
    bool railAvail;
    short railTimeout;
    char recentRailPeriod;
    LEDRail *next;
	CRGB (*strip)[NUMSTRIPS][STRIPLEN]; //Is this allocating an int pointer or the whole array of CRGBs??
	Effects *effects;

  public:
    LEDRail ();
    LEDRail (char newStripNum, char newRailNum, CRGB (*newStrip)[NUMSTRIPS][STRIPLEN], Effects *newEffects);
    void callInit (char newStripNum, char newRailNum, CRGB (*newStrip)[NUMSTRIPS][STRIPLEN], Effects *newEffects);
    bool incrRail ();
    bool checkRailAvail ();
    void setRailAvail (bool newRailAvail);
    void setRailTimeout (short newRailTimeout, char newRecentRailPeriod);
    void setBoltInRail (char memColour, char memStart, char memWidth);
    void showRail ();
    bool checkIfValidPeriod (char newBoltPeriod, float *fastBoltAdvantage);
    void setFringePixel (char currentHead, CRGB pixelColour);
    LEDRail *getNext ();
    void setNext (LEDRail *newNext);
    char getRailNum();
    short getTimeout();
    char checkPrevRailPer();
};

#endif
