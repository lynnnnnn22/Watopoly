#include "residence.h"


Residence::Residence(const std::string &name, const std::string &type, const int &purchaseCost): Ownable(name, type, true, false, purchaseCost) { }


const int Residence::getTotalCost(void) { return purchaseCost; }


const int Residence::getTuition(Player &p) {
    if (!getOwner()) { return 0; }
    if (checkMortgage()) { return 0; }
    int tuition = 25;
    for (int i = 0; i < (int)monopolyList.size(); i += 1) {
        if (monopolyList.at(i)->getOwner() == getOwner()) {
            tuition *= 2;
        }
    }
    return tuition;
}


void Residence::improve(void) {
    std::cout << "Sorry, this building is not improvable." << std::endl;
}


void Residence::degenerate(void) {
    std::cout << "Sorry, this building is not degeneratable." << std::endl;
}


bool Residence::setImprovementLevel(const int level) { 
    if (level > 0) {
        std::cout << "Sorry, Residence is not improvable." << std::endl;
        return false;
    }
    improveLevel = level;
    return true; 
}


const bool Residence::checkOtherImprove(void) { return false; }


Residence::~Residence() { }
