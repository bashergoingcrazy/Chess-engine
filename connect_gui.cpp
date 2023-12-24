#include "connect_gui.h"
#include "bitboard.h"
#include "bitgamestate.h"
#include "search.h"
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

BitGameState GameState;
int depth=5;
// #define empty_board "8/8/8/8/8/8/8/8 w - - "
// #define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
// #define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
// #define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
// #define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

void test(){
    parse_position("position startpos moves e2e4 e7e5 g1f3 fsd");
    GameState.print_board();
    parse_go("Go depth 3");

    // parse_position("position fen r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 moves g2h3 e8c8 e1c1 c7c5 d5c6 ");
    // parse_position("position startpos moves e2e4 e7e5 g1f3 g8f6 ");
    // // GameState.parse_fen(start_position);
    // // GameState.print_board();
    //
    //
    // // while(true){
    //     std::cout<<"Enter a Valid move in this position: ";
    //     std::string parserString;
    //     std::cin>>parserString;
    //     int move = parse_move(parserString);
    //     if(move)
    //         GameState.make_move(move,all_moves);
    //     else
    //         std::cout << "Illegal move given"; 
    //     GameState.print_board();
    // }
}

int parse_move(std::string move) {
    std::string sourceMove = move.substr(0, 2);
    std::string targetMove = move.substr(2,2);
    int sourceSquare, targetSquare ;
    for (int i = 0; i < 64; i++) {
        if (sourceMove == square_to_coordinates[i])
            sourceSquare = i;
        if (targetMove == square_to_coordinates[i])
            targetSquare = i;
    }

    std::vector<int> res = GameState.generate_moves();
    for(const auto& intMove: res){

        if(sourceSquare == get_move_source(intMove) && targetSquare == get_move_target(intMove))
        {
            int promoted_piece = get_move_promoted(intMove);

            if(promoted_piece){
                if((promoted_piece == Q || promoted_piece == q) && move[4]=='q')
                    return intMove;
                if((promoted_piece == B || promoted_piece == b) && move[4]=='b')
                    return intMove;
                if((promoted_piece == N || promoted_piece == n) && move[4]=='n')
                    return intMove;
                if((promoted_piece == R || promoted_piece == r) && move[4]=='r')
                    return intMove;
                continue;
            }

            //return legal move
            return intMove;
        }
    }

    return -1;
}


int parse_position(std::string input){
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    
    std::string token;
    while(std::getline(iss,token,' ')){
        tokens.push_back(token);
    }
    bool fromStartPos ;

    if(tokens[0] != "position"){
        return 0;
    }
    if(tokens[1] == "startpos"){
        GameState.parse_fen(start_position);
        fromStartPos = true;
    }
    else{
        std::string fenSubstring;
        for(int i=2; i<=7; i++){
            fenSubstring+=tokens[i];
            fenSubstring+= " ";
        }
        GameState.parse_fen(fenSubstring);
        fromStartPos = false;
    }

    int moveIndex =(fromStartPos)? 3:9;
    while(moveIndex <tokens.size()){
        int move = parse_move(tokens[moveIndex]);
        if(move == -1){
            moveIndex++;
            continue;
        }
        GameState.make_move(move, all_moves);
        moveIndex++;
    }
    GameState.print_board();
    return 0;
}

void parse_go(std::string input){
    std::vector<std::string> tokens;
    std::istringstream iss(input);
    
    std::string token;
    while(std::getline(iss,token,' ')){
        tokens.push_back(token);
    }

    depth = 3;
    if(tokens[1] == "depth")
        depth = std::stoi(tokens[2]);
        
    std::cout<<"Depth: "<<depth<<std::endl;
    search_position(depth,GameState);
}

void uci_loop()
{
    setbuf(stdin, NULL);
    setbuf(stdout, NULL);

    std::string input;
    std::cout << "id name sharpbasher\n";
    std::cout<< "id author bashergoingcrazy\n";
    std::cout << "uciok\n";

    while (1) {
        // reset user/GUI input (reset input here)

        // make sure output reaches GUI
        fflush(stdout);

        // get user/GUI input
        if (!std::getline(std::cin, input)) {
            continue;
        }

        // make sure input is available
        if (input.empty()) {
            continue;
        }

        // parse UCI "isready" command
        if (input.compare(0, 7, "isready") == 0) {
            std::cout << "readyok\n";
            continue;
        }

        // parse UCI "position" command
        if (input.compare(0, 8, "position") == 0) {
            parse_position(input);
            continue;
        }

        // parse UCI "ucinewgame" command
        if (input.compare(0, 10, "ucinewgame") == 0) {
            parse_position("position startpos");
            continue;
        }

        // parse UCI "go" command
        if (input.compare(0, 2, "go") == 0) {
            parse_go(input);
            continue;
        }

        // parse UCI "quit" command
        if (input.compare(0, 4, "quit") == 0) {
            // exit the loop
            break;
        }

        // parse UCI "uci" command
        if (input.compare(0, 3, "uci") == 0) {
            std::cout << "id name sharpbasher\n";
            std::cout<< "id author bashergoingcrazy\n";
            std::cout<<"uciok\n";
        }
    }
}

