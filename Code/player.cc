#include "player.h"

static const Player *BANK = nullptr;


Player::Player(Property *landing, const char &symbol, const std::string &name):
    symbol(symbol), name(name), landing(landing) {
        ownedList.clear();
        mortgagedList.clear();
}


const int &Player::getCup(void) { return cup; }


void Player::addCup(const int num) {
    if (num > 0) {
        std::cout << getName() << ": " << "You win " << num << " cup!" << std::endl;
    } else {
        std::cout << getName() << ": " << "You lose " << -1 * num << " cup!" << std::endl;
    }
    cup += num;
    std::cout << getName() << ": " << "Now you have " << cup << " cup" << (cup == 1 ? "" : "s") << std::endl;
}


const int &Player::getCash(void) const { return cashFlow; }


const bool &Player::getMovable(void) { return movable; }


const char &Player::getSymbol(void) { return symbol; }


const std::string &Player::getName(void) { return name; }


Property *Player::getLanding(void) { return landing; }


bool Player::move(const int &step) {
    if (step != 0) {
        landing->removeGuest(*this);
        std::cout << getName() << " is leaving from " << landing->getName() << std::endl;
        if (step >= 0) {
            for (int i = 0; i < step; i += 1) {
                landing = landing->getNext();
                if ((landing->getName() == "Collect OSAP") && (i != step - 1)) {
                    pay(nullptr, landing->getTuition(*this));
                }
            }
        } else {
            for (int i = 0; i > step; i -= 1) {
                landing = landing->getPrev();
                if ((landing->getName() == "Collect OSAP") && (i != step - 1)) {
                    pay(nullptr, landing->getTuition(*this));
                }
            }
        }
        landing->attachGuest(this);
        std::cout << getName() << " is landing at " << landing->getName() << std::endl;
        movable = false;
        movalready = true;
    } else {
        return false;
    }
    return true;
}


bool Player::move(const std::string &target) {
    std::cout << getName() << " is leaving from " << landing->getName() << std::endl;
    bool OSAPBanned = false;
    bool isDC = false;
    if (target == "DC Tims Line") {
        OSAPBanned = true;
        isDC = true;
    } else if (target == "PAC") {
        OSAPBanned = true;
    }
    landing->removeGuest(*this);
    while (landing->getName() != target) {
        landing = landing->getNext();
        if (landing->getName() == "Collect OSAP" && !OSAPBanned) {
            pay(nullptr, -200);
        }
    }
    landing->attachGuest(this, isDC);
    std::cout << getName() << " is landing at " << landing->getName() << std::endl;
    movable = false;
    movalready = true;
    if (isDC) {
        std::cout << getName() << " is currently waiting at DC Tims Line!" << std::endl;
    }
    return true;
}


void Player::moveReset(void) {
    if (landing->getName() == "DC Tims Line") {
        if (landing->getTime(this) == -1 ||
            landing->getTime(this) == 0) {
        } else {
            landing->subTime(this);
            movable = false;
            movalready = true;
            return;
        }
    }
    movable = true;
    movalready = false;
}


