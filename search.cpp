
#include "search.h"
#include "bitgamestate.h"
#include "evaluation.h"
#include "time.h"
#include <algorithm>
#include <iostream>
#include <vector>
unsigned long long nodes = 0ULL;
int ply = 0;
int best_move = 0;

int negamax(int alpha, int beta, int depth, BitGameState& bgs)
{
    // Recusion escape condition
    if(depth == 0 ) 
        return evaluate(bgs);

    // increment nodes count
    nodes++;

    int in_check = bgs.is_square_attacked(((bgs.side==white)?bgs.bitboards[K].get_lsb_index():
                bgs.bitboards[k].get_lsb_index())   ,bgs.side ^1);

    //legal moves counter
    int legal_moves;

    //int best move so far
    int best_sofar;
    int old_alpha = alpha;

    std::vector<int> move_list = bgs.generate_moves();

    for(const auto &move: move_list)
    {
        BitGameState clone;
        clone = std::move(bgs);

        ply++;

        if(!clone.make_move(move,all_moves))
        {
            ply--;
            continue;
        }
        int score = -negamax(-beta, -alpha, depth-1,clone);
        
        ply --;
        // fail-hard beta cutoff
        if(score >= beta)
        {
            // node (move) fails high
            return beta;
        }

        if(score > alpha)
        {
            // PV node (move)
            alpha = score;
            if(ply == 0){
                best_sofar = move;
            }
        }
        
    }

    if(old_alpha != alpha)
        //initialize best move
        best_move = best_sofar;

    // node (move) fails low 
    return alpha;
}


void search_position(int depth, BitGameState& bgs)
{
    //find best move within a given position
    int score = negamax(-50000, 50000, depth, bgs);
    std::cout<<"bestmove ";
    bgs.print_move(best_move);
    std::cout<<std::endl;
    std::cout<<"which was :"<<best_move<<std::endl;
}




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

