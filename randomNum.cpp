#include "randomNum.h"

unsigned int state = 1804289383;

unsigned int get_random_U32_number(){
    unsigned int number = state;

    //XOR shift algorithm
    number ^=number <<13;
    number ^=number >> 17;
    number ^=number << 5;

    // update random number state
    state = number;
    
    //return random number
    return number;
}

unsigned long long get_random_U64_numbers(){
    unsigned long long n1, n2, n3, n4;
    n1 = (unsigned long long) (get_random_U32_number()& 0xFFFF);
    n2 = (unsigned long long) (get_random_U32_number()& 0xFFFF);
    n3 = (unsigned long long) (get_random_U32_number()& 0xFFFF);
    n4 = (unsigned long long) (get_random_U32_number()& 0xFFFF);

    return n1 | (n2<<16) | (n3<<32) | (n4<<48);
}

unsigned long long generate_magic_number(){
    return (get_random_U64_numbers() & get_random_U64_numbers() & get_random_U64_numbers());
} 