bool Player::makeAction(int &roll1, int &roll2) {
    roll1 = -1;
    roll2 = -1;
    if (landing->getType() == "EVENT") {
        if (landing->getMovable()) {
            bool isStep = true;
            int step = 0;
            std::string target = landing->getMovement(this);
            try {
                step = std::stoi(target);
            }  catch(std::out_of_range &e) {
                isStep = false;
            } catch(std::invalid_argument &e) {
                isStep = false;
            }
            if (isStep) {
                if (move(step)) {
                    return makeAction(roll1, roll2);
                } else {
                    return false;
                }
            } else {
                move(target);
                if (landing->getName() != "DC Tims Line") {
                    return makeAction(roll1, roll2);
                }
            }
        } else {
            if (landing->getName() == "DC Tims Line") {
                int tuition = 0;
                if (landing->getOutOfLine(*this, tuition, roll1, roll2)) {
                    landing->setTime(this, -1);
                    std::cout << "Player " << getName() << " gets out of DC Tims Line!" << std::endl;
                    if (pay(nullptr, tuition)) {
                        if ((roll1 != -1) && (roll2 != -1)) {
                            if (move(roll1 + roll2)) {
                                return makeAction(roll1, roll2);
                            } else {
                                return false;
                            }
                        } else {
                            movable = true;
                            movalready = false;
                        }
                    }
                    return false;
                }
            } else {
                int tuition = landing->getTuition(*this);
                if (!pay(nullptr, tuition)) {
                    return false;
                }
            }
        }
    } else {
        if (!landing->getOwner()) {
            std::cout << "This place does not have an owner." << std::endl;
            std::cout << "Do you want to buy this building?" << std::endl;
            std::cout << "Type Y if you want, type N is you don't." << std::endl;
            std::string choose = "N";
            while (getline(std::cin, choose)) {
                if (choose == "Y") {
                    Property *p = landing;
                    if (!buy(dynamic_cast<Ownable *>(p))) { return true; }
                    break;
                } else if (choose == "N") {
                    std::cout << "You have decided not to buy this building." << std::endl;
                    return true;
                } else {
                    std::cout << "You should type \'Y\' or \'N\'" << std::endl;
                }
            } 
        } else {
            int tuition = landing->getTuition(*this);
            if (landing->getOwner() != this) {
                pay(landing->getOwner(), tuition);
            }
        }
    }
    return false;
}


bool Player::pay(Player *owner, const int tuition) {
    lastPropertyOwner = owner;
    if (cashFlow - tuition < 0 && tuition > 0) {
        moneyOwed =  moneyOwed + tuition;
        if (owner) {
            std::cout << "Now you owe " << owner->getName() << " $" << moneyOwed << "." << std::endl;
        } else {
            std::cout << "Now you owe Bank $" << moneyOwed << "." << std::endl;
        }
        return false;
    }
    cashFlow -= tuition;
    if (tuition > 0) {
        std::cout << "$" << tuition << " has been removed from " << getName() << "'s cash flow!" << std::endl;
    } else if (tuition < 0) {
        std::cout << "$" << -1 * tuition << " has been added to " << getName() << "'s cash flow!" << std::endl;
    }
    if (owner) { owner->pay(nullptr, -1 * tuition); }
    return true;
}


bool Player::buy(Ownable *property) {
    if (property->getOwner()) {
        std::cout << "Sorry, this building is owned by" << property->getOwner()->getName() << std::endl;
        std::cout << "If you want this building, you should trade with him." << std::endl;
        return false;
    }
    if (getCash() < property->getPrice()) {
        std::cout << "Sorry, you don't have enough money for this building." << std::endl;
        return false;
    }
    pay(nullptr, property->getPrice());
    property->setOwner(this);
    ownedList.emplace_back(property);
    return true;
}


void Player::get(Ownable *property) {
    if (((Ownable *)property)->getImproveLevel() == -1) {
        mortgagedList.emplace_back(property);
    }
    ownedList.emplace_back(property);
    property->setOwner(this);
}


void Player::improve(const std::string &property) {
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        if (ownedList.at(i)->getName() == property) {
            ownedList.at(i)->improve();
            return;
        }
    }
    std::cout << getName() << " doesn't have the property: " << property << std::endl;
}


void Player::degenerate(const std::string &property) {
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        if (ownedList.at(i)->getName() == property) {
            ownedList.at(i)->degenerate();
            return;
        }
    }
    std::cout << getName() << " doesn't have the property: " << property << std::endl;
}


void Player::mortgage(const std::string &property) {
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        if (ownedList.at(i)->getName() == property) {
            if (ownedList.at(i)->getImproveLevel() == -1) { 
                std::cout << "You cannot mortgage this property. It is already mortgaged." << std::endl;
                break;
            } else if (ownedList.at(i)->getImproveLevel() > 0) {
                std::cout << "You cannot mortgage this property. Please sell the improvements on this building first." << std::endl;
                break;
            }
            if (ownedList.at(i)->checkOtherImprove()) {
                std::cout << "You cannot mortgage this property. Please sell the improvements on buildings in its monopoly first." << std::endl;
                break;
            }
            ownedList.at(i)->setImprovementLevel(-1);
            mortgagedList.emplace_back(ownedList.at(i));
            pay(nullptr, -1 * ownedList.at(i)->getPrice() / 2);
            std::cout << ownedList.at(i)->getName() << " is mortgaged to the bank." << std::endl;
            return;
        }
    }
    std::cout << getName() << " doesn't have the property: " << property << std::endl;
}


