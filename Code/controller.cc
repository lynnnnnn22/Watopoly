#include "controller.h"


// ================================================================================
// Static fields

static const int START_POSITION = 0;
static const int MINIMUM_PLAYER = 2;
static const int INVALID_POSITION = -1;
// ================================================================================
// Public methods of Controller

Controller::Controller() {
        std::cout << "Watopoly Game is initialized ..." << std::endl;
        playerList.clear();
}


void Controller::GameInit(void) {
    dice = Dice{testPermit, rng};
    grid = std::make_shared<Grid>(dice);
    int playNum = 0, startingPos = START_POSITION;
    std::cout << "Please input the number of players in this game: " << std::endl;
    std::string line;
    while (getline(std::cin, line)) {
        std::istringstream ss{line};
        ss >> playNum;
        if (ss.fail()) {
            std::cout << "That is not a valid input, please input again:" << std::endl;
            ss.ignore();
            ss.clear();
        } else {
            if (MINIMUM_PLAYER <= playNum &&
                playNum <= 7) {
                break;
            } else {
                std::cout << "That is not a valid input, please input again:" << std::endl;
            }
        }
        std::cout << "Please input the number of the player in this game: " << std::endl;
    }
    if (std::cin.eof()) { return; }

    std::cout << "Hello players!" << std::endl;
    std::map<char, std::string> characterList = listInitializer();

    for (int i = 0; i < playNum; ++i) {
        std::string name;
        std::string symbol;
        std::cout << "Player " << i + 1 << " please enter your name:" << std::endl;
        if (!getline(std::cin, name)) { throw "Input Error"; }
        std::cout << "Hello " << name << "!" << std::endl;
        std::cout << "Note that we have the following pieces you can choose from:" << std::endl;
        for (auto& it : characterList) {
            std::cout << "- " << it.first << ": " << it.second <<std::endl;
        }
        std::cout << "Please enter the symbol character you want to play:" << std::endl;
        if (!getline(std::cin, symbol)) { throw "Input Error"; }
        while ((symbol.length() != 1) || (characterList.find(symbol[0]) == characterList.end())) {
            std::cout << "That is not a valid symbol!" << std::endl;
            std::cout << "Please input again:" << std::endl;
            if (!getline(std::cin, symbol)) { throw "Input Error"; }
        }
        playerList.emplace_back(playerPtr(new Player{ grid->at(startingPos).get(), symbol[0], name}));
        characterList.erase(symbol[0]);
    }

    for (int i = 0; i < (int)playerList.size(); i += 1) {
        std::cout << "Player: " << playerList[i]->getName() << "(" << playerList[i]->getSymbol() << ")";
        std::cout << " has been located at " << grid->at(startingPos)->getName()<< std::endl;
        grid->at(startingPos)->attachGuest(playerList[i].get());
    }
    grid->gridAttach();
}


