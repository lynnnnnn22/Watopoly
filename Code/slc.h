#ifndef __SLC_H__
#define __SLC_H__


#include <ctime>
#include <random>
#include <algorithm>
#include <memory>
#include <sstream>
#include "player.h"
#include "unownable.h"
#include "dice.h"


class SLC;


typedef std::shared_ptr<SLC> SLCPtr;


class SLC: public Unownable {
    Dice &dice;
    std::vector<int> v = {-3, -3, -3, -2, -2, -2, -2, -1, 
                          -1, -1, -1, 1, 1, 1, 2, 2,
                          2, 2, 3, 3, 3, 3, 4, 5};
    std::vector<int> v100;
public:
    // Constructor of SLC
    SLC(Dice &_dice);
    
    // const std::string getMovement(playerPtr) will return the instruction
    //   that the position or the steps that the player should move.
    virtual const std::string getMovement(Player *) noexcept;
};


#endif
