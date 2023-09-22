#include <string>
#include <fstream>
#include <iostream>
#include "controller.h"


int main(int argc, char *argv[]) {
    std::string fileName;
    bool testPermit = false;
    bool loadPermit = false;
    bool extendPermit = false;
    unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
    try {
        for (int i = 1; i < argc; i += 1) {
            std::string command{argv[i]};
            if (command == "-testing") {
                testPermit = true;
                std::cout << "Test mode is now initialized!" << std::endl;
            } else if (command == "-load") {
                if (loadPermit) { throw "Loading File exists."; }
                fileName = argv[++i];
                loadPermit = true;
            } else if (command == "-seed") {
                try {
                    seed = std::stoi(std::string{argv[++i]});
                } catch( std::invalid_argument & e ) {
                    std::cerr << e.what() << std::endl;
                } catch( std::out_of_range & e ) {
                    std::cerr << e.what() << std::endl;
                    return -1;
                }
            } else if (command == "-extend") {
                extendPermit = true;
                std::cout << "Extension mode is now initialized!" << std::endl;
            } else {
                throw "Invalid argument exists.";
            }
        }
    } catch (const std::string &error) {
        std::cerr << error << std::endl;
        return 1;
    }
    Controller controller{};
    controller.GameSeed(seed);
    try {
        if (testPermit) { controller.GameTest(); }
        if (extendPermit) { controller.GameExtend(); }
        if (loadPermit) {
            std::cout << "The game will load data from " << fileName << std::endl;
            std::ifstream loadingFile{fileName};
            if (loadingFile.fail()) {
                std::cerr << "Loading file does not exist or is not readable!" << std::endl;
                return 1;
            }
            std::cout << "Load mode is now initialized!" << std::endl;
            controller.GameLoad(loadingFile);
            controller.GamePrint();
        } else {
            controller.GameInit();
            controller.GamePrint();
        }
    } catch(const std::string &e) {
        std::cerr << e << std::endl;
    }
    controller.GamePlay();
    controller.controllerClean();
    std::cout << "Watopoly Game is over!" << std::endl;
    std::cout << "Thank you for playing!" << std::endl;
}