void Player::unmortgage(const std::string &property) {
    for (int i = 0; i < (int)mortgagedList.size(); i += 1) {
        if (mortgagedList.at(i)->getName() == property) {
            int payment = mortgagedList.at(i)->getPrice() * 0.6;
            if (getCash() < payment) {
                std::cout << "You don't have enough money for unmortgaging this building." << std::endl;
                return;
            }
            pay(nullptr, payment);
            mortgagedList.at(i)->setImprovementLevel(0);
            std::cout << mortgagedList.at(i)->getName() << " is unmortgaged from the bank." << std::endl;
            mortgagedList.erase(mortgagedList.begin() + i);
            return;
        }
    }
    std::cout << getName() << " doesn't have the property: " << property;
    std::cout << ", or it is unmortgaged already." << std::endl;
}


void Player::tradeWith(Player *trader, const int &money,
                       const std::string &receive) {
    if (getCash() < money) {
        std::cout << getName() << ": " << "Sorry, you don't have so much money for this trade." << std::endl;
        return;
    }
    if (trader == BANK) {
        std::cout << getName() << ": " << "Sorry, you cannot trade with bank or somebody we don't know" << std::endl;
        return;
    }
    Ownable *result = trader->tradeAgree(money, receive);
    if (result) {
        pay(nullptr, money);
        ownedList.emplace_back(result);
        if (result->getImproveLevel() == -1) {
            receiveMortgaged(result);
        }
        result->setOwner(this);
        std::cout << getName() << ": " << "The trade is made. Congratulations." << std::endl;
    } else {
        std::cout << getName() << ": " << "The trade is cancelled." << std::endl;
    }
}


void Player::tradeWith(Player *trader, const std::string &give, const int &money) {
    if (trader == BANK) {
        std::cout << getName() << ": " << "Sorry, you cannot trade with bank or somebody we don't know" << std::endl;
        return;
    }
    Ownable *property = nullptr;
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        if (ownedList.at(i)->getName() == give) {
            property = ownedList.at(i);
        }
    }
    if (!property) {
        std::cout << getName() << ": " << "Sorry, you don't have this building: " << give << std::endl;
        return;
    }
    if (property->getImproveLevel() > 0) {
        std::cout << getName() << ": " << "Sorry, building " << give << " cannot be sold." << std::endl;
        std::cout << getName() << ": " << "The improvement level of this building is: " << property->getImproveLevel() << std::endl;
        return;
    }
    if (property->checkOtherImprove()) {
        std::cout << getName() << ": " << "Sorry, building " << give << " cannot be sold." << std::endl;
        std::cout << getName() << ": " << "This building is under monopoly, and some building in the monopoly is improved." << std::endl;
        return;
    }
    bool result = trader->tradeAgree(property, money);
    if (result) {
        for (int i = 0; i < (int)ownedList.size(); i += 1) {
            if (ownedList.at(i) == property) {
                ownedList.erase(ownedList.begin() + i);
            }
        }
        if (property->getImproveLevel() == -1) {
            for (int i = 0; i < (int)mortgagedList.size(); ++i) {
                if (mortgagedList.at(i) == property) {
                    mortgagedList.erase(mortgagedList.begin() + i);
                }
            }
        }
        pay(nullptr, -1 * money);
        std::cout << getName() << ": " << "The trade is made. Congratulations." << std::endl;
    } else {
        std::cout << getName() << ": " << "The trade is cancelled." << std::endl;
    }
}


