#ifndef __NEEDLESHALL_H__
#define __NEEDLESHALL_H__


#include <memory>
#include <random>
#include <algorithm>
#include <sstream>
#include "player.h"
#include "unownable.h"
#include "dice.h"


class NeedlesHall;


typedef std::shared_ptr<NeedlesHall> needPtr;


class NeedlesHall: public Unownable {
    Dice &dice;
    std::vector<int> v = {-200, -100, -100, 
                          -50, -50, -50,
                          25, 25, 25, 25, 25, 25, 
                          50, 50, 50, 
                          100, 100, 200};
    std::vector<int> v100;
    
public:
    // Constructor of NeedlesHall
    NeedlesHall(Dice &_dice);
    
    // const int getTuition(playerPtr) will return the amount
    //   of money that the player should pay.
    virtual const int getTuition(Player &p);
};

#endif
