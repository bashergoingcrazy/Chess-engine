#ifndef BITGAMESTATE_H
#define BITGAMESTATE_H

#include "bitboard.h"
#include <algorithm>
#include <string>
#include <vector>
#define empty_board "8/8/8/8/8/8/8/8 w - - "
#define start_position "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1 "
#define tricky_position "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - 0 1 "
#define killer_position "rnbqkb1r/pp1p1pPp/8/2p1pP2/1P1P4/3P3P/P1P1P3/RNBQKBNR w KQkq e6 0 1"
#define cmk_position "r2q1rk1/ppp2ppp/2n1bn2/2b1p3/3pP3/3P1NPP/PPP1NPB1/R1BQ1RK1 b - - 0 9 "

enum {wk=1, wq=2, bk=4, bq=8};

const char ascii_pieces[13] = "PNBRQKpnbrqk";


const std::string square_to_coordinates[64]{
    "a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
    "a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
    "a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
    "a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
    "a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
    "a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
    "a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
    "a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1",
};

enum { P, N, B, R, Q, K, p, n, b, r, q, k};
enum {all_moves, only_captures};

int encode_move(int s, int t, int p, int pp, int c, int d, int e, int ca);
int get_move_source(int move);
int get_move_piece(int move);
int get_move_target(int move);
int get_move_promoted(int move);
int get_move_castle(int move);
int get_move_double(int move);
int get_move_enp(int move);
int get_move_capture(int move);

class BitGameState{
private:
public:
    BitGameState() = default;

    BitGameState& operator=(BitGameState&& other)noexcept{
        if(this!=&other){
            for(int i=0;i<12;i++){
                bitboards[i] = std::move(other.bitboards[i]);
            }
            for(int i=0;i<3;i++){
                occupancies[i] = std::move(other.occupancies[i]);
            }
            side = std::move(other.side);
            enpassant = std::move(other.enpassant);
            castle = std::move(other.castle);
        }
        return *this;
    }


    // WARN remove this later
    void test();
 
    Bitboard bitboards[12];
    Bitboard occupancies[3];
    int side = -1;
    int enpassant = no_sq;
    int castle;

    std::vector<int> generate_moves();
    void print_board();
    int make_move(int move, int move_flag);
    int is_square_attacked(int square, int side);
    void print_attacked_squares(int side);
    void parse_fen(std::string input);
    static void print_move(int move);

};

const int castling_rights[64]{
    7,15,15,15,3,15,15,11,
    15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,15,15,
    15,15,15,15,15,15,15,15,
    13,15,15,15,12,15,15,14,
};

#endif // !BITGAMESTATE_H
