#include "dice.h"

void Dice::roll_dice(std::vector<int> v, std::vector<int> v2, std::istream& in, int &die1, int &die2) {
    if (v2.empty()) {
        v2 = v;
    }
    if (testPermit) {
        in >> die1;
        if (in.fail()) {
            in.clear();
            in.ignore();
            std::shuffle(v.begin(), v.end(), rng);
            die1 = *(v.begin());
        }
        in >> die2;
        if (in.fail()) {
            in.clear();
            in.ignore();
            std::shuffle(v2.begin(), v2.end(), rng);
            die2 = *(v2.begin());
        }
    } else {
        std::shuffle(v.begin(), v.end(), rng);
        die1 = *(v.begin());
        std::shuffle(v2.begin(), v2.end(), rng);
        die2 = *(v2.begin());
    }
}

int Dice::get_cup() { return TimsCup; }

void Dice::add_cup() { TimsCup += 1; }

void Dice::sub_cup() { TimsCup -= 1; }
