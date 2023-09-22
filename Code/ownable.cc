#include "ownable.h"


Ownable::Ownable(const std::string &name, const std::string &type,
                 const bool &ownable, const bool &improvable, const int &purchaseCost):
    Property(name, type, ownable, false), improvable(improvable), owner(nullptr),
    purchaseCost(purchaseCost) { }


const bool Ownable::getImproveable(void) { return improvable; }


void Ownable::setOwner(Player *_owner) { owner = _owner; }


Player *Ownable::getOwner(void) noexcept { return owner; }


std::string Ownable::getGuest(void) {
    std::string guests = "";
    for (int i = 0; i < (int)guestList.size(); i += 1) {
        guests += (guestList[i]->getSymbol());
    }
    return guests;
}


const int Ownable::getImproveLevel(void) { return improveLevel; }


const int Ownable::getPrice(void) { return purchaseCost; }


const std::string Ownable::getMovement(Player *p) noexcept { return "0"; }


const bool Ownable::checkMortgage(void) {
    return owner->checkMortgage(this);
}


Ownable::~Ownable() { }