void Player::tradeWith(Player *trader, const std::string &give, const std::string &receive) {
    if (trader == BANK) {
        std::cout << getName() << ": " << "Sorry, you cannot trade with bank or somebody we don't know" << std::endl;
        return;
    }
    Ownable *property = nullptr;
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        if (ownedList.at(i)->getName() == give) {
            property = ownedList.at(i);
        }
    }
    if (!property) {
        std::cout << getName() << ": " << "Sorry, you don't have this building: " << give << std::endl;
        return;
    }
    if (property->getImproveLevel() > 0) {
        std::cout << getName() << ": " << "Sorry, building " << give << " cannot be sold." << std::endl;
        std::cout << getName() << ": " << "The improvement level of this building is: " << property->getImproveLevel() << std::endl;
        return;
    }
    if (property->checkOtherImprove()) {
        std::cout << getName() << ": " << "Sorry, building " << give << " cannot be sold." << std::endl;
        std::cout << getName() << ": " << "This building is under monopoly, and some building in the monopoly is improved." << std::endl;
        return;
    }
    Ownable *result = trader->tradeAgree(property, receive);
    if (result) {
        for (int i = 0; i < (int)ownedList.size(); i += 1) {
            if (ownedList.at(i) == property) {
                ownedList.erase(ownedList.begin() + i);
            }
        }
        if (property->getImproveLevel() == -1) {
            for (int i = 0; i < (int)mortgagedList.size(); ++i) {
                if (mortgagedList.at(i) == property) {
                    mortgagedList.erase(mortgagedList.begin() + i);
                }
            }
        }
        ownedList.emplace_back(result);
        if (result->getImproveLevel() == -1) {
            receiveMortgaged(result);
        }
        result->setOwner(this);
        std::cout << getName() << ": " << "The trade is made. Congratulations." << std::endl;
    } else {
        std::cout << getName() << ": " << "The trade is cancelled." << std::endl;
    }
}


Ownable *Player::tradeAgree(const int money, const std::string receive) {
    Ownable *property = nullptr;
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        if (ownedList.at(i)->getName() == receive) {
            property = ownedList.at(i);
        }
    }
    if (!property) {
        std::cout << getName() << ": " << "Sorry, I do not have property: " << receive << std::endl;
        return nullptr;
    }
    if (property->getImproveLevel() > 0) {
        std::cout << getName() << ": " << "Sorry, building " << receive << " cannot be sold." << std::endl;
        std::cout << getName() << ": " << "The improvement level of this building is: " << property->getImproveLevel() << std::endl;
        return nullptr;
    }
    if (property->checkOtherImprove()) {
        std::cout << getName() << ": " << "Sorry, building " << receive << " cannot be sold." << std::endl;
        std::cout << getName() << ": " << "This building is under monopoly, and some building in the monopoly is improved." << std::endl;
        return nullptr;
    }
    std::cout << getName() << ": " << "Do you want to trade your property: " << receive << " for $" << money << "?" << std::endl;
    if (property->getImproveLevel() == -1) {
        std::cout << getName() << ": " << "This is a mortgaged building" << std::endl;
    }
    std::cout << getName() << ": " << "Type accept if you want, type reject is you don't" << std::endl;
    std::string choose = "";
    
    while (getline(std::cin, choose)) {
        if (choose == "accept") {
            for (int i = 0; i < (int)ownedList.size(); i += 1) {
                if (ownedList.at(i)->getName() == receive) {
                    ownedList.erase(ownedList.begin() + i);
                }
            }
            if (property->getImproveLevel() == -1) {
                for (int i = 0; i < (int)mortgagedList.size(); ++i) {
                    if (mortgagedList.at(i) == property) {
                        mortgagedList.erase(mortgagedList.begin() + i);
                    }
                }
            }
            pay(nullptr, -1 * money);
            return property;
        } else if (choose == "reject") {
            std::cout << getName() << ": " << "Sorry, maybe next time~" << std::endl;
            return nullptr;
        } else {
            std::cout << getName() << ": " << "You should type accept or reject" << std::endl;
        }
    }
    return nullptr;
}


