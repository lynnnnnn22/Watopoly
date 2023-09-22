#include "slc.h"


SLC::SLC(Dice &_dice): Unownable("SLC", true), dice{_dice} {
    v100 = std::vector<int>(100);
    std::iota(v100.begin(), v100.end(), 0);  // v100 will become: [0..99]
}


const std::string SLC::getMovement(Player *p) noexcept {
    std::cout << p->getName() << ": " << "Please roll a magic dice to be sent to your destination:" << std::endl;
    std::string line;
    std::string cmd;
    std::istringstream ss{line};
    while (getline(std::cin, line)) {
        ss = std::istringstream{line};
        ss >> cmd;
        if (cmd == "roll") {
            break;
        } else {
            std::cout << p->getName() << ": " << "Please roll again!" << std::endl;
        }
    }
    int decision = -4;
    int fake_dice = -1;
    dice.roll_dice(v, v100, ss, decision, fake_dice);
    if (decision == -4) {
        throw "Roll dice error!";
    }
    if (dice.get_cup() != 4) {
        if (fake_dice == 0) {
            std::cout << p->getName() << ": " << "Congrats! You have obtained a Roll Up the Rim Cup!" << std::endl;
            dice.add_cup();
            p->addCup(1);
            std::cout << "There are currently " << dice.get_cup() << " active Roll Up the Rim Cups!"  << std::endl;
            return "0";
        }
    }
    if (decision == -3) { return "-3"; }
    else if (decision == -2) { return "-2"; }
    else if (decision == -1) { return "-1"; }
    else if (decision == 1) { return "1"; }
    else if (decision == 2) { return "2"; }
    else if (decision == 3) { return "3"; }
    else if (decision == 4) { return "DC Tims Line"; }
    else { return "Collect OSAP"; }
}