void Controller::GamePlay(void) {
    if ((int)playerList.size() < 2 || (int)playerList.size() > 7) {
        std::cout << "Player number invalid, should be 2-7." << std::endl;
	    return;
    }

    std::cout << "Watopoly Game starts!" << std::endl;
    int currDay = 0;
    std::vector<int> playRoll = {1, 2, 3, 4, 5, 6};

    while (true) {
	    playerPtr currPlayer = playerList[currPlayerNum];
        currPlayer->moveReset();
        int rollDouble = 0;
        int roll1 = -1;
        int roll2 = -1;
        int money;
        std::string trader, filename, give, receive, property, decision;

        if (extendPermit) {
            if ((currPlayerNum == 0) && (!currPlayer->moneyOwed)) {
                currDay += 1;
                std::cout << "It's Day " << currDay << " now!" << std::endl;
            }
            #if 0
            if ((currDay + 1) % 10 == 0) {
                for (playerPtr p : playerList) {
                    if (!currPlayer->getMovable()) {
                        
                    }
                }
                currPlayerNum = 0;
                continue;
            }
            #endif
        }

        if (currPlayer->getLanding()->getName() == "DC Tims Line") {
            int time = currPlayer->getLanding()->getTime(currPlayer.get());
            if (time >= 0) {
                std::cout << currPlayer->getName() << 
                    " is waiting at DC Tims Line! " << time << " day remaining!" << std::endl;
                if (currPlayer->makeAction(roll1, roll2)) {
                    GamePrint();
                    std::cout << "Auction begins!" << std::endl;
                    playerPtr auctionWinner = auction();
                    auctionWinner->get((Ownable *)currPlayer->getLanding());
                } else {
                    GamePrint();
                }
            }
        }

        while (true) {
            std::string line;
            std::istringstream ss{line};
            std::string cmd;
            if (currPlayer->moneyOwed) {
                currPlayer->lastPropertyOwner = currPlayer->getLanding()->getOwner();
                if (currPlayer->moneyOwed <= currPlayer->getCash()) {
                    std::cout << currPlayer->getName() << ": " << "You have obtained enough money! your debt will be paid now!" << std::endl;
                    currPlayer->pay(currPlayer->lastPropertyOwner, currPlayer->moneyOwed);
                    currPlayer->moneyOwed = 0;
                    if ((roll1 != -1) && (roll2 != -1)) {
                        currPlayer->move(roll1 + roll2);
                        GamePrint();
                        roll1 = -1;
                        roll2 = -1;
                        if (currPlayer->makeAction(roll1, roll2)) {
                            std::cout << "Auction begins!" << std::endl;
                            playerPtr auctionWinner = auction();
                            auctionWinner->get((Ownable *)currPlayer->getLanding());
                        }
                        GamePrint();
                    }
                    if ((rollDouble > 0) && (rollDouble < 3)) {
                        roll(currPlayer, rollDouble, ss, roll1, roll2);
                    }
                } else {
                    std::cout << currPlayer->getName() << ": " << "You don't have enough money! ";
                    std::cout << currPlayer->getName() << ": " << "You can now choose bankrupt, or try your best to raise enough money!" << std::endl;
                    std::cout << "i.e. by selling improvements and mortgage everything." << std::endl;
                }
            }
            
            std::cout << currPlayer->getName() << " please enter your command:" << std::endl;
            if (!getline(std::cin, line)) {
                std::cerr << "Command expected" << std::endl;
                return;
            }
            ss = std::istringstream{line};
            ss >> cmd;
            if (!currPlayer->moneyOwed) {
                if (cmd == "roll") {
                    if (!currPlayer->getMovable()) {
                        std::cout << currPlayer->getName() << " cannot move!" << std::endl;
                        if (currPlayer->getLanding()->getName() == "DC Tims Line") {
                            int time = currPlayer->getLanding()->getTime(currPlayer.get());
                            if (time > 0) {
                                std::cout << currPlayer->getName() << 
                                    " is waiting at DC Tims Line! " << time << " days remaining!" << std::endl;
                            }
                        }
                    } else {
                        roll(currPlayer, rollDouble, ss, roll1, roll2);
                    }
                } else if (cmd == "next") {
                    if (!currPlayer->movalready) { 
                        std::cout << currPlayer->getName() << ": "<< "You can still roll! Cannot give control to next player yet!" << std::endl;
                        continue;
                    }
                    GamePrint();
                    break;
                }
            }
            if (cmd == "trade") {
                ss >> trader >> give >> receive;
                if (ss.fail()) { 
                    std::cout << currPlayer->getName() << ": " << "Invalid input, please try again." << std::endl;
                    continue; 
                }
                Player *traderPtr = findPlayer(trader);
                if (!traderPtr) { 
                    std::cout << currPlayer->getName() << ": " << "Player " << trader << " not found, please try again." << std::endl;
                    continue; 
                }
                if (trader == currPlayer->getName()) {
                    std::cout << currPlayer->getName() << ": " << "You cannot trade with yourself." << std::endl;
                    continue;
                }
                if (isNum(give[0] && isNum(receive[0]))) { 
                    std::cout << currPlayer->getName() << ": " << "You cannot give money in return for money." << std::endl;
                    continue; }
                try {
                    if (isNum(give[0])) {
                        money = std::stoi(give);
                    } else if (isNum(receive[0])) {
                        money = std::stoi(receive);
                    }
                } catch (std::invalid_argument &e) {
                    continue;
                }
                if (isNum(give[0])) {
                    currPlayer->tradeWith(traderPtr, money, receive);
                } else if (isNum(receive[0])) {
                    currPlayer->tradeWith(traderPtr, give, money);
                } else {
                    currPlayer->tradeWith(traderPtr, give, receive);
                }
                GamePrint();
            } else if (cmd == "improve") {
                ss >> property >> decision;
                if (ss.fail()) { 
                    std::cout << currPlayer->getName() << ": " << "Invalid input, please try again." << std::endl;
                    continue; 
                }
                if (decision == "buy") {
                    currPlayer->improve(property);
                } else if (decision == "sell") {
                    currPlayer->degenerate(property);
                } else {
                    std::cout << currPlayer->getName() << ": " << "Invalid input, please try again." << std::endl;
                }
                GamePrint();
            } else if (cmd == "mortgage") {
                ss >> property;
                if (ss.fail()) { 
                    std::cout << currPlayer->getName() << ": " << "Invalid input, please try again." << std::endl;
                    continue; 
                }
                currPlayer->mortgage(property);
                GamePrint();
            } else if (cmd == "unmortgage") {
                ss >> property;
                if (ss.fail()) { 
                    std::cout << currPlayer->getName() << ": " << "Invalid input, please try again." << std::endl;
                    continue; 
                }
                currPlayer->unmortgage(property);
                GamePrint();
            } else if (cmd == "assets") {
                currPlayer->showAssets();
                GamePrint();
            } else if (cmd == "bankrupt") {
                if (currPlayer->moneyOwed) {
                    std::cout << "Player " << currPlayer->getName() << " dropped out of school!" << std::endl;
                    if (currPlayer->lastPropertyOwner == nullptr) { // auction player's properties
                        std::cout << currPlayer->getName() << " owes money to the bank." << std::endl;
                        currPlayer->showAssets();
                        for (int i = 0; i < currPlayer->getCup(); ++i) {
                            dice.sub_cup();
                        }
                        std::cout << "All " << currPlayer->getName() << "'s Roll Up the Rim Cups has been destroyed!" << std::endl;
                        std::cout << "Auction begins!" << std::endl;
                        std::cout << "Auction winner will receive all " << currPlayer->getName() << "'s properties as unmortgaged." << std::endl;
                        playerPtr auctionWinner = auction(currPlayer);
                        for (Ownable *property : currPlayer->getOwned()) {
                            property->setImprovementLevel(0);
                            property->setOwner(auctionWinner.get());
                            auctionWinner->receiveOwned(property);
                        }
                        std::cout << auctionWinner->getName() << " obtained all " << currPlayer->getName() << "'s properties." << std::endl;
                        auctionWinner->showAssets();
                    } else {
                        std::cout << currPlayer->getName() << " owes money to " << 
                            currPlayer->lastPropertyOwner->getName() << "." << std::endl;
                        Player *lastPropertyOwner = currPlayer->lastPropertyOwner;
                        currPlayer->turnPrivate(currPlayer->lastPropertyOwner);
                        std::cout << lastPropertyOwner->getName() << " obtained all " << currPlayer->getName() << "'s properties." << std::endl;
                        lastPropertyOwner->showAssets();
                    }
                    currPlayer->getLanding()->removeGuest(*currPlayer);
                    playerList.erase(playerList.begin() + currPlayerNum);
                    currPlayerNum -= 1;
                    if (currPlayerNum >= (int)playerList.size()) {
                        currPlayerNum = playerList.size();
                    }
                    break;
                } else {
                    std::cout << currPlayer->getName() << ": " << "You cannot not be bankruptted." << std::endl;
                }
                GamePrint();
            } else if (cmd == "all") {
                std::cout << "Now, we have the current situation:" << std::endl;
                for (int i = 0; i < (int)playerList.size(); i += 1) {
                    playerList[i]->showAssets();
                }
                GamePrint();
            } else if (cmd == "save") {
                try {
                    ss >> filename;
                    if (ss.fail()) { throw ""; }
                } catch (const std::string &) {
                    ss.clear();
                    ss.ignore();
                    filename = "gameData.txt";
                }
                std::ofstream saveFile{filename};
                if (saveFile.fail()) { throw "saveFile error"; }
                GameSave(saveFile);
                GamePrint();
            } else if (cmd == "quit") { return; }
        }

        if (checkWin()) {
            std::cout << "We have a WINNER: " << playerList[0]->getName() << std::endl;
            std::cout << "Congratulations!" << std::endl;
            break;
        }
        nextPlayer(currPlayerNum);
    }
}