const bool Player::tradeAgree(Ownable *property, const int money) {
    if (getCash() < money) {
        std::cout << getName() << ": " << "Sorry, you don't have so much money for this trade." << std::endl;
        return false;
    }
    std::string choose = "";
    std::cout << getName() << ": " << "Do you want to trade " << property->getName() << " with $" << money << "?" << std::endl;
    if (property->getImproveLevel() == -1) {
        std::cout << getName() << ": " << "This is a mortgaged building" << std::endl;
    }
    std::cout << getName() << ": " << "Type accept if you want, type reject is you don't" << std::endl;
    
    while (getline(std::cin, choose)) {
        if (choose == "accept") {
            pay(nullptr, money);
            ownedList.emplace_back(property);
            if (property->getImproveLevel() == -1) {
                receiveMortgaged(property);
            }
            property->setOwner(this);
            std::cout << getName() << ": " << "The trade is made. Congratulations." << std::endl;
            return true;
        } else if (choose == "reject") {
            std::cout << getName() << ": " << "Sorry, maybe next time~" << std::endl;
            return false;
        } else {
            std::cout << getName() << ": " << "You should type accept or reject" << std::endl;
        }
    }
    return false;
}


Ownable *Player::tradeAgree(Ownable *property, const std::string receive) {
    Ownable *p = nullptr;
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        if (ownedList.at(i)->getName() == receive) {
            p = ownedList.at(i);
        }
    }
    if (!p) {
        std::cout << getName() << ": " << "Sorry, I do not have property: " << receive << std::endl;
        return nullptr;
    }
    if (p->getImproveLevel() > 0) {
        std::cout << getName() << ": " << "Sorry, building " << receive << " cannot be sold." << std::endl;
        std::cout << getName() << ": " << "The improvement level of this building is: " << property->getImproveLevel() << std::endl;
        return nullptr;
    }
    if (p->checkOtherImprove()) {
        std::cout << getName() << ": " << "Sorry, building " << receive << " cannot be sold." << std::endl;
        std::cout << getName() << ": " << "This building is under monopoly, and some building in the monopoly is improved." << std::endl;
        return nullptr;
    }
    std::cout << getName() << ": " << "Do you want to trade " << property->getName() << " with your property: " << receive << "?" << std::endl;
    if (property->getImproveLevel() == -1) {
        std::cout << getName() << ": " << property->getName() << " is a mortgaged building" << std::endl;
    }
    if (p->getImproveLevel() == -1) {
        std::cout << getName() << ": " << p->getName() << " is a mortgaged building" << std::endl;
    }
    std::cout << getName() << ": " << "Type accept if you want, type reject is you don't" << std::endl;
    std::string choose = "";
    
    while (getline(std::cin, choose)) {
        if (choose == "accept") {
            for (int i = 0; i < (int)ownedList.size(); i += 1) {
                if (ownedList.at(i) == p) {
                    ownedList.erase(ownedList.begin() + i);
                }
            }
            if (p->getImproveLevel() == -1) {
                for (int i = 0; i < (int)mortgagedList.size(); ++i) {
                    if (mortgagedList.at(i) == p) {
                        mortgagedList.erase(mortgagedList.begin() + i);
                    }
                }
            }
            ownedList.emplace_back(property);
            if (property->getImproveLevel() == -1) {
                receiveMortgaged(property);
            }
            property->setOwner(this);
            std::cout << getName() << ": " << "The trade is made. Congratulations." << std::endl;
            return p;
        } else if (choose == "reject") {
            std::cout << getName() << ": " << "Sorry, maybe next time~" << std::endl;
            return nullptr;
        } else {
            std::cout << getName() << ": " << "You should type accept or reject" << std::endl;
        }
    }
    return nullptr;
}


void Player::showAssets(void) {
    std::cout << "Player: " << getName() << std::endl;
    std::cout << "- CashFlow: " << getCash() << std::endl;
    std::cout << "- Cup:" << getCup() << std::endl;
    std::cout << "- Owned List: " << std::endl;
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        std::cout << "  " << i + 1 << ". " << ownedList.at(i)->getName();
        std::cout << " " << ownedList.at(i)->getImproveLevel();
        if (ownedList.at(i)->getImproveLevel() == -1) {
            std::cout << " " << "(Mortgaged)";
        }
        std::cout << std::endl;
    }
    std::cout << "- Mortgaged List: " << std::endl;
    for (int i = 0; i < (int)mortgagedList.size(); i += 1) {
        std::cout << "  " << i + 1 << ". " << mortgagedList.at(i)->getName();
        std::cout << " " << -1 << std::endl;
    }
}

