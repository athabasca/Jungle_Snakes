#include "LEDRail.h"

LEDRail::LEDRail () {}

LEDRail::LEDRail (char newStripNum, char newRailNum, CRGB (*newStrip)[NUMSTRIPS][STRIPLEN], Effects *newEffects) {
  stripNum = newStripNum;
  railNum = newRailNum;
  railTimeout = 0;
  recentRailPeriod = 0;
  railAvail = false;
  next = NULL;
  strip = newStrip;
  effects = newEffects;
  if (!(railNum % 2)) flipStrip = false;
  else flipStrip = true;
}

void LEDRail::callInit (char newStripNum, char newRailNum, CRGB (*newStrip)[NUMSTRIPS][STRIPLEN], Effects *newEffects) {
  stripNum = newStripNum;
  railNum = newRailNum;
  railTimeout = 0;
  recentRailPeriod = 0;
  railAvail = false;
  next = NULL;
  strip = newStrip;
  effects = newEffects;
  if (!(railNum % 2)) flipStrip = false;
  else flipStrip = true;
}

bool LEDRail::incrRail () {
  railTimeout--;
  if (railTimeout <= 0 && !railAvail) {
    railAvail = true;
    return true;
  }
  else return false;
}

bool LEDRail::checkRailAvail () {
  return railAvail;
}
void LEDRail::setRailAvail (bool newRailAvail) {
  railAvail = newRailAvail;
}

void LEDRail::setRailTimeout (short newRailTimeout, char newRecentRailPeriod) {
  railTimeout = newRailTimeout;
  recentRailPeriod = newRecentRailPeriod;
  railAvail = false;
}

void LEDRail::setBoltInRail (char memColour, char memStart, char memWidth) {
  //memcpy(&((*strip)[memStart]), memData, memWidth);
  //if (memStart > 0) (*strip)[stripNum][memStart - 1] = CHSV(0, 0, 0);
  for (char c = 0; c < memWidth; c++) {
    if (memStart + c - (*effects).bassBendCounter >= 0) {
      if (!flipStrip)
        (*strip)[stripNum][memStart + c - (*effects).bassBendCounter] = CHSV((256 + memColour + (*effects).colourPalatte + ((*effects).bassShakeDir * (*effects).bassShakeCounter) / 2) % 256, 255 - (*effects).bassShakeCounter, 255 * (memWidth - abs(memWidth - 8 * c / 5)) / memWidth);
      else
        (*strip)[stripNum][(STRIPLEN - 1) - (memStart + c - (*effects).bassBendCounter)] = CHSV((256 + memColour + (*effects).colourPalatte + ((*effects).bassShakeDir * (*effects).bassShakeCounter) / 2) % 256, 255 - (*effects).bassShakeCounter, 255 * (memWidth - abs(memWidth - 8 * c / 5)) / memWidth);
    }
    else if (PIXELOUT) { //Twinkle-out effect!
      if (!flipStrip)
        (*strip)[stripNum][RAILLEN + (memStart + c - (*effects).bassBendCounter)] = CHSV(0, 0, 0);
      else
        (*strip)[stripNum][(RAILLEN - 1) - (memStart + c - (*effects).bassBendCounter)] = CHSV(0, 0, 0);
    }
    /*if ((*effects).pixelSmashCounter) {
      if (!flipStrip && (*effects).pixelSmashGrid[stripNum][memStart + c])
        (*strip)[stripNum][memStart + c] = CHSV(0, 0, 0);
      else if (flipStrip && (*effects).pixelSmashGrid[stripNum][(STRIPLEN - 1) - (memStart + c)])
        (*strip)[stripNum][(STRIPLEN - 1) - (memStart + c)] = CHSV(0, 0, 0);
    }*/
  }
}

void LEDRail::showRail () {
  FastLED.show();
  /*for(short i = 0; i < STRIPLEN; i++) {
    (*strip)[i] = CRGB(0,0,0);
    }*/
}

bool LEDRail::checkIfValidPeriod (char newBoltPeriod, float *fastBoltAdvantage) {
  //Incorporate reward function to help faster bolts gain traction
  if (railAvail && recentRailPeriod > newBoltPeriod && \
      ((int)newBoltPeriod - (int)recentRailPeriod)*RAILLEN / (int)recentRailPeriod > railTimeout) {
    *fastBoltAdvantage -= 0.1 * (float)(recentRailPeriod - newBoltPeriod);
    return true;
  }
  else if (railAvail && recentRailPeriod <= newBoltPeriod && \
           ((float)newBoltPeriod - (float)recentRailPeriod - *fastBoltAdvantage) * (float)RAILLEN / ((float)recentRailPeriod + *fastBoltAdvantage) > railTimeout) {
    return true;
  }
  else {
    railAvail = false;
    return false;
  }
}

void LEDRail::setFringePixel(char currentHead, CRGB pixelColour) {
  //if (currentHead < STRIPLEN - 1)
  // (*strip)[currentHead + 1] = pixelColour;
}

LEDRail *LEDRail::getNext () {
  return next;
}
void LEDRail::setNext (LEDRail *newNext) {
  next = newNext;
}
char LEDRail::getRailNum() {
  return railNum;
}

short LEDRail::getTimeout() {
  return railTimeout;
}

char LEDRail::checkPrevRailPer() {
  return recentRailPeriod;
}
