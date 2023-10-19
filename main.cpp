#include <iostream>
#include "PDA.h"
#include "CFG.h"

int main() {
    PDA pda("input-pda2cfg1.json");
    pda.toCFG().print();
    return 0;
}
