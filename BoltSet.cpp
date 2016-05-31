#include "BoltSet.h"



BoltSet::BoltSet () {}

BoltSet::BoltSet (char newBoltPeriod, char newLowColour, char newHighColour, char newLowWidth, char newHighWidth, \
                  Biquad newFilter, short newAvgFiresPer10Sec, unsigned char newSetNum, unsigned int *quotaTotal, short *netQuota) {
  //newestBolt = NULL;
  newestBolt = -1;
  oldestBolt = -1;
  boltPeriod = newBoltPeriod;
  lowColour = newLowColour;
  highColour = newHighColour;
  lowWidth = newLowWidth;
  highWidth = newHighWidth;
  currentRevPos = 0;
  prevRevPos = 0;
  boltTrigger = newFilter;
  //*boltTrigger = newFilter;
  //boltTrigger = new Biquad(bq_type_lowpass, 10.0 / 78.125, 0.707, 6, 64);
  //boltTrigger = new Biquad();
  boltMagnitude = 0.0;
  triggerMagnitude = 0.0;
  newBoltTrigger = false;
  avgFiresPer10Sec = newAvgFiresPer10Sec;
  filterThresh = 4.0 / (float)avgFiresPer10Sec;
  maxMag = 1.0 / (float)avgFiresPer10Sec; //25% of threshold
  prevFire = millis();
  setNum = newSetNum;
  quota = 10 * avgFiresPer10Sec; //Just to get things started without causing a transient
  *quotaTotal += quota;
  *netQuota += avgFiresPer10Sec;
  overrideCheck = false;
  avgSignal = 1.0;
  stability = 1.0;
}

void BoltSet::callInit (char newBoltPeriod, char newLowColour, char newHighColour, char newLowWidth, char newHighWidth, \
                        Biquad newFilter, short newAvgFiresPer10Sec, unsigned char newSetNum, unsigned int *quotaTotal, short *netQuota) {
  BoltSet(newBoltPeriod, newLowColour, newHighColour, 
          newLowWidth, newHighWidth, newFilter, 
          newAvgFiresPer10Sec, newSetNum, 
          quotaTotal, netQuota);
}

float BoltSet::trackBoltFiring (float newSample, Effects *effects) {
  static float oldFilterThresh;
  stability = 0.99 * stability + 0.01 * abs(oldFilterThresh - filterThresh) / avgSignal;
  oldFilterThresh = filterThresh;
  boltMagnitude = boltTrigger.process(newSample);
  if (boltMagnitude < 0) boltMagnitude *= -1; //Rectify
  avgSignal = 0.99 * avgSignal + 0.01 * boltMagnitude;
  if (filterThresh < 0.1 || filterThresh > 100) filterThresh = 0.1;
  else if (millis() - prevFire > 4000) {
    filterThresh *= 0.01;
    prevFire = millis();
  }
  if (effects->crazyMode && (setNum == 4 || setNum == 4)) {
    //filterThresh = 0.1;
    //overrideCheck = true;
    //if (setNum == 4 || setNum == 4) overrideCheck = true;
  }
  /* Serial.print(boltMagnitude);
    Serial.print(" / ");
    Serial.println(filterThresh);
  */if (boltMagnitude > (1.0 - effects->variabilityFactor[setNum])*filterThresh && (setNum == 1 || setNum == 2 || (!effects->startup && BASSDROPCUTOFF(effects->variability, effects->variabilityAvg)))) {
    if (!effects->startup && setNum == 4 && boltMagnitude > filterThresh * (1.0 - 0.5 * effects->variabilityFactor[setNum]) * effects->overrideLevel && BASSDROPCUTOFF(effects->variability, effects->variabilityAvg)) {
      overrideCheck = true;
      effects->overrideLevel = boltMagnitude / filterThresh;
    }
    else if (!effects->startup && setNum == 2 && effects->pixelSmashCounter == 0 && boltMagnitude > filterThresh * (1.0 - effects->variabilityFactor[setNum]) * effects->pixelSmashLevel && BASSDROPCUTOFF(effects->variability, effects->variabilityAvg)) {
      effects->pixelSmashCheck = true;
      effects->pixelSmashLevel = boltMagnitude / filterThresh;
    }
    else if (!effects->startup && setNum == 1 && boltMagnitude > filterThresh * (1.0 - effects->variabilityFactor[setNum]) * effects->bassShakeLevel && effects->bassShakeCounter == 0 && effects->bassBendCounter == 0 && BASSDROPCUTOFF(effects->variability, effects->variabilityAvg)) {
      effects->bassShakeLag = BASSSHAKELAG * (1.0 - effects->bassShakeLevel / (boltMagnitude / filterThresh));
      effects->bassShakeCheck = true;
      if (BASSSHAKERATE) {
        effects->bassShakeType = 1;
        effects->bassShakeRatio = 0.95 * effects->bassShakeRatio + 0.05;
      }
      else {
        effects->bassShakeType = 0;
        effects->bassShakeRatio = 0.95 * effects->bassShakeRatio;
      }
      effects->bassShakeThresh *= (1.0 + (effects->bassShakeRatio - BASSSHAKERATIO));
      effects->bassShakeDir = (bool)rand()%2;
      effects->bassShakeLevel = boltMagnitude / (filterThresh * (1.0 + effects->bassShakeLag));
      if (effects->bassShakeLevel < 0.0) effects->bassShakeLevel = boltMagnitude / filterThresh;
      effects->colourPalatteTimeout--;
    }
    triggerMagnitude = boltMagnitude;
    newBoltTrigger = true;
  }
  if (setNum == 1) {
    effects->checkCrazyMode(newBoltTrigger);
    effects->bassBandAvg = 0.99 * effects->bassBandAvg + 0.01 * boltMagnitude;
  }
  else if (setNum == 4) {
    effects->checkSongAvg(abs(newSample));
  }
  return boltMagnitude;
}

