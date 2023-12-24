#include <iostream>
#include "bitboard.h"
#include "bitgamestate.h"
#include "connect_gui.h"
#include "evaluation.h"
#include "search.h"
#include "time.h"


// #define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
// #define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
int main(){

    int debug_mode = 1;
    
    if(debug_mode)
    {
        BitGameState bgs;
        bgs.parse_fen( "rnbqkb1r/pppppppp/5n2/8/4P3/8/PPPP1PPP/RNBQKBNR w KQkq - 0 1 ");
        bgs.print_board();
        search_position(3,bgs);
    }
    else
        uci_loop();

    return 0;
}