#if 0

void Player::takeTuition(const int money, Ownable *property) {
    bool ownedProperty = true;
    for (int i = 0; i < (int)mortgagedList.size(); i += 1) {
        if (mortgagedList.at(i) == property) {
            ownedProperty = false;
            break;
        }
    }
    int payment = money;
    if (!ownedProperty) { payment /= 2; }
    pay(-1 * payment);
}
#endif


void Player::destroy(void) {
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        ownedList.at(i) = nullptr;
    }
    for (int i = 0; i < (int)mortgagedList.size(); i += 1) {
        mortgagedList.at(i) = nullptr;
    }
    landing = nullptr;
    lastPropertyOwner = nullptr;
}


const int Player::getTotalWorth(void) const {
    int worth = getCash();
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        if (ownedList.at(i)->getImproveLevel() == -1) {
            worth += (ownedList.at(i)->getTotalCost() / 2);
        } else {
            worth += ownedList.at(i)->getTotalCost();
        }
    }
    return worth;
}


const bool Player::checkMortgage(Ownable *p) {
    for (int i = 0; i < (int)mortgagedList.size(); i += 1) {
        if (p == mortgagedList.at(i)) {
            return true;
        }
    }
    return false;
}


std::vector<Ownable *> Player::getOwned(void) { return ownedList; }


void Player::turnPrivate(Player *player) {
    player->pay(this, -1 * cashFlow);
    player->addCup(getCup());
    cup = 0;
    for (int i = 0; i < (int)ownedList.size(); i += 1) {
        ownedList.at(i)->setOwner(player);
        player->receiveOwned(ownedList.at(i));
    }
    for (int i = 0; i < (int)mortgagedList.size(); i += 1) {
        player->receiveMortgaged(mortgagedList.at(i));
    }
    while (!ownedList.empty()) {
        ownedList.erase(ownedList.begin());
    }
    while (!mortgagedList.empty()) {
        mortgagedList.erase(mortgagedList.begin());
    }
}


void Player::receiveOwned(Ownable *property) {
    ownedList.emplace_back(property);
}


void Player::receiveMortgaged(Ownable *property) {
    std::cout << getName() << ": " << property->getName() << " is a mortgaged building, 10% of its original price has to be paid to the bank immediately." << std::endl;
    int price = property->getPrice();
    if (!pay(nullptr, price * 0.1)) { 
        std::cout << getName() << ": " << "You can't pay 10% of its original price, please pay your money owed!" << std::endl;
        mortgagedList.emplace_back(property);
    } else {
        std::cout << getName() << ": " << "You can choose to unmortgage it right now at $" << price * 0.5 << "." << std::endl;
        std::cout << getName() << ": " << "Or you can choose to leave it mortgaged for now, and unmortgage it later at $" << price * 0.6 << "." << std::endl;
        std::cout << getName() << ": " << "Do you wish to unmortgage it right now? Type Y/N:" << std::endl;
        std::string choose;
        while (getline(std::cin, choose)) {
            if (choose == "Y") {
                if (getCash() < price * 0.5) {
                    std::cout << getName() << ": " << "You don't have enough money for unmortgaging this building." << std::endl;
                    std::cout << getName() << ": " << "Please attempt unmortgage later." << std::endl;
                    mortgagedList.emplace_back(property);
                } else {
                    pay(nullptr, price * 0.5);
                    property->setImprovementLevel(0);
                    std::cout << getName() << ": " << property->getName() << " is unmortgaged from the bank." << std::endl;
                }
                break;
            } else if (choose == "N") {
                std::cout << getName() << ": " << "Got it! You have received the mortgaged building!" << std::endl;
                mortgagedList.emplace_back(property);
                break;
            } else {
                std::cout << getName() << ": " << "Invalid input, please try again." << std::endl;
            }
        }
    }
}

