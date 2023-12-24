#pragma once

#include <string>

// parse user GUI move string input 
int parse_move(std::string move_string);

int parse_position(std::string input);

void parse_go(std::string input);

void uci_loop();

void test();
