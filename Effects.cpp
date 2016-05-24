#include "Effects.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

Effects::Effects (int seed) {

  bassShakeCheck = false;
  bassShakeCounter = 0;
  bassShakeDir = 1;
  bassShakeType = false;
  bassShakeDown = false;
  overrideLevel = 6.0;
  bassShakeLevel = 20.0;
  bassShakeLag = 1.0;

  bassBendCounter = 0;
  bassShakeThresh = 0.1 / BASSSHAKERATIO;
  bassShakeRatio = BASSSHAKERATIO;

  pixelSmashLevel = 10.0;
  pixelSmashCounter = 0;
  pixelSmashCheck = false;
  pixelSmashDown = false;
  for (char i = 0; i < NUMSTRIPS; i++) {
    for (char c = 0; c < STRIPLEN; c++ ) {
      pixelSmashGrid[i][c] = false;
    }
  }
  
        srand(seed);
  
  colourPalatte = rand()%256;// (MINCOLOURPALATTEDISP + rand() % (256 - 2 * MINCOLOURPALATTEDISP)) % 256;;
  colourPalatteTimeout = COLOURPALATTETIMEOUTDUR - COLOURPALATTETIMEOUTVAR / 2 + rand() % COLOURPALATTETIMEOUTVAR;

  crazyMode = false;
  crazyCounter = 0;
  crazyCounterSquareRoot = 0;
  sectionAvg = 1;
  prevSectionAvg = 1;
  variability = 0.5;
  variabilityAvg = 0.5;
  startup = true;

  eqBandsAvg = 1;
  bassBandAvg = 1;

  for (char c = 0; c < NUMBOLTSETS; c++)
    variabilityFactor[c] = 1.0;

}
void Effects::checkBassShake () {
  if (bassShakeCheck || bassShakeCounter != 0 || bassBendCounter != 0) {
    if (bassShakeType) {
      if (bassShakeCheck && bassShakeCounter <= 0) {
        bassShakeCounter = 1 + BASSSHAKEATTACKSPEED * bassShakeCounter / BASSSHAKEDUR;
        bassShakeDown = false;
        bassShakeCheck = false;
      }
      else if (bassShakeCounter > 0 && bassShakeCounter < BASSSHAKEMAX) {
        if (!bassShakeDown) bassShakeCounter += (1 + BASSSHAKEATTACKSPEED * bassShakeCounter / BASSSHAKEDUR);
        else bassShakeCounter -= bassShakeCounter / (BASSSHAKEENVDECAYLONGNESS * BASSSHAKEDUR) + 1;
      }
      else if (bassShakeCounter >= BASSSHAKEMAX) {
        if (colourPalatteTimeout <= 0) {
          colourPalatte = (colourPalatte + MINCOLOURPALATTEDISP + rand() % (256 - 2 * MINCOLOURPALATTEDISP)) % 256;
          colourPalatteTimeout = COLOURPALATTETIMEOUTDUR - COLOURPALATTETIMEOUTVAR / 2 + rand() % COLOURPALATTETIMEOUTVAR;
        }
        bassShakeCounter = BASSSHAKEMAX - (bassShakeCounter / (BASSSHAKEENVDECAYLONGNESS * BASSSHAKEDUR) + 1);
        bassShakeDown = true;
      }
      else if (!bassShakeCheck && bassShakeCounter <= 0)
        bassShakeCounter = 0;
    }
    else {
      if (bassShakeCheck && bassBendCounter <= 0) {
        bassBendCounter = 1 + BASSBENDATTACKSPEED * bassBendCounter / BASSBENDDUR;
        bassShakeDown = false;
        bassShakeCheck = false;
      }
      else if (bassBendCounter > 0 && bassBendCounter < BASSBENDMAX) {
        if (!bassShakeDown) bassBendCounter += (1 + BASSBENDATTACKSPEED * bassBendCounter / BASSBENDDUR);
        else bassBendCounter -= (bassBendCounter / (BASSBENDENVDECAYLONGNESS * BASSBENDDUR) + 1);
      }
      else if (bassBendCounter >= BASSBENDMAX) {
        bassBendCounter = BASSBENDMAX - (bassBendCounter / (BASSBENDENVDECAYLONGNESS * BASSBENDDUR) + 1);
        bassShakeDown = true;
      }
      else if (!bassShakeCheck && bassBendCounter <= 0)
        bassBendCounter = 0;
      //Serial.println(bassBendCounter);
    }
  }
}
void Effects::checkPixelSmash () {
  /*  static short c, px, str, tempIncr, i;
    //Serial.println(pixelSmashCounter);
    if (pixelSmashCheck) {
      if (!pixelSmashDown && pixelSmashCounter <= 0) {
        Serial.print("Start..");
        pixelSmashCounter = 1 + PIXELSMASHATTACKSPEED * pixelSmashCounter / PIXELSMASHDUR;
      }
      else if (pixelSmashCounter > 0 && pixelSmashCounter < PIXELSMASHMAX) {
        if (!pixelSmashDown)
          tempIncr = 1 + PIXELSMASHATTACKSPEED * pixelSmashCounter / PIXELSMASHDUR;
        else
          tempIncr = -(pixelSmashCounter / (PIXELSMASHENVDECAYLONGNESS * PIXELSMASHDUR) + 1);

        for (c = 0; c < 100; c += (100*abs(tempIncr)/(1 + PIXELSMASHATTACKSPEED * PIXELSMASHMAX / PIXELSMASHDUR))/4) { //abs(tempIncr); c++) {
         // i = 0;
          //do {
            str = random(0, NUMSTRIPS);
            px = (pixelSmashCounter + c/4)%STRIPLEN;//random(0, STRIPLEN);
           // i++;
            //if(i>0) Serial.println(i);
         // }
       //   while (pixelSmashGrid[str][px] == (bool)(tempIncr / abs(tempIncr) + 1) && i < 400);
          pixelSmashGrid[str][px] = (bool)(tempIncr / abs(tempIncr) + 1);
          //if(pixelSmashGrid[str][px]) strip[str][px] = CHSV(0,0,0);
        }
        pixelSmashCounter += tempIncr;
      }
      else if (pixelSmashCounter >= PIXELSMASHMAX) {
        pixelSmashCounter = PIXELSMASHMAX - (pixelSmashCounter / (PIXELSMASHENVDECAYLONGNESS * PIXELSMASHDUR) + 1);
        pixelSmashDown = true;
      }
      else if (pixelSmashDown && pixelSmashCounter <= 0) {
        pixelSmashDown = false;
        for (char i = 0; i < NUMSTRIPS; i++) {
          for (char c = 0; c < STRIPLEN; c++ ) {
            pixelSmashGrid[i][c] = false;
          }
        }
        Serial.println("done");
        pixelSmashCounter = 0;
        pixelSmashCheck = false;
      }
    }*/
}

