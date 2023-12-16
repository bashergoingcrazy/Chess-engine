#include "bitboard.h"
#include <iostream>

void print_bitboard(Bitboard bitboard){
    for(int rank = 0 ; rank<8 ; rank++){
        for(int file = 0; file< 8; file++){
            int square = rank*8 + file;

            std::cout<<square<<std::endl;
        }
        std::cout<<"\n";
    }
}
