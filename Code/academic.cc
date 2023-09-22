#include "academic.h"


static const int NO_COST = 0;
static const Player *BANK = nullptr;
static const int MINIMUM_LEVEL = 0;
static const int MAXIMUM_LEVEL = 6;


Academic::Academic(const std::string &name, const std::string &type,
                   const int &purchaseCost, const pl &payList):
    Ownable(name, type, true, true, purchaseCost) {
        this->payList.improveCost = payList.improveCost;
        this->payList.data.clear();
        for (int i = MINIMUM_LEVEL; i < MAXIMUM_LEVEL; i += 1) {
            this->payList.data.emplace_back(payList.data[i]);
        }
}


const int Academic::getTotalCost(void) {
    return getPrice() + getImproveLevel() * payList.improveCost;
}


const int Academic::getTuition(Player &p) {
    if (getOwner() == BANK) {
        return NO_COST;
    }
    if (checkMortgage()) { return NO_COST; }
    bool doubleTheTuition = true;
    for (int i = 0; i < (int)monopolyList.size(); i += 1) {
        if (monopolyList.at(i)->getOwner() != getOwner()) {
            doubleTheTuition = false;
            break;
        }
    }
    if (!improveLevel) {
        if (doubleTheTuition) {
            return 2 * payList.data[improveLevel];
        } else {
            return payList.data[improveLevel];
        }
    } else {
        return payList.data[improveLevel];
    }
}


void Academic::improve(void) {
    for (int i = 0; i < (int)monopolyList.size(); i += 1) {
        if (monopolyList.at(i)->getOwner() != getOwner()) {
            std::cout << "Improvement fails." << std::endl;
            std::cout << "Sorry, Some property of this monopoly set doesn't belong to you!" << std::endl;
            return;
        }
    }
    if (getImproveLevel() == -1) {
        std::cout << "Improvement fails." << std::endl;
        std::cout << "This property is already mortgaged." << std::endl;
    } else if (getImproveLevel() < MAXIMUM_LEVEL - 1) {
        int cost = payList.improveCost;
        if (cost > getOwner()->getCash()) {
            std::cout << "Improvement fails." << std::endl;
            std::cout << "Insufficient Cash." << std::endl;
        } else {
            getOwner()->pay(nullptr, payList.improveCost);
            improveLevel += 1;
        }
    } else {
        std::cout << "Improvement fails." << std::endl;
        std::cout << getName() << " Has already improved to the maximum level!" << std::endl;
    }
}


void Academic::degenerate(void) {
    for (int i = 0; i < (int)monopolyList.size(); i += 1) {
        if (monopolyList.at(i)->getOwner() != getOwner()) {
            std::cout << "Degenerate fails." << std::endl;
            std::cout << "Sorry, Some property of this monopoly set doesn't belong to you!" << std::endl;
            return;
        }
    }
    if (getImproveLevel() == -1) {
        std::cout << "Degenerate fails." << std::endl;
        std::cout << "This property is already mortgaged." << std::endl;
    } else if (getImproveLevel() > MINIMUM_LEVEL) {
        getOwner()->pay(nullptr, -1 * (payList.improveCost / 2));
        improveLevel -= 1;
    } else {
        std::cout << "Degenerate fails." << std::endl;
        std::cout << getName() << " Has already degenerated to the minimum level!" << std::endl;
    }
}


bool Academic::setImprovementLevel(const int _improveLevel) {
    if (_improveLevel < MAXIMUM_LEVEL) {
        improveLevel = _improveLevel;
        return true;
    } else {
        std::cout << "Improvement fails." << std::endl;
        std::cout << getName() << " Has already improved into the maximum level!" << std::endl;
        return false;
    }
}


const bool Academic::checkOtherImprove(void) {
    for (int i = 0; i < (int)monopolyList.size(); i += 1) {
        if (monopolyList.at(i)->getOwner() != getOwner()) {
            return false;
        } else if (monopolyList.at(i)->getImproveLevel() > 0) {
            return true;
        }
    }
    return false;
}


Academic::~Academic() { }
