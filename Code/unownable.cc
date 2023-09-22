#include "unownable.h"


Unownable::Unownable(const std::string &name, const bool &movable):
    Property(name, "EVENT", false, movable) { }


const int Unownable::getTuition(Player &p) { return 0; }


const std::string Unownable::getMovement(Player *p) noexcept { return "0"; }


const int Unownable::getImproveLevel(void) { return 0; }


std::string Unownable::getGuest(void) {
    std::string guests = "";
    for (int i = 0; i < (int)guestList.size(); i += 1) {
        guests += (guestList[i]->getSymbol());
    }
    return guests;
}
