#include "attack_tables.h"
#include "bitboard.h"
#include "randomNum.h"
#include <algorithm>
#include <iostream>

Attacks atk;

void Attacks::init_leapers_attacks(){
    for(int i = 0; i<64; i++){
        pawn_attacks[white][i] = mask_pawn_attacks(i,white);
    }
    for(int i = 0; i<64; i++){
        pawn_attacks[black][i] = mask_pawn_attacks(i,black);
    }
    for(int i = 0; i<64; i++){
        knight_attacks[i] = mask_knight_attacks(i);
        king_attacks[i] = mask_king_attacks(i);
    }
}

void Attacks::init_slider_attacks(int bishop){

    for(int square = 0; square<64; square++){
        bishop_masks[square]  = mask_bishop_attacks(square);
        rook_masks[square] = mask_rook_attacks(square);

        Bitboard attack_mask = bishop ? rook_masks[square]: bishop_masks[square];

        int relevant_bits = attack_mask.bits.count();
        int occupancy_indicies = (1 << relevant_bits);

        for(int index = 0; index<occupancy_indicies; index++){
            if(!bishop){
                Bitboard occupancy = set_occupancy(index, relevant_bits, attack_mask);

                int magic_index = (occupancy.bits.to_ullong() * bishop_magic_number[square]) >> (64 - bishop_relevant_bits[square]);

                bishop_attacks[square][magic_index] = bishop_attacks_on_the_fly(square, occupancy);
            }
            else{
                Bitboard occupancy = set_occupancy(index, relevant_bits, attack_mask);

                int magic_index = (occupancy.bits.to_ullong() * rook_magic_number[square]) >> (64 - rook_relevant_bits[square]);

                rook_attacks[square][magic_index] = rook_attacks_on_the_fly(square, occupancy);

            }
        }
    }
}

Bitboard Attacks::get_bishop_attacks(int square, Bitboard occupancy) {
    unsigned long long occupancylong = (occupancy.bits & bishop_masks[square].bits).to_ullong();
    occupancylong *= bishop_magic_number[square];
    occupancylong >>= (64 - bishop_relevant_bits[square]);

    return bishop_attacks[square][occupancylong];
}


Bitboard Attacks::get_rook_attacks(int square, Bitboard occupancy){
    unsigned long long occupancylong = (occupancy.bits & rook_masks[square].bits).to_ullong();
    occupancylong *= rook_magic_number[square];
    occupancylong >>= 64 - rook_relevant_bits[square];

    return rook_attacks[square][occupancylong];
}

Bitboard Attacks::get_queen_attacks(int square, Bitboard occupancy){
    Bitboard result;
    result = get_bishop_attacks(square, occupancy);
    result.bits |= get_rook_attacks(square,occupancy).bits;
    return result; 
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

    if(file>0 && mask1>=0) attacks.bits.set(mask1);
    if(mask2>=0) attacks.bits.set(mask2);
    if(file<7 && mask3>=0) attacks.bits.set(mask3);
    if(file>0) attacks.bits.set(mask4);
    if(file<7) attacks.bits.set(mask5);
    if(file>0 && mask6<=63) attacks.bits.set(mask6);
    if(mask7<=63) attacks.bits.set(mask7);
    if(file<7 && mask8<=63) attacks.bits.set(mask8);


    return attacks;

}

Bitboard Attacks::mask_bishop_attacks(int square){
    Bitboard attacks;

    int r,f;
    int tr = square / 8; 
    int tf = square % 8;

    for(r = tr+1, f = tf+1; r<=6 && f<=6; r++,f++ ) attacks.bits.set(r*8+f);
    for(r = tr-1, f = tf+1; r>=1 && f<=6; r--, f++ ) attacks.bits.set(r*8+f);
    for(r = tr+1, f = tf-1; r<=6 && f>=1; r++,f-- ) attacks.bits.set(r*8+f);
    for(r = tr-1, f = tf-1; r>=1 && f>=1; r--,f-- ) attacks.bits.set(r*8+f);


    return attacks;
}

