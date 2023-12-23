#include <iostream>
#include "bitboard.h"
#include "bitgamestate.h"
#include "search.h"
#include "time.h"


// #define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
int main(){

    // sest occupancy
    BitGameState bgs ; 
    bgs.parse_fen(tricky_position);
    bgs.print_board();

    mainPerft(6,bgs);


    return 0;
}

