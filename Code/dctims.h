#ifndef __DCTIMS_H__
#define __DCTIMS_H__


#include <memory>
#include <ctime>
#include <random>
#include <algorithm>
#include <sstream>
#include "player.h"
#include "unownable.h"
#include "dice.h"


class DCTims;


typedef std::shared_ptr<DCTims> DCTPtr;



class DCTims: public Unownable {
    std::vector<int> timeList;
    Dice &dice;
    std::vector<int> playRoll = {1, 2, 3, 4, 5, 6};
    int lastRoll1 = 0;
    int lastRoll2 = 0;
public:
    // Constructor of SLC
    DCTims(Dice &_dice);
    
    // bool getOutOfLine(Player &p, int &tuition) will attempt to get
    //   the player out of Tims Line, return true if the player could
    //   get out of line, false otherwise
    bool getOutOfLine(Player &p, int &tuition, int &roll1, int &roll2) override;

    // const int getTuition(playerPtr) will return the amount
    //   of money that the player should pay.
    virtual const int getTuition(Player &p) override;
    
    // void attachGuest(playerPtr) will set the given
    //   player as a guest.
    virtual void attachGuest(Player *p, bool isDC = false) noexcept override;
    
    // void removeGuest(playerPtr) will remove the
    //   matched player from the guest list.
    virtual void removeGuest(Player &) noexcept override;
    
    // void setTime(Player *p, const int) will set the time of the DC
    //   Tims stuck event
    void setTime(Player *p, const int) override;

    // const int getTime(playerPtr) will provide the time of
    //   the player should still stay in the DC.
    virtual const int getTime(Player *) override;
    
    // void subTime(playerPtr) will substract the
    //   remaining time of the player.
    virtual void subTime(Player *) override;
};

#endif
