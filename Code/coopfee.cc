#include "coopfee.h"

CoopFee::CoopFee(): Unownable("Coop Fee", false) { }


const int CoopFee::getTuition(Player &p) {
    return 150;
}