bool BoltSet::checkIfAddNewBolt (float *fastBoltAdvantage, LEDRail **oldestRail, LEDRail (*rail)[NUMRAILS], unsigned char *prevSetNum, unsigned int *quotaTotal, unsigned char *numBolts, int *memForBolts, \
      short *netQuota, Effects *effects) {
  bool tempToken = false;
  if (newBoltTrigger) {
    maxMag *= 0.95; //Gradually reduce so bolts retain shape
    //Short-impulse feedback (shapes accentuations, adds sustain and release)
    filterThresh *= (1.2 - 0.2 * (float)avgFiresPer10Sec * (float)(millis() - prevFire) / 10000.0); //100% + half of over/undershoot percentage
    /*
        Seerial.print("Desired avg: ");
        Seerial.println(avgFiresPer10Sec);
        Seerial.print("actual avg: ");
        Seerial.println(10000.0 / (float)(millis() - prevFire));
    */
    *fastBoltAdvantage += 0.01 / (float)boltPeriod;

    //Seerial.println(oldestRail->getRailNum());
    //Check just in case another bolt size snagged the window, and ensure that it's not going to overtake a slower previous bolt
    short tempOldestTimeout = 0;
    *oldestRail = NULL;
    for (char c = 0; c < NUMRAILS; c++) {
      if ((*rail)[c].getTimeout() < tempOldestTimeout && (*rail)[c].checkIfValidPeriod(boltPeriod, fastBoltAdvantage)) {
        tempOldestTimeout = (*rail)[c].getTimeout();
        *oldestRail = &(*rail)[c];
      }
    }
    if (*oldestRail == NULL && overrideCheck) {
      *oldestRail = &(*rail)[rand()%(NUMRAILS - 1)];
      overrideCheck = false;
    }
    if (*oldestRail /*&& !checkQuota()*//* && setNum != prevSetNum*/) {
      LEDRail *currentRail = *oldestRail, *prevRail = NULL;
      /*while (!currentRail->checkIfValidPeriod(boltPeriod, fastBoltAdvantage)) {
        //Seerial.println(currentRail->checkIfValidPeriod(boltPeriod, fastBoltAdvantage));
        prevRail = currentRail;
        currentRail = prevRail->getNext();
        if (currentRail == NULL) break;
        }*/
      if (currentRail) {
        filterThresh *= (3.0 - 2.0 * (float)avgFiresPer10Sec * (float)(millis() - prevFire) / 10000.0); //100% + half of over/undershoot percentage
        if (filterThresh < 0.1 || filterThresh > 5000) filterThresh = 0.1;
        prevFire = millis();
        *prevSetNum = setNum;
        quota++; *quotaTotal++;
        triggerMagnitude -= filterThresh;
        if (triggerMagnitude < 0) triggerMagnitude = 0.0;
        if (triggerMagnitude > maxMag) maxMag = triggerMagnitude + 0.0001;
        char boltWidth, boltColour, railNum;
        /*Serial.print("Relative width: ");
          Serial.print(triggerMagnitude/maxMag);
          Serial.print("Threshold: ");
          Serial.println(filterThresh);
        */boltWidth = lowWidth + (highWidth - lowWidth) * (triggerMagnitude / maxMag);
        if (boltWidth > highWidth) {
          /*Serial.print("Trigmag: ");
            Serial.print(triggerMagnitude);
            Serial.print(" maxmag: ");
            Serial.println(maxMag);
          */
        }
        boltColour = (int)(lowColour + (highColour - lowColour) * (triggerMagnitude / maxMag)) % 256;
        railNum = currentRail->getRailNum();
        /*
          if (newestBolt) {
          Bolt* tempBolt = newestBolt;
          newestBolt = new Bolt(boltWidth, boltColour, railNum, tempBolt);
          //Seerial.println(boltColour);
          }
          else newestBolt = new Bolt(boltWidth, boltColour, railNum, NULL);
          currentRail->setRailTimeout((boltWidth + BOLTSPACING)*boltPeriod, boltPeriod); //NOTE: auto-sets rail as unavail
        */

        newestBolt = (newestBolt + 1 + 20) % 20;
        if (oldestBolt < 0) oldestBolt = 0;
        boltCache[newestBolt].callInit(boltWidth, boltColour, railNum, NULL, numBolts, memForBolts);
        //Seerial.print("New bolt: ");
        //Seerial.println(newestBolt);
        currentRail->setRailTimeout((boltWidth + BOLTSPACING)*boltPeriod, boltPeriod); //NOTE: auto-sets rail as unavail
        //Seerial.println(boltColour);

        // Reconfigure auto-queue
        if (prevRail) { //Remove mid-node, if oldest is not ready for bolt of this size
          prevRail->setNext(currentRail->getNext());
        }
        else { //If pulled out oldest rail, next rail becomes oldest
          //Seerial.println("noprevrail");
          *oldestRail = currentRail->getNext();
        }
        currentRail->setNext(NULL);
      }
      tempToken = true;
    }
    newBoltTrigger = false;
  }
  //Long-acting feedback ensures quotas are approximately met
  if (setNum == 1 || setNum == 2 || RECHARGECUTOFF(effects->variability, effects->variabilityAvg))
    filterThresh *= (0.8 + 0.1 * (float)((*netQuota) * quota + 1) / (float)(avgFiresPer10Sec * (*quotaTotal) + 1)); //gradually reduce thresh so attainable
  return tempToken;
}

