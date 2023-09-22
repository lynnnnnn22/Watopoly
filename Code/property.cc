#include "property.h"


Property::Property(const std::string &name, const std::string &type,
                   const bool &ownable, const bool &movable):
    movable(movable), ownable(ownable), next(nullptr), prev(nullptr),
    name(name), type(type) {
        guestList.clear();
        monopolyList.clear();
}


const bool Property::getMovable(void) const noexcept { return movable; }


const bool Property::getOwnable(void) const noexcept { return ownable; }


const std::string Property::getName(void) const noexcept { return name; }


const std::string Property::getType(void) const noexcept { return type; }


Player *Property::getOwner(void) noexcept { return nullptr; }


Property *Property::getNext(void) noexcept { return next; }


Property *Property::getPrev(void) noexcept { return prev; }


void Property::setNext(Property *_next) noexcept { next = _next; }


void Property::setPrev(Property *_prev) noexcept { prev = _prev; }


void Property::attachGuest(Player *p, bool isDC) noexcept {
    guestList.emplace_back(p);
}


void Property::attachMonopoly(Property *property) {
    monopolyList.emplace_back(property);
}

void Property::removeGuest(Player &guest) noexcept {
    for (int i = 0; i < (int)guestList.size(); i += 1) {
        if (guestList.at(i) == &guest) {
            guestList.erase(guestList.begin() + i);
        }
    }
}


bool Property::getOutOfLine(Player &p, int &tuition, int &roll1, int &roll2) { return false; }


void Property::setTime(Player *p, const int) { }


const int Property::getTime(Player *) { return -1; }


void Property::subTime(Player *) { return; }


void Property::destroy(void) {
    for (int i = 0; i < (int)guestList.size(); i += 1) {
        guestList[i] = nullptr;
    }
    guestList.clear();
    for (int i = 0; i < (int)monopolyList.size(); i += 1) {
        monopolyList[i] = nullptr;
    }
    monopolyList.clear();
    setNext(nullptr);
    setPrev(nullptr);
}


Property::~Property() { }
