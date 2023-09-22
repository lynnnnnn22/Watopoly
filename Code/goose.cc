#include "goose.h"

Goose::Goose(): Unownable("Goose Nesting", false) { }


const int Goose::getTuition(Player &p) {
    std::cout << "Watch out! A flock of nesting geese are attacking you." << std::endl;
    return 0;
}
