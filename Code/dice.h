#ifndef __ROLL_H__
#define __ROLL_H__

#include <iostream>
#include <random>
#include <algorithm>
#include <vector>

class Dice {
    int TimsCup = 0;
    bool testPermit = false;
    std::default_random_engine rng;
public:
    Dice(bool _testPermit, std::default_random_engine _rng): testPermit{_testPermit}, rng(_rng) {}

    // void roll_dice(std::vector<int> v, std::vector<int> v2, std::istream& in, int &die1, int &die2) will randomly generate an int
    //   from v with seed rng
    void roll_dice(std::vector<int> v, std::vector<int> v2, std::istream& in, int &die1, int &die2);

    // int get_cup() will return the current number of active Roll Up the Rims Cup
    int get_cup();

    // void add_cup() will increase the current number of active Roll Up the Rims Cup
    void add_cup();

    // void sub_cup() will decrease the current number of active Roll Up the Rims Cup
    void sub_cup();
};

#endif