Bitboard Attacks::bishop_attacks_on_the_fly(int square, Bitboard block){
    Bitboard attacks;

    int r,f;
    int tr = square / 8; 
    int tf = square % 8;

    for(r = tr+1, f = tf+1; r<=7 && f<=7; r++,f++ ){
        attacks.bits.set(r*8+f);
        if(block.bits.test(r*8+f))
            break;
    } 
    for(r = tr-1, f = tf+1; r>=0 && f<=7; r--, f++ ){
        attacks.bits.set(r*8+f);
        if(block.bits.test(r*8+f))
            break;
    } 
    for(r = tr+1, f = tf-1; r<=7 && f>=0; r++,f-- ){
        attacks.bits.set(r*8+f);
        if(block.bits.test(r*8+f))
            break;
    } 
    for(r = tr-1, f = tf-1; r>=0 && f>=0; r--,f-- ){
        attacks.bits.set(r*8+f);
        if(block.bits.test(r*8+f))
            break;
    } 

    return attacks;
}

Bitboard Attacks::mask_rook_attacks(int square){
    Bitboard attacks;

    int r,f;
    int tr = square / 8; 
    int tf = square % 8;

    for(r = tr+1; r<=6 ;r++) attacks.bits.set(r*8+tf);
    for(r = tr-1; r>=1 ;r--) attacks.bits.set(r*8+tf);
    for(f = tf+1; f<=6 ;f++) attacks.bits.set(tr*8 + f);
    for(f = tf-1; f>=1 ;f--) attacks.bits.set(tr*8 + f);

    return attacks;
}

Bitboard Attacks::rook_attacks_on_the_fly(int square, Bitboard block){
    Bitboard attacks;

    int r,f;
    int tr = square / 8; 
    int tf = square % 8;

    for(r = tr+1; r<=7 ;r++){
      attacks.bits.set(r*8+tf);  
      if(block.bits.test(r*8+tf)) break;
      
    } 
    for(r = tr-1; r>=0 ;r--){
        attacks.bits.set(r*8+tf);
        if(block.bits.test(r*8+tf)) break;
    } 
    for(f = tf+1; f<=7 ;f++){
        attacks.bits.set(tr*8 + f);
        if(block.bits.test(tr*8+f)) break;
    } 
    for(f = tf-1; f>=0 ;f--){
        attacks.bits.set(tr*8 + f);
        if(block.bits.test(tr*8+f)) break;
    } 

    return attacks;
}

Bitboard Attacks::set_occupancy(int index, int bits_in_mask, Bitboard attack_mask){
    Bitboard occupancy ;

    //loop over the range of bits within attack mask
    for(int count = 0; count <bits_in_mask; count++){
        int square = attack_mask.get_lsb_index();

        attack_mask.bits.reset(square);

        //correct the below code to fit my criterion
        if(index & (1<<count))
            occupancy.bits.set(square);
    }



    return occupancy;
}

int count_bits(unsigned long long number) {
    int count = 0;
    while (number) {
        count += number & 1;
        number >>= 1;
    }
    return count;
}




unsigned long long Attacks::find_magic_number(int square, int relevant_bits, int bishop){
    unsigned long long occupancies[4096];
    unsigned long long attacks[4096];
    unsigned long long used_attacks[4096];
    unsigned long long attack_mask = bishop ? mask_rook_attacks(square).bits.to_ullong()
        : mask_bishop_attacks(square).bits.to_ullong();

    int occupancy_indicies = 1 << relevant_bits;
    
    for(int index=0; index<occupancy_indicies; index++){
        occupancies[index] = set_occupancy(index,relevant_bits,attack_mask).bits.to_ullong();
        attacks[index]=bishop? rook_attacks_on_the_fly(square,occupancies[index]).bits.to_ullong():
            bishop_attacks_on_the_fly(square,occupancies[index]).bits.to_ullong();
        // std::cout<<"Index "<<index<<std::endl;
        // print_bitboard(attacks[index]);
        // std::cin.ignore();
    }

    for(int random_count=0 ;random_count < 100000000; random_count++){
        unsigned long long magic_number = generate_magic_number();
        // std::cout<<magic_number<<std::endl;
        if(count_bits((attack_mask*magic_number)& 0xFF00000000000000)<6)
            continue;
        std::fill(used_attacks,used_attacks+4096,0ULL);
        int index, fail;
        for(index=0,fail=0; !fail && index<occupancy_indicies;index++){
            // init magic index
            int magic_index = (int)((occupancies[index]*magic_number)>>(64-relevant_bits));
            if(used_attacks[magic_index]==0ULL){
                used_attacks[magic_index] = attacks[index];
            }
            else if(used_attacks[magic_index] != attacks[index]){
                fail=1;
            }
        }
        if(!fail){
            return magic_number;
        }
    }
    std::cout<<"Magic number fails";
    return 0ULL;
    
    return attack_mask;

}