void Controller::roll(playerPtr currPlayer, int &rollDouble, std::istringstream& ss, int &roll1, int &roll2) {
    std::vector<int> playRoll = {1, 2, 3, 4, 5, 6};
    while (rollDouble < 3) {
        std::vector<int> fake_vector;
        dice.roll_dice(playRoll, fake_vector, ss, roll1, roll2);
        if ((roll1 == -1) || (roll2 == -1)) {
            throw "Roll dice error";
        }
        std::cout << currPlayer->getName() << ": " << "The first roll is " << roll1 << std::endl;
        std::cout << currPlayer->getName() << ": " << "The second roll is " << roll2 << std::endl;
        rollDouble += 1;
        if ((rollDouble == 3) && (roll1 == roll2)) {
            currPlayer->move("DC Tims Line");
            GamePrint();
        } else if (roll1 == roll2) {
            currPlayer->move(roll1 + roll2);
            GamePrint();
            if (currPlayer->makeAction(roll1, roll2)) {
                std::cout << "Auction begins!" << std::endl;
                playerPtr auctionWinner = auction();
                auctionWinner->get((Ownable *)currPlayer->getLanding());
            }
            GamePrint();
            if (currPlayer->moneyOwed) {
                break;
            }
            if (currPlayer->getLanding()->getTime(currPlayer.get()) >= 0) {
                std::cout << currPlayer->getName() << ": " << "You were sent to DC Tims Line, can't roll your dices now!" << std::endl;
                break;
            }
            std::cout << currPlayer->getName() << ": " << "You rolled double! Please roll again!" << std::endl;
            std::string line;
            std::string cmd;
            while (getline(std::cin, line)) {
                ss = std::istringstream{line};
                ss >> cmd;
                if (cmd != "roll") {
                    std::cout << currPlayer->getName() << ": " << "Please enter the correct command to roll!" << std::endl;
                    continue;
                } else {
                    break;
                }
            }
        } else {
            currPlayer->move(roll1 + roll2);
            GamePrint();
            if (currPlayer->makeAction(roll1, roll2)) {
                std::cout << "Auction begins!" << std::endl;
                playerPtr auctionWinner = auction();
                auctionWinner->get((Ownable *)currPlayer->getLanding());
            }
            GamePrint();
            rollDouble = 0;
            break;
        }
    }
}


