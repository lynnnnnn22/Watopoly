#include "gym.h"

Gym::Gym(const std::string &name, const std::string &type, const int &purchaseCost, Dice &_dice)
         : Ownable(name, type, true, false, purchaseCost), dice{_dice} { }


const int Gym::getTotalCost(void) { return purchaseCost; }


const int Gym::getTuition(Player &p) {
    if (!getOwner()) { return 0; }
    if (checkMortgage()) { return 0; }
    int time = 4;
    for (int i = 0; i < (int)monopolyList.size(); i += 1) {
        if (monopolyList.at(i)->getOwner() == getOwner()) {
            time = 10;
        }
    }
    std::cout << p.getName() << ": " << "Please roll 2 dices to determine your usage fee:" << std::endl;
    std::string line;
    std::string cmd;
    std::istringstream ss{line};
    while (getline(std::cin, line)) {
        ss = std::istringstream{line};
        ss >> cmd;
        if (cmd == "roll") {
            break;
        } else {
            std::cout << p.getName() << ": " << "Please roll again!" << std::endl;
        }
    }
    int roll1 = -1;
    int roll2 = -1;
    int result = 0;
    std::vector<int> fake_vector;
    dice.roll_dice(playRoll, fake_vector, ss, roll1, roll2);
    if ((roll1 == -1) || (roll2 == -1)) {
        throw "Roll dice error";
    }
    std::cout << p.getName() << ": " << "The first roll is " << roll1 << std::endl;
    std::cout << p.getName() << ": " << "The second roll is " << roll2 << std::endl;
    result = roll1 + roll2;
    return result * time;
}


void Gym::improve(void) {
    std::cout << "Sorry, this building is not improvable." << std::endl;
}


void Gym::degenerate(void) {
    std::cout << "Sorry, this building is not degeneratable." << std::endl;
}


bool Gym::setImprovementLevel(const int level) { 
    if (level > 0) {
        std::cout << "Sorry, Gym is not improvable." << std::endl;
        return false;
    }
    improveLevel = level;
    return true;
}


const bool Gym::checkOtherImprove(void) { return false; }


Gym::~Gym() { }
