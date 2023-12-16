#include "attack_tables.h"
#include "bitboard.h"

void Attacks::init_leapers_attacks(){
    for(int i = 0; i<64; i++){
        pawn_attacks[white][i] = mask_pawn_attacks(i,white);
    }
    for(int i = 0; i<64; i++){
        pawn_attacks[black][i] = mask_pawn_attacks(i,black);
    }
    for(int i = 0; i<64; i++){
        knight_attacks[i] = mask_knight_attacks(i);
    }
}

Bitboard Attacks::mask_pawn_attacks(int square, int side){
    Bitboard bitboard;
    Bitboard attacks;

    //set piece on the board;
    bitboard.bits.set(square);
    int file = square % 8;
    
    if(!side){
        int mask1 = square -7;
        int mask2 = square -9;
        if(mask1>=0 && file<7) attacks.bits.set(mask1);
        if(mask2>=0 && file>0) attacks.bits.set(mask2);

    }
    else{
        int mask1 = square +7;
        int mask2 = square +9;
        if(mask1<64 && file>0) attacks.bits.set(mask1);
        if(mask2<64 && file<7) attacks.bits.set(mask2);

    }

    return attacks;
}

Bitboard Attacks::mask_knight_attacks(int square){
    Bitboard bitboard;
    Bitboard attacks;

    //set piece on the board;
    bitboard.bits.set(square);
    int file = square % 8;
    int rank = square / 8;

    int mask1 = square - 17;
    int mask2 = square - 15;
    int mask3 = square - 10;
    int mask4 = square - 6;
    int mask5 = square + 17;
    int mask6 = square + 15;
    int mask7 = square + 10;
    int mask8 = square + 6;


    if(file>0 && mask1>=0) attacks.bits.set(mask1);
    if(file<7 && mask2>=0) attacks.bits.set(mask2);
    if(file>1 && mask3>=0) attacks.bits.set(mask3);
    if(file<6 && mask4>=0) attacks.bits.set(mask4);

    if(file<7 && mask5<=63) attacks.bits.set(mask5);
    if(file>0 && mask6<=63) attacks.bits.set(mask6);
    if(file<6 && mask7<=63) attacks.bits.set(mask7);
    if(file>1 && mask8<=63) attacks.bits.set(mask8);

    return attacks;
}

Bitboard Attacks::mask_king_attacks(int square){
    Bitboard bitboard;
    Bitboard attacks;

    //set piece on the board;
    bitboard.bits.set(square);
    int file = square % 8;
    int rank = square / 8;

    int mask1 = square - 9;
    int mask2 = square - 8;
    int mask3 = square - 7;
    int mask4 = square - 1;
    int mask5 = square + 1;
    int mask6 = square + 7;
    int mask7 = square + 8;
    int mask8 = square + 9;


    return attacks;

}