void BoltSet::incrBolts (LEDRail (*rail)[NUMRAILS]) {
  /*
    Bolt *currentBolt = newestBolt, *prevBolt = NULL;
    while (currentBolt) {
      if (!(currentRevPos % boltPeriod)) {
        if (currentBolt->incrBolt()) {
          rail[currentBolt->getRailNum()].setBoltInRail(currentBolt->getBoltData(), currentBolt->getEffTail(), currentBolt->getMemWidth());
        }
        else {
          if (prevBolt) prevBolt->setNext(currentBolt->getNext());
          else newestBolt = NULL;
          delete currentBolt;
        }
      }
      else {
        rail[currentBolt->getRailNum()].setFringePixel(currentBolt->getHead(), CHSV(currentBolt->getColour(), 255, 255 * 5 * (currentRevPos % boltPeriod) / (8 * boltPeriod)));
      }
      prevBolt = currentBolt;
      currentBolt = prevBolt->getNext();
    }*/
  for (char c = 0; c < 20; c++) {
    static char pos;
    pos = (c + newestBolt + 1) % 20;
    if (boltCache[pos].en && !(currentRevPos % boltPeriod)) {
      prevRevPos = currentRevPos;
      if (boltCache[pos].incrBolt()) {
        (*rail)[boltCache[pos].getRailNum()].setBoltInRail(boltCache[pos].getColour(), boltCache[pos].getEffTail(), boltCache[pos].getMemWidth());
      }
      else if (boltCache[pos].getTail() >= RAILLEN) {
        boltCache[pos].killBolt();
        oldestBolt = (oldestBolt + 1 + 20) % 20;
      }
    }
    /*else if (boltCache[pos].en) {
        rail[boltCache[pos].getRailNum()].setBoltInRail(boltCache[pos].getColour(), boltCache[pos].getEffTail(), boltCache[pos].getMemWidth());
      }*/
  }
  currentRevPos ++;
}

char BoltSet::getBoltPeriod () {
  return boltPeriod;
}

char BoltSet::getDivider() {
  return boltTrigger.getDivider();
}

char BoltSet::getSetNum() {
  return setNum;
}

bool BoltSet::checkQuota() {
  /*Serial.print(quota);
    Serial.print("/");
    Serial.println(avgFiresPer10Sec);
  */
  if (quota > 10000) return true; //Make sure doesn't overflow
  return false;
}
void BoltSet::resetQuota() {
  quota /= 10;
}

float BoltSet::getAvg() {
  return avgSignal;
}

float BoltSet::getStability() {
  return stability;
}
