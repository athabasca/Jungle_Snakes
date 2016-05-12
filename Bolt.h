#ifndef Bolt_H
#define Bolt_H
#include "JungleDefines.h"
#include <FastLED.h>

class Bolt {
  private:
    short head;
    short tail;
    char width;
    char colour;
    char wiggle;

    char railNum;
    short memStart;
    short memWidth;
    short effTail;
	
 unsigned	char *numBolts;

    //Bolt* next;

    //CRGB * boltData;
  public:
    bool en;
    Bolt ();
    Bolt (char newWidth, char newColour, char newRailNum, Bolt* newestBolt, unsigned char *newNumBolts, int *memForBolts);
    void callInit (char newWidth, char newColour, char newRailNum, Bolt* newestBolt, unsigned char *newNumBolts, int *memForBolts);
    ~Bolt();
    bool incrBolt ();
    void setHead (short newHead);
    short getHead ();
    short getTail ();
    void setWidth (char newWidth);
    char getWidth ();
    void setColour (char newColour);
    char getColour ();
    short getMemWidth ();
    //CRGB * getBoltData ();
    void killBolt ();
    short getEffTail ();
    char getRailNum ();
    char getWiggle();
    //Bolt * getNext();
    //void setNext(Bolt * newNext);
};

#endif