void Controller::GameLoad(std::ifstream &loadFile) {
    dice = Dice{testPermit, rng};
    grid = std::make_shared<Grid>(dice);
    int playerNum, cupNum, money, position, improvement, stuck;
    int time = -1;
    std::string cmd, playerName, mode, owner, property;
    char playerChar;
    grid->gridAttach();
    loadFile >> playerNum;
    if (loadFile.fail()) { throw "Load file failed."; }
    for (int i = 0; i < playerNum; i += 1) {
        loadFile >> playerName >> playerChar >> cupNum >> money >> position;
        std::cout << "Player Information: " << std::endl;
        std::cout << "- " << playerName << " " << playerChar << " ";
        std::cout << cupNum << " " << money << " " << position << std::endl;
        if (loadFile.fail()) { throw "Load file failed."; }
        stuck = false;
        if (position == 10) { loadFile >> stuck; }
        if (stuck) {
            loadFile >> time;
            if (loadFile.fail()) { throw "Load file failed."; }
        }
        playerPtr currPlayer(new Player{grid->at(position).get(), playerChar, playerName});
        playerList.emplace_back(currPlayer);
        for (int i = 0; i < cupNum; ++i) {
            if (dice.get_cup() == 4) {
                throw "There should be no more than 4 active Roll Up the Rim Cup!";
            }
            dice.add_cup();
        }
        currPlayer->addCup(cupNum);
        if (money < 0) {
            currPlayer->pay(nullptr, 1500);
            currPlayer->moneyOwed = -1 * money;
        } else {
            currPlayer->pay(nullptr, 1500 - money);
        }
        grid->at(position)->attachGuest(currPlayer.get(), stuck);
        if ((position == 10) && stuck) {
            grid->at(position)->setTime(currPlayer.get(), 3 - time);
        }
    }
    getline(loadFile, mode);
    while (getline(loadFile, mode)) {
        std::stringstream ss{mode};
        ss >> property >> owner >> improvement;
        if (ss.fail()) { return; }
        for (int i = 0; i < (int)playerList.size(); i += 1) {
            if (owner == playerList[i]->getName()) {
                Property *p = grid->find(property).get();
                Ownable *propertyPtr = dynamic_cast<Ownable *>(p);
                playerList[i]->get(propertyPtr);
                propertyPtr->setImprovementLevel(improvement);
                break;
            }
        }
    }
    std::cout << "The file is loaded to the game!" << std::endl;
}


