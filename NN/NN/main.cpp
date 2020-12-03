#include "Evolution.h"

int main() {
    genetic::Evolution evolution;// { "nn1.bin", "nn2.bin" };

    //evolution.evolution();
    evolution.load("nn2-3e.bin", "nn1-3m.bin");

    return 0;
}