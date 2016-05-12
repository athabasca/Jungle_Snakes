#include "Bolt.h"

Bolt::Bolt () {
  width = 0;
  en = false;
}

Bolt::Bolt (char newWidth, char newColour, char newRailNum, Bolt* newestBolt, unsigned char *newNumBolts, int *memForBolts) {
  numBolts = newNumBolts;
  *memForBolts -= width * sizeof(CRGB);
  head = -1;
  width = newWidth;
  *memForBolts += width * sizeof(CRGB);
  memStart = width;
  memWidth = 0;
  railNum = newRailNum;
  tail = head - width + 1;
  effTail = -1;
  colour = newColour;
  wiggle = 0;
  //boltData = new CRGB[width]();
  // for (char i = 0; i < width; i++) { //NOTE it starts at tail cause it's copying into mem starting at tail
  //   boltData[i] = CHSV(colour, 255, 255 * (width - abs(width - 8 * i / 5)) / width);
  // }
  //next = newestBolt;
  *numBolts++;
  //Seerial.println(numBolts);
  en = true;
}

void Bolt::callInit (char newWidth, char newColour, char newRailNum, Bolt* newestBolt, unsigned char *newNumBolts, int *memForBolts) {
  numBolts = newNumBolts;
  *memForBolts -= width * sizeof(CRGB);
  head = -1;
  width = newWidth;
  *memForBolts += width * sizeof(CRGB);
  memStart = width;
  memWidth = 0;
  railNum = newRailNum;
  tail = head - width + 1;
  effTail = -1;
  colour = newColour;
  wiggle = 0;
  //boltData = new CRGB[width]();
  //for (char i = 0; i < width; i++) { //NOTE it starts at tail cause it's copying into mem starting at tail
  //  boltData[i] = CHSV(colour, 255, 255 * (width - abs(width - 8 * i / 5)) / width);
  //}
  //next = newestBolt;
  *numBolts++;
  //Seerial.println(numBolts);
  en = true;
}

Bolt::~Bolt () {
  //Seerial.println((int)numBolts);
  //TODO: replicate functionality of destructor without requiring a pointer member variable in Bolt class
  *numBolts--;
}
bool Bolt::incrBolt () {
  head++;
  tail++;
  if (tail < 0) {
    effTail = 0;
    memStart = effTail - tail;
  }
  else if (tail >= RAILLEN)
    return false;
  else {
    effTail = tail;
    memStart = 0;
  }
  if (head < RAILLEN) {
    memWidth = head - effTail + 1;
  }
  else {
    memWidth = (RAILLEN - 1) - effTail + 1;
  }
  return true;
}
void Bolt::setHead (short newHead) {
  head = newHead;
}
short Bolt::getHead () {
  return head;
}

short Bolt::getTail () {
  return tail;
}

void Bolt::setWidth (char newWidth) {
  width = newWidth;
}
char Bolt::getWidth () {
  return width;
}

void Bolt::setColour (char newColour) {
  colour = newColour;
}
char Bolt::getColour () {
  return colour;
}

short Bolt::getMemWidth () {
  return memWidth;// * sizeof(CRGB);
}

/*CRGB * Bolt::getBoltData () {
  return &(boltData[memStart]);
  }*/

void Bolt::killBolt () {
  en = false;
  //memForBolts -= width * sizeof(CRGB);
  width = 0;
  // boltData = NULL;
}

short Bolt::getEffTail () {
  return effTail;
}

char Bolt::getRailNum () {
  return railNum;
}
/*
  Bolt * Bolt::getNext() {
  return next;
  }

  void Bolt::setNext(Bolt * newNext) {
  next = newNext;
  }
*/
char Bolt::getWiggle() {
  wiggle = (wiggle + 1) % 64;
  return wiggle / 8;
}
