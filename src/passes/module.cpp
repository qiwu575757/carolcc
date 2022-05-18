
#include "module.h"

void scope::enter() {
    this->_table.push_back({});
}
void scope::exit() {
    this->_table.pop_back();
}
