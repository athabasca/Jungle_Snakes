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

    bool overrideCheck;
    float avgSignal;
    float stability;

    unsigned int currentRevPos;
    unsigned int prevRevPos;

    char railNum;
    char setNum;

    Biquad boltTrigger;
    float boltMagnitude;
    float triggerMagnitude;
    bool newBoltTrigger;
    float filterThresh;
    short avgFiresPer10Sec;
    int prevFire;
    float maxMag;

  public:
    BoltSet ();
    BoltSet (char newBoltPeriod, char newLowColour, char newHighColour, char newLowWidth, char newHighWidth, \
             Biquad newFilter, short newAvgFiresPerSec, unsigned char newSetNum, unsigned int *quotaTotal, short *netQuota);
    void callInit (char newBoltPeriod, char newLowColour, char newHighColour, char newLowWidth, char newHighWidth, \
                   Biquad newFilter, short newAvgFiresPerSec, unsigned char newSetNum, unsigned int *quotaTotal, short *netQuota);
    float trackBoltFiring (float newSample, Effects *effects);
    bool checkIfAddNewBolt (float *fastBoltAdvantage, LEDRail **oldestRail, LEDRail (*rail)[NUMRAILS], unsigned char *prevSetNum, unsigned int *quotaTotal, unsigned char *numBolts, int *memForBolts\
, short *netQuota);
    void incrBolts (LEDRail (*rail)[NUMRAILS]);
    char getBoltPeriod ();
    char getDivider();
    char getSetNum();
    bool checkQuota();
    void resetQuota();
    float getAvg();
    float getStability();
};

#endif
