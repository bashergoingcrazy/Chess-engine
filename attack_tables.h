#ifndef ATTACKTABLES_H
#define ATTACKTABLES_H

#include "bitboard.h"

// WARNING adjust the public and private methods
class Attacks{
private:
     void init_leapers_attacks();


public:
     Bitboard pawn_attacks[2][64];
     Bitboard knight_attacks[64];
     Bitboard king_attacks[64];
     
     Bitboard mask_pawn_attacks(int square, int side);
     Bitboard mask_king_attacks(int square);
     Bitboard mask_knight_attacks(int square);
     Attacks(){
         init_leapers_attacks();
     }
};


#endif // !ATTACKTABLES_H



