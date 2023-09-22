#include "gotims.h"

GoTims::GoTims(): Unownable("Go to Tims", true) { }


const std::string GoTims::getMovement(Player *p) noexcept {
    return "DC Tims Line";
}
