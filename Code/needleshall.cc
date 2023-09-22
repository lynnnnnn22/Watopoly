#include "needleshall.h"

NeedlesHall::NeedlesHall(Dice &_dice): Unownable("Needles Hall", false), dice{_dice} {
    v100 = std::vector<int>(100);
    std::iota(v100.begin(), v100.end(), 0);  // v100 will become: [0..99]
}


const int NeedlesHall::getTuition(Player &p) {
    int tuition = -1;
    std::cout << p.getName() << ": " << "Please roll a magic dice to see how much you gain or loss:" << std::endl;
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
    int fake_dice = -1;
    dice.roll_dice(v, v100, ss, tuition, fake_dice);
    if (tuition == -1) {
        throw "Roll dice error";
    }
    if (dice.get_cup() != 4) {
        if (fake_dice == 0) {
            std::cout << p.getName() << ": " << "Congrats! You have obtained a Roll Up the Rim Cup!" << std::endl;
            dice.add_cup();
            p.addCup(1);
            std::cout << "There are currently " << dice.get_cup() << " active Roll Up the Rim Cups!"  << std::endl;
            return 0;
        }
    }
    return -1 * tuition;
}


