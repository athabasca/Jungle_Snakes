#ifndef BoltSet_H
#define BoltSet_H
#include "JungleDefines.h"
#include <FastLED.h>
#include "Bolt.h"
#include "Biquad.h"
#include "Effects.h"
#include "LEDRail.h"

class BoltSet {
  private:
    //Bolt* newestBolt;
    Bolt boltCache [20];
    short newestBolt;
    short oldestBolt;
    short quota;

    char boltPeriod;
    char lowWidth;
    char highWidth;
    char lowColour;
    char highColour;

    boolean overrideCheck;
    float avgSignal;

    unsigned int currentRevPos;
    unsigned int prevRevPos;

    char railNum;
    char setNum;

    Biquad boltTrigger;
    float boltMagnitude;
    float triggerMagnitude;
    boolean newBoltTrigger;
    float filterThresh;
    short avgFiresPer10Sec;
    int prevFire;
    float maxMag;

  public:
    BoltSet ();
    BoltSet (char newBoltPeriod, char newLowColour, char newHighColour, char newLowWidth, char newHighWidth, \
             Biquad newFilter, short newAvgFiresPerSec, char newSetNum, unsigned int *quotaTotal, short *netQuota);
    void callInit (char newBoltPeriod, char newLowColour, char newHighColour, char newLowWidth, char newHighWidth, \
                   Biquad newFilter, short newAvgFiresPerSec, char newSetNum, unsigned int *quotaTotal, short *netQuota);
    void trackBoltFiring (float newSample, Effects *effects);
    boolean checkIfAddNewBolt (float *fastBoltAdvantage, LEDRail **oldestRail, LEDRail (*rail)[NUMRAILS], char *prevSetNum, unsigned int *quotaTotal, char *numBolts, int *memForBolts\
, LEDRail **newestRail, short *netQuota);
    void incrBolts (LEDRail (*rail)[NUMRAILS]);
    char getBoltPeriod ();
    char getDivider();
    char getSetNum();
    boolean checkQuota();
    void resetQuota();
    float getAvg();
};

#endif
