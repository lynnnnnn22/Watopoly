#include "tuition.h"


Tuition::Tuition(): Unownable("Tuition", false) { }


const int Tuition::getTuition(Player &p) {
    std::cout << "You should pay one of the following options:" << std::endl;
    std::cout << "1. Paying $300 tuition." << std::endl;
    std::cout << "2. 10% of your total worth." << std::endl;
    std::cout << "Type 1 if you want to pay $300, 2 if you want to pay 10% of your total worth." << std::endl;
    std::string line;
    int choose = 1;
    while (getline(std::cin, line)) {
        std::istringstream ss{line};
        ss >> choose;
        if (ss.fail()) {
            ss.ignore();
            ss.clear();
            std::cout << "That is not a valid choose, please choose again:" << std::endl;
            continue;
        } else if (choose != 1 && choose != 2) {
            std::cout << "That is not a valid choose, please choose again:" << std::endl;
            continue;
        } else {
            break;
        }
    }
    if (choose == 1) {
        std::cout << "Your choice of tuition is $300!" << std::endl;
        return 300;
    } else if (choose == 2) {
        std::cout << "Your choice of tuition is $" << p.getTotalWorth() * 0.1 << "!" << std::endl;
        return p.getTotalWorth() * 0.1;
    } else {
        std::cout << "The choose is invalid!" << std::endl;
        return 0;
    }
}
