#include "bitboard.h"
#include <iostream>

void print_bitboard(Bitboard bitboard){
    std::cout<<std::endl;

    for(int rank = 0 ; rank<8 ; rank++){
        for(int file = 0; file< 8; file++){
            int square = rank*8 + file;

            if(!file){
                std::cout<<8-rank<<"  ";
            }

            std::cout<<bitboard.bits[square]<<" ";
        }
        std::cout<<"\n";
    }
    std::cout<<std::endl;
    std::cout<<"   a b c d e f g h"<<std::endl;
    std::cout<<"   "<<bitboard.bits.to_ullong()<<std::endl;
}
