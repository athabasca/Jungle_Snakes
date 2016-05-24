#ifndef EFFECTS_H
#define EFFECTS_H
#include "JungleDefines.h"

class Effects {
  public:

    bool bassShakeCheck;
    short bassShakeCounter;
    short bassShakeDir;
    bool bassShakeType;
    bool bassShakeDown;
    float bassShakeLevel;
    float bassShakeLag;

    float overrideLevel;

    short bassBendCounter;
    float bassShakeThresh;
    float bassShakeRatio;

    float pixelSmashLevel;
    short pixelSmashCounter;
    bool pixelSmashCheck;
    bool pixelSmashDown;
    bool pixelSmashGrid[NUMSTRIPS][STRIPLEN];

    char colourPalatte;
    short colourPalatteTimeout;

    bool crazyMode;
    short crazyCounter;
    short crazyCounterSquareRoot;
    float sectionAvg;
    float prevSectionAvg;
    float variability;
    float variabilityAvg;
    //float eqBandsAvg;
    float variabilityFactor[NUMBOLTSETS];
    bool startup;

    float eqBandsAvg;
    float bassBandAvg;
	
	Effects (int seed);
	void checkBassShake ();
	void checkPixelSmash ();
	void checkEffectLevels (float trebleAvg, float midAvg, float bassAvg);
  float checkSongAvg (float sample);
  void checkCrazyMode (bool newBolt);
	};
	
	#endif
