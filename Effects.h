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
	
	Effects ();
	void checkBassShake ();
	void checkPixelSmash ();
	void checkEffectLevels (float trebleAvg, float midAvg, float bassAvg);
	};
	
	#endif