void Effects::checkEffectLevels (float trebleAvg, float midAvg, float bassAvg) {
  if (RECHARGECUTOFF(variability, variabilityAvg)) {
    overrideLevel *= OVERRIDEDECAY;
    if (overrideLevel < trebleAvg) overrideLevel = trebleAvg;
    pixelSmashLevel *= PIXELSMASHDECAY;
    if (pixelSmashLevel < midAvg) pixelSmashLevel = midAvg;
    bassShakeLevel *= BASSSHAKEDECAY;
    if (bassShakeLevel < bassAvg) bassShakeLevel = bassAvg;
  }
}

float Effects::checkSongAvg (float sample) {
  prevSectionAvg = sectionAvg;
  sectionAvg = 0.8 * sectionAvg + 0.2 * sample;
  if (prevSectionAvg >= sectionAvg)
    variability = 0.97 * variability + 0.03 * (prevSectionAvg - sectionAvg);
  else
    variability = 0.97 * variability + 0.03 * (sectionAvg - prevSectionAvg);
  if ((variabilityAvg - variability) / variabilityAvg > VARAVGCUTOUT) {//after drop, to compensate for avg buildup in drop
    variabilityAvg = 0.2 * variabilityAvg + 0.8 * variability;
  }
  else
    variabilityAvg = (0.9994 - 0.0002) * variabilityAvg + 0.0006 * variability; //Depreciation to keep things exciting!

  variabilityFactor[0] = BASSADVANTAGE * (variability - variabilityAvg) / variabilityAvg;
  variabilityFactor[1] = LOWMIDADVANTAGE * (variability - variabilityAvg) / variabilityAvg;
  variabilityFactor[2] = HIGHMIDADVANTAGE * (variability - variabilityAvg) / variabilityAvg;
  variabilityFactor[3] = TREBLEADVANTAGE * (variability - variabilityAvg) / variabilityAvg;

  //eqBandsAvg = 0.999 * eqBandsAvg + 0.001 * sample;
  return prevSectionAvg - sectionAvg;
}

void Effects::checkCrazyMode (bool newBolt) {
  if (!startup && PIXELBLASTCUTOFF(variability, variabilityAvg) && newBolt) {
    crazyMode = true;
    if (!crazyCounterSquareRoot) {
      crazyCounterSquareRoot = CRAZYDECAY;
    }
  }
  else if (variability < variabilityAvg) crazyMode = false;
  if (crazyCounterSquareRoot > 0)  {
    crazyCounter = pow(crazyCounterSquareRoot--, 2);
  }
  else if (crazyCounter)
    crazyCounter = 0;
}


