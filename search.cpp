
#include "search.h"
#include "bitgamestate.h"
#include "time.h"
#include <iostream>
#include <vector>
unsigned long long nodes = 0ULL;

void perftTest(int depth,BitGameState& bgs){
    if (depth == 0){
        nodes++;
        return;
    }
    std::vector<int> moves  = bgs.generate_moves();
    for(int move_count = 0 ; move_count <moves.size(); move_count++ ){
        int move = moves[move_count];
        BitGameState cl ;
        cl = std::move(bgs);
        if(!cl.make_move(move,all_moves))
            continue;
        
        // call perft driver recursively
        perftTest(depth-1, cl);

        // bgs.print_board(); 
        // std::cin.ignore();
        // cl.print_board();
        // std::cin.ignore();
        
    }

}

void mainPerft(int depth, BitGameState& bgs){
    std::cout<<"Performance test "<<std::endl;
    std::vector<int> moves = bgs.generate_moves();

    const auto start = getCurrentTime();
    
    for(const auto& move:moves){
        BitGameState cl;
        cl = std::move(bgs);
        if(!cl.make_move(move, all_moves))
            continue;
        unsigned long long commulative_nodes= nodes;

        perftTest(depth -1, cl);

        unsigned long long old_nodes = nodes- commulative_nodes;

        //print moves
        std::cout<<"   move: ";
        cl.print_move(move);
        std::cout<<" nodes: "<<old_nodes<<std::endl;;
    }

    const auto end = getCurrentTime();
    std::cout<<"\n    Depth: "<<depth<<std::endl;
    std::cout<<"   Nodes: "<<nodes<<std::endl;
    std::cout<<"   Time: "<<calculateDurationMili(start,end).count()<<"ms"<<std::endl;

}

unsigned long long getNodeCount(){
    return nodes;
}

