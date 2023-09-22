#include "dctims.h"

DCTims::DCTims(Dice &_dice): Unownable("DC Tims Line", false), dice{_dice} {
    timeList.clear();
}


bool DCTims::getOutOfLine(Player &p, int &tuition, int &roll1, int &roll2) {
    int time = getTime(&p);

    if (time == -1) {
        tuition = 0;
        return false;
    }

    if (p.getCup() == 0) {
        std::cout << p.getName() << ": " << "You don't have any roll up the rim cup." << std::endl;
    } else {
        std::cout << p.getName() << ": " << "You have " << p.getCup() << " roll up the rim cup" << std::endl;
        std::cout << p.getName() << ": " << "Do you want to use one of it?" << std::endl;
        std::cout << p.getName() << ": " << "Type Y if you want, N otherwise." << std::endl;
        std::string choose = "N";
        while (getline(std::cin, choose)) {
            if (choose == "Y") {
                p.addCup(-1);
                dice.sub_cup();
                tuition = 0;
                return true;
            } else if (choose == "N") {
                break;
            } else {
                std::cout << p.getName() << ": " << "Invalid choose, please type again." << std::endl;
            }
        }
    }
    std::cout << p.getName() << "Do you want to pay $50 to leave?"  << std::endl;
    std::cout << p.getName() << ": " << "Type Y if you want, N otherwise." << std::endl;
    std::string choose = "N";
    while (getline(std::cin, choose)) {
        if (choose == "Y") {
            if (p.getCash() < 50) {
                std::cout << p.getName() << ": " << "You do not have enough money to get out of the line!" << std::endl;
                break;
            }
            tuition = 50;
            return true;
        } else if (choose == "N") {
            break;
        } else {
            std::cout << p.getName() << ": " << "Invalid choose, please type again." << std::endl;
        }
    }
    std::cout << p.getName() << ": " << "Please roll 2 dices:" << std::endl;
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
    std::vector<int> fake_vector;
    dice.roll_dice(playRoll, fake_vector, ss, roll1, roll2);
    if ((roll1 == -1) || (roll2 == -1)) {
        throw "Roll dice error";
    }
    std::cout << p.getName() << ": " << "The first roll is " << roll1 << std::endl;
    std::cout << p.getName() << ": " << "The second roll is " << roll2 << std::endl;
    if (roll1 == roll2) {
        std::cout << p.getName() << ": " << "You have rolled double!" << std::endl;
        tuition = 0;
        return true;
    } else {
        std::cout << p.getName() << ": " << "Sorry, you have rolled single!" << std::endl;
        lastRoll1 = roll1;
        lastRoll2 = roll2;
    }

    if (time == 0) {
        std::cout << p.getName() << ": " << "Sorry, you have to pay $50 and leave." << std::endl;
        tuition = 50;
        roll1 = lastRoll1;
        roll2 = lastRoll2;
        return true;
    }

    return false;
}


const int DCTims::getTuition(Player &p) { return 0; }


void DCTims::attachGuest(Player *p, bool isDC) noexcept {
    if (isDC) {
        guestList.emplace_back(p);
        timeList.emplace_back(3);
    } else {
        guestList.emplace_back(p);
        timeList.emplace_back(-1);
    }
}


void DCTims::removeGuest(Player &p) noexcept {
    for (int i = 0; i < (int)timeList.size(); i += 1) {
        if (guestList.at(i) == &p) {
            guestList[i] = nullptr;
            guestList.erase(guestList.begin() + i);
            timeList.erase(timeList.begin() + i);
            break;
        }
    }
}


void DCTims::setTime(Player *p, const int time) {
    for (int i = 0; i < (int)timeList.size(); i += 1) {
        if (guestList.at(i) == p) {
            timeList.at(i) = time;
            return;
        }
    }
}


const int DCTims::getTime(Player *p) {
    for (int i = 0; i < (int)timeList.size(); i += 1) {
        if (guestList.at(i) == p) {
            return timeList.at(i);
        }
    }
    return -1;
}


void DCTims::subTime(Player *p) {
    for (int i = 0; i < (int)timeList.size(); i += 1) {
        if (guestList.at(i) == p) {
            timeList.at(i) = timeList.at(i) - 1;
            return;
        }
    }
}