void Controller::GameSave(std::ofstream &saveFile) {
    saveFile << (int)playerList.size() << std::endl;
    int savePlayerNum = currPlayerNum;
    saveFile << playerList[savePlayerNum]->getName() << " ";
    saveFile << playerList[savePlayerNum]->getSymbol() << " ";
    saveFile << playerList[savePlayerNum]->getCup() << " ";
    if (playerList[savePlayerNum]->moneyOwed) {
        saveFile << playerList[savePlayerNum]->getCash() - playerList[savePlayerNum]->moneyOwed << " ";
    } else {
        saveFile << playerList[savePlayerNum]->getCash() << " ";
    }
    saveFile << grid->findPos(playerList[savePlayerNum]->getLanding()->getName());
    if (grid->findPos(playerList[savePlayerNum]->getLanding()->getName()) == 10) {
        if (playerList[savePlayerNum]->getLanding()->getTime(playerList[savePlayerNum].get()) == -1) {
            saveFile << " " << 0;
        } else {
            saveFile << " " << 1 << " " << 3 - playerList[savePlayerNum]->getLanding()->getTime(playerList[savePlayerNum].get());
        }
    }
    saveFile << std::endl;
    nextPlayer(savePlayerNum);
    while (savePlayerNum != currPlayerNum) {
        saveFile << playerList[savePlayerNum]->getName() << " ";
        saveFile << playerList[savePlayerNum]->getSymbol() << " ";
        saveFile << playerList[savePlayerNum]->getCup() << " ";
        if (playerList[savePlayerNum]->moneyOwed) {
            saveFile << playerList[savePlayerNum]->getCash() - playerList[savePlayerNum]->moneyOwed << " ";
        } else {
            saveFile << playerList[savePlayerNum]->getCash() << " ";
        }
        saveFile << grid->findPos(playerList[savePlayerNum]->getLanding()->getName());
        if (grid->findPos(playerList[savePlayerNum]->getLanding()->getName()) == 10) {
            if (playerList[savePlayerNum]->getLanding()->getTime(playerList[savePlayerNum].get()) == -1) {
                saveFile << " " << 0;
            } else {
                saveFile << " " << 1 << " " << 3 - playerList[savePlayerNum]->getLanding()->getTime(playerList[savePlayerNum].get());
            }
        }
        saveFile << std::endl;
        nextPlayer(savePlayerNum);
    }
    grid->gridSave(saveFile);
    std::cout << "All game data is stored!" << std::endl;
}


