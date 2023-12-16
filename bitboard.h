#ifndef BITBOARD_H
#define BITBOARD_H

#include <bitset>
struct Bitboard{
    std::bitset<64> bits;
};

void print_bitboard(Bitboard bitboard);

#endif // !BITBOARD_H