void Controller::GameTest(void) noexcept { testPermit = true; }


void Controller::GameSeed(const int &_seed) noexcept {
    seed = _seed;
    seedPermit = true;
    rng = std::default_random_engine{seed};
}


void Controller::GameExtend(void) noexcept { extendPermit = true; }


void Controller::GamePrint(void) noexcept { grid->gridPrint(); }



// ================================================================================
// Private methods of Controller

std::map<char, std::string> Controller::listInitializer(void)
        const noexcept {
    std::map<char, std::string> characterList;
    characterList.emplace('G', "Goose");
    characterList.emplace('B', "GRT_Bus");
    characterList.emplace('D', "Tim_Hortons_Doughnut");
    characterList.emplace('P', "Professor");
    characterList.emplace('S', "Student");
    characterList.emplace('$', "Money");
    characterList.emplace('L', "Laptop");
    characterList.emplace('T', "Pink_tie");
    return characterList;
}


int Controller::listPosition(const char &c, std::map<char, std::string> nl)
        const noexcept {
    std::map<char, std::string>::iterator it = nl.find(c);
    if (it != nl.end()) {
        return std::distance(nl.begin(), it);
    }
    return INVALID_POSITION;
}


void Controller::nextPlayer(int &currentPlayerNum) noexcept {
    currentPlayerNum = (currentPlayerNum + 1) % (int)playerList.size();
}


Player *Controller::findPlayer(const std::string &name) noexcept {
    for (int i = 0; i < (int)playerList.size(); i += 1) {
        if (playerList[i]->getName() == name) {
            return playerList[i].get();
        }
    }
    return nullptr;
}


const bool Controller::isNum(const char &c) const noexcept {
    return '0' <= c && c <= '9';
}


const bool Controller::checkWin(void) const noexcept {
    return playerList.size() == 1;
}


playerPtr Controller::auction(playerPtr currPlayer) {
    int bid = 0;
    int i = 0;
    std::string line;
    std::istringstream ss;
    std::vector<playerPtr> bidList = playerList;
    if (currPlayer != nullptr) {
        auto it = find(bidList.begin(), bidList.end(), currPlayer);
        bidList.erase(it);
    }
    while (bidList.size() > 1) {
        std::cout << bidList.at(i)->getName() << ": " << "Current bid price is: " << bid << std::endl;
        std::cout << bidList.at(i)->getName() << ": " << "Do you wish to raise the current bid or withdraw?" << std::endl;
        std::cout << bidList.at(i)->getName() << ": " << "You can input \"raise [new bid price]\" or \"withdraw\"" <<std::endl;
        if (!getline(std::cin, line)) {
            throw "Input invalid.";
        }
        std::string choose;
        ss = std::istringstream{line};
        ss >> choose;
        if (choose == "raise") {
            int new_bid;
            ss >> new_bid;
            if (new_bid <= bid) {
                std::cout << bidList.at(i)->getName() << ": " << "Your bid price has to be higher than the current bid price, please try again!" << std::endl;
                continue;
            }
            if (new_bid > bidList.at(i)->getCash()) {
                std::cout << bidList.at(i)->getName() << ": " << "You do not have enough money to pay your bid, please try again!" << std::endl;
                continue;
            }
            bid = new_bid;
            i += 1;
        } else if (choose == "withdraw") {
            bidList.erase(bidList.begin() + i);
        }
        if (i >= (int)bidList.size()) { i = 0; }
    }
    std::cout << "Congrats! " << bidList[0]->getName() << " has won the auction!" << std::endl;
    bidList[0]->pay(nullptr, bid);
    return bidList[0];
}


void Controller::controllerClean(void) {
    std::cout << "Cleaning the data ..." << std::endl;
    grid->gridClean();
    for (int i = 0; i < (int)playerList.size(); i += 1) {
        playerList.at(i)->destroy();
    }
}


Controller::~Controller() { controllerClean(); }
