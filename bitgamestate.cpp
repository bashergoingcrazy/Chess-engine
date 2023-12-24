#include "bitgamestate.h"
#include "attack_tables.h"
#include "bitboard.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

// Encoding for moves
int encode_move(int s, int t, int p, int pp, int c, int d, int e, int ca){
    return  s | (t << 6) | (p << 12) | (pp << 16) | (c << 20) | (d << 21) | (e << 22) | (ca << 23);
}

// Decoding for moves
int get_move_source(int move){
    return move & 0x3f;
} 
int get_move_target(int move){
    return (move & 0xfc0) >> 6;
} 
int get_move_piece(int move){
    return (move & 0xf000) >> 12;
} 
int get_move_promoted(int move){
    return (move & 0xf0000) >> 16;
} 
int get_move_capture(int move){
    return (move & 0x100000) >> 20;
} 
int get_move_double(int move){
    return (move & 0x200000) >> 21;
} 
int get_move_enp(int move){
    return (move & 0x400000) >> 22;
} 
int get_move_castle(int move){
    return (move & 0x800000) >> 23;
} 

void BitGameState::print_move(int move){
    char promotedPiece ;
    char lowercasePromotedPiece;
    if(get_move_promoted(move)){
        promotedPiece = ascii_pieces[get_move_promoted(move)];
        lowercasePromotedPiece = std::tolower(promotedPiece);
    }
    std::cout<<square_to_coordinates[get_move_source(move)]
        <<square_to_coordinates[get_move_target(move)]
        <<((get_move_promoted(move)==0)? ' ': lowercasePromotedPiece);
}

void BitGameState::test(){

}

void BitGameState::print_board(){
    std::cout<<std::endl;

    for(int rank = 0 ; rank<8 ;rank++){
        for(int file = 0; file<8; file++){
            int square = rank*8+file;
            if(!file)
                std::cout<<8-rank<<"  ";
           
            int piece = -1;

            for(int bb_piece = P; bb_piece <=k ;bb_piece++){
                if (bitboards[bb_piece].bits[square]){
                    piece = bb_piece;
                }
            }
            
            std::cout<<((piece==-1)?'.':ascii_pieces[piece]);
            std::cout<<' ';

        }
        std::cout<<std::endl;
    }
    std::cout<<"\n   a b c d e f g h"<<std::endl;
    std::cout<<std::endl;
    std::cout<<"     Side: "<<((side==white)?"White":"Black")<<std::endl;
    std::cout<<"  Enpassant: "<<((enpassant!=no_sq)? square_to_coordinates[enpassant]: "No")<<std::endl;
    std::cout<<"   Castling :"<<((castle & wk)? "K":"-")
                            <<((castle & wq)? "Q":"-")
                            <<((castle & bk)? "k":"-")
                            <<((castle & bq)? "q":"-")<<std::endl;

    std::cout<<std::endl;
}

void BitGameState::parse_fen(std::string input){
    //Initialize the current game state as nothing 
    for(int i=P; i<=k;i++){
        bitboards[i] = Bitboard();
    }
    for(int i=0; i<=2;i++){
        occupancies[i] = Bitboard();
    }

    side = -1;
    castle = -1;
    enpassant = no_sq;

    std::istringstream fenStream(input);
    std::string token;
    fenStream >> token;

    int rank = 0; 
    int file = 0;

    for(char c: token){
        if(c == '/'){
            // Move to the next rank
            rank++;
            file = 0;
        } else if(isdigit(c)){
            //Skip empty squares
            file += (c - '0');
        } else{
            //Place the piece on the board
            int pieceIndex = -1;

            switch(c){
                case 'P':pieceIndex = 0; break;
                case 'N':pieceIndex =1; break;
                case 'B': pieceIndex = 2; break;
                case 'R': pieceIndex = 3; break;
                case 'Q': pieceIndex = 4; break;
                case 'K': pieceIndex = 5; break;
                case 'p': pieceIndex = 6; break;
                case 'n': pieceIndex = 7; break;
                case 'b': pieceIndex = 8; break;
                case 'r': pieceIndex = 9; break;
                case 'q': pieceIndex = 10; break;
                case 'k': pieceIndex = 11; break;
            }
            if(pieceIndex !=-1){
                bitboards[pieceIndex].bits.set(rank*8 + file);
            }
            file++;
        }
    }
    
    for(int index = P; index<=K; index++)
        occupancies[white].bits |= bitboards[index].bits;

    for(int index=p; index<=k; index++)
        occupancies[black].bits |= bitboards[index].bits;

    occupancies[both].bits = occupancies[white].bits | occupancies[black].bits;

    fenStream >> token;
    
    if(token == "w") side = white;
    else side = black; 

    fenStream >> token;
    castle = 0;

    if(token != "-"){
        for(char c : token){
            switch (c) {
                case 'K': castle |= wk; break;
                case 'Q': castle |= wq; break;
                case 'k': castle |= bk; break;
                case 'q': castle |= bq; break;
                case '-': break;
            }
        }
    }  

    fenStream >> token;
    if(token != "-"){
        char fileChar = token[0];
        char rankChar = token[1];

        int file = fileChar - 'a';
        int rank =  '8' - rankChar;
        enpassant = rank*8 + file;
    }
}

void BitGameState::print_attacked_squares(int side){
    std::cout<<std::endl;
    for(int rank=0 ;rank<8;rank++){
        for(int file=0; file<8 ;file++){
            int square = rank*8+file;

            if(!file)
                std::cout<<8-rank<<" ";
            std::cout<<" "<<is_square_attacked(square,side);
        }
        std::cout<<std::endl;
    }
    std::cout<<"\n   a b c d e f g h"<<std::endl;
}

int BitGameState::is_square_attacked(int square, int side){
    //Attacked by Pawns
    if((side==white) && (atk.pawn_attacks[black][square].bits & bitboards[P].bits).any()) return 1;
    if((side==black) && (atk.pawn_attacks[white][square].bits & bitboards[p].bits).any()) return 1;

    // Attacked by Knight 
    if((atk.knight_attacks[square].bits & ((side==white)? bitboards[N].bits : bitboards[n].bits)).any()) return 1;
    
    // Attacked by King
    if((atk.king_attacks[square].bits & ((side==white)? bitboards[K].bits : bitboards[k].bits)).any()) return 1;

    // Attacked by Bishops
    if((atk.get_bishop_attacks(square,occupancies[both]).bits & ((side==white)? bitboards[B].bits:bitboards[b].bits)).any()) return 1;

    // Attacked by Rooks 
    if((atk.get_rook_attacks(square,occupancies[both]).bits & ((side==white)? bitboards[R].bits:bitboards[r].bits)).any()) return 1;

    // Attacked by Queen
    if((atk.get_queen_attacks(square,occupancies[both]).bits & ((side==white)? bitboards[Q].bits:bitboards[q].bits)).any()) return 1;

    return 0;
}


std::vector<int> BitGameState::generate_moves(){
    std::vector<int> res;

    int source_square, target_square;

    Bitboard piece, attacks;

    for(int index = P; index <=k; index++){
        piece = bitboards[index];

        // Generate white pawns and white king castling moves
        if(side == white){
            if(index == P){
                while(piece.bits.any()){
                    // init source square 
                    source_square = piece.get_lsb_index();

                    // init target square
                    target_square = source_square - 8;

                    // generate quite pawn moves
                    if(target_square >=a8 && !occupancies[both].bits[target_square]){
                        //pawn promotison
                        if(source_square>=a7 && source_square<=h7){
                            // Add move into a move list
                            res.push_back(encode_move(source_square,target_square,index,Q,0,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,R,0,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,B,0,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,N,0,0,0,0));
                        }else{
                            // One Square pushes
                            res.push_back(encode_move(source_square,target_square,index,0,0,0,0,0));

                            // Two Square push
                            if(source_square>=a2 && source_square <= h2 && !occupancies[both].bits[target_square-8]){
                                res.push_back(encode_move(source_square,target_square-8,index,0,0,1,0,0));
                            }
                        }
                    }
                    
                    attacks.bits = (atk.pawn_attacks[side][source_square].bits & occupancies[black].bits);
                    
                    //generate pawn captures
                    while(attacks.bits.any()){
                        // init target square
                        target_square = attacks.get_lsb_index();

                        if(source_square>=a7 && source_square<=h7){
                            // Add move into a move list
                            res.push_back(encode_move(source_square,target_square,index,Q,1,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,R,1,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,B,1,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,N,1,0,0,0));

                        }else{
                            // One Square pushes
                            res.push_back(encode_move(source_square,target_square,index,0,1,0,0,0));
                        }

                        // reset attacks lsb bit
                        attacks.bits.reset(target_square);
                    }

                    if(enpassant != no_sq){
                        Bitboard enp_attacks(atk.pawn_attacks[white][source_square].bits.to_ullong() & (1UL << enpassant));
                        if(enp_attacks.bits.any()){
                            // init enpassant capture target square
                            int target_enp = enp_attacks.get_lsb_index();
                            res.push_back(encode_move(source_square,target_enp,index,0,1,0,1,0));
                        }
                    }
                    
                    // reset piece lsb bit
                    piece.bits.reset(source_square);
                }
            }
            // castling moves
            if(index == K){
                // King side castling 
                if(castle & wk){
                    if(!occupancies[both].bits[g1] && !occupancies[both].bits[f1]){
                        if(!is_square_attacked(e1,black) && !is_square_attacked(f1,black)){
                            res.push_back(encode_move(e1,g1,index,0,0,0,0,1));
                        }
                    }
                }
                
                // Queen side ccastling
                if(castle & wq){
                    if(!occupancies[both].bits[d1] && !occupancies[both].bits[c1] && !occupancies[both].bits[b1]){
                        if(!is_square_attacked(e1,black) && !is_square_attacked(d1,black) )
                            res.push_back(encode_move(e1,c1,index,0,0,0,0,1));
                    }
                }
            }
        }
        else{
            if(index == p){
                while(piece.bits.any()){
                    // init source square 
                    source_square = piece.get_lsb_index();

                    // init target square
                    target_square = source_square + 8;

                    // generate quite pawn moves
                    if(target_square <= h1 && !occupancies[both].bits[target_square]){
                        //pawn promotison
                        if(source_square<=h2 && source_square>=a2){
                            // Add move into a move list
                            res.push_back(encode_move(source_square,target_square,index,q,0,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,r,0,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,b,0,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,n,0,0,0,0));


                        }else{
                            // One Square pushes
                            res.push_back(encode_move(source_square,target_square,index,0,0,0,0,0));

                            // Two Square push
                            if(source_square>=a7 && source_square <= h7 && !occupancies[both].bits[target_square+8]){
                                res.push_back(encode_move(source_square,target_square+8,index,0,0,1,0,0));
                            }
                        }
                    }

                    attacks.bits = (atk.pawn_attacks[side][source_square].bits & occupancies[white].bits);
                    
                    //generate pawn captures
                    while(attacks.bits.any()){
                        // init target square
                        target_square = attacks.get_lsb_index();

                        if(source_square>=a2 && source_square<=h2){
                            // Add move into a move list
                            res.push_back(encode_move(source_square,target_square,index,q,1,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,r,1,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,b,1,0,0,0));
                            res.push_back(encode_move(source_square,target_square,index,n,1,0,0,0));

                        }else{
                            // One Square pushes
                            res.push_back(encode_move(source_square,target_square,index,0,1,0,0,0));
                        }

                        // reset attacks lsb bit
                        attacks.bits.reset(target_square);
                    }

                    if(enpassant != no_sq){
                        Bitboard enp_attacks(atk.pawn_attacks[side][source_square].bits.to_ullong() & (1UL << enpassant));
                        if(enp_attacks.bits.any()){
                            // init enpassant capture target square
                            int target_enp = enp_attacks.get_lsb_index();
                            res.push_back(encode_move(source_square,target_enp,index,0,1,0,1,0));
                        }
                    }
                    // reset piece lsb bit
                    piece.bits.reset(source_square);
                }
            }
            // castling moves
            if(index == k){
                // King side castling 
                if(castle & bk){
                    if(!occupancies[both].bits[g8] && !occupancies[both].bits[f8]){
                        if(!is_square_attacked(e8,white) && !is_square_attacked(f8,white)){
                            res.push_back(encode_move(e8,g8,index,0,0,0,0,1));
                        }
                    }
                }
                
                // Queen side ccastling
                if(castle & bq){
                    // Queen side castling 
                    if(!occupancies[both].bits[d8] && !occupancies[both].bits[c8] && !occupancies[both].bits[b8]){
                        if(!is_square_attacked(e8,white) && !is_square_attacked(d8,white))
                            res.push_back(encode_move(e8,c8,index,0,0,0,0,1));
                    }
                }
            }
        }

        //generate knight moves
        if((side==white)? index == N : index == n){
            while(piece.bits.any()){
                source_square = piece.get_lsb_index();

                attacks.bits = (atk.knight_attacks[source_square].bits &
                        ((side == white)?~occupancies[white].bits : ~occupancies[black].bits));

                while(attacks.bits.any()){
                    target_square = attacks.get_lsb_index();
                    
                    //quite move
                    if(!occupancies[((side==white)? black:white)].bits[target_square])
                         res.push_back(encode_move(source_square,target_square,index,0,0,0,0,0));
                    //capture move
                    else
                        res.push_back(encode_move(source_square,target_square,index,0,1,0,0,0));

                    attacks.bits.reset(target_square);
                }

                piece.bits.reset(source_square);
            }
        }
        

        //generate bishop moves
        if((side==white)? index == B : index == b){
            while(piece.bits.any()){
                source_square = piece.get_lsb_index();

                attacks.bits = (atk.get_bishop_attacks(source_square,occupancies[both]).bits &
                        ((side == white)?~occupancies[white].bits : ~occupancies[black].bits));

                while(attacks.bits.any()){
                    target_square = attacks.get_lsb_index();
                    
                    //quite move
                    if(!occupancies[((side==white)? black:white)].bits[target_square])
                         res.push_back(encode_move(source_square,target_square,index,0,0,0,0,0));
                    //capture move
                    else
                        res.push_back(encode_move(source_square,target_square,index,0,1,0,0,0));

                    attacks.bits.reset(target_square);
                }

                piece.bits.reset(source_square);
            }
        }
        //generate rook moves
        if((side==white)? index == R : index == r){
            while(piece.bits.any()){
                source_square = piece.get_lsb_index();

                // int encode_move(int s, int t, int p, int pp, int c, int d, int e, int ca){
                attacks.bits = (atk.get_rook_attacks(source_square,occupancies[both]).bits &
                        ((side == white)?~occupancies[white].bits : ~occupancies[black].bits));

                while(attacks.bits.any()){
                    target_square = attacks.get_lsb_index();
                    
                    //quite move
                    if(!occupancies[((side==white)? black:white)].bits[target_square])
                        res.push_back(encode_move(source_square,target_square,index,0,0,0,0,0));
                    //capture move
                    else
                        res.push_back(encode_move(source_square,target_square,index,0,1,0,0,0));

                    attacks.bits.reset(target_square);
                }

                piece.bits.reset(source_square);
            }
        }
        //generate queen moves
        if((side==white)? index == Q : index == q){
            while(piece.bits.any()){
                source_square = piece.get_lsb_index();

                attacks.bits = (atk.get_queen_attacks(source_square,occupancies[both]).bits &
                        ((side == white)?~occupancies[white].bits : ~occupancies[black].bits));

                while(attacks.bits.any()){
                    target_square = attacks.get_lsb_index();
                    
                    //quite move
                    if(!occupancies[((side==white)? black:white)].bits[target_square])
                        res.push_back(encode_move(source_square,target_square,index,0,0,0,0,0));
                    //capture move
                    else
                        res.push_back(encode_move(source_square,target_square,index,0,1,0,0,0));

                    attacks.bits.reset(target_square);
                }

                piece.bits.reset(source_square);
            }
        }
        //generate king moves
        if((side==white)? index == K : index == k){
            while(piece.bits.any()){
                source_square = piece.get_lsb_index();

                attacks.bits = (atk.king_attacks[source_square].bits &
                        ((side == white)?~occupancies[white].bits : ~occupancies[black].bits));

                while(attacks.bits.any()){
                    target_square = attacks.get_lsb_index();
                    
                    //quite move
                    if(!occupancies[((side==white)? black:white)].bits[target_square])
                        res.push_back(encode_move(source_square,target_square,index,0,0,0,0,0));
                    //capture move
                    else
                        res.push_back(encode_move(source_square,target_square,index,0,1,0,0,0));

                    attacks.bits.reset(target_square);
                }

                piece.bits.reset(source_square);
            }
        }
    }


    return res;
}

int BitGameState::make_move(int move, int move_flag){
    if(move_flag == all_moves){
        int source_square = get_move_source(move);
        int target_square = get_move_target(move);
        int piece = get_move_piece(move);
        int promoted = get_move_promoted(move);
        int capture = get_move_capture(move);
        int doublePawn = get_move_double(move);
        int enpass = get_move_enp(move);
        int castling = get_move_castle(move);

        // Move piece
        bitboards[piece].bits.set(target_square);
        bitboards[piece].bits.reset(source_square);

        // Handling Captures
        if(capture){
            int start_piece,end_piece;
            if(side == white){
                start_piece = p;
                end_piece = k;
            }
            else{
                start_piece = P;
                end_piece = K;
            }
            for(int bb_piece = start_piece ; bb_piece<=end_piece; bb_piece++){
                if(bitboards[bb_piece].bits[target_square])
                    bitboards[bb_piece].bits.reset(target_square);
            }
        }

        // Handling promotion
        if(promoted){
            bitboards[piece].bits.reset(target_square);
            bitboards[promoted].bits.set(target_square);
        }

        // Handling Enpassant Captures
        if(enpass){
            bitboards[((side==white)?p:P)].bits.reset(((side==white)? target_square + 8: target_square-8));
        }
        enpassant = no_sq;
        if(doublePawn){
            enpassant = ((side==white)? target_square+8 : target_square-8);
        }

        if(castling){
            switch (target_square) {
                case(g1):
                    bitboards[R].bits.reset(h1);
                    bitboards[R].bits.set(f1);
                    break;
                case(c1):
                    bitboards[R].bits.reset(a1);
                    bitboards[R].bits.set(d1);
                    break;
                case(g8):
                    bitboards[r].bits.reset(h8);
                    bitboards[r].bits.set(f8);
                    break;
                case(c8):
                    bitboards[r].bits.reset(a8);
                    bitboards[r].bits.set(d8);
                    break;

            }
        }

        // Update castling rights
        castle &= castling_rights[source_square];
        castle &= castling_rights[target_square];

        // Update Occupancies
        Bitboard occupnacyWhite ;
        for(int index = P; index<=K; index++){
            occupnacyWhite.bits |=bitboards[index].bits;
        }
        Bitboard occupnacyBlack ;
        for(int index = p; index<=k; index++){
            occupnacyBlack.bits |=bitboards[index].bits;
        }

        occupancies[white] = occupnacyWhite;
        occupancies[black] = occupnacyBlack;
        occupancies[both].bits = occupancies[white].bits | occupancies[black].bits;

        // Change side
        side ^=1;

        if(is_square_attacked((side==white)? bitboards[k].get_lsb_index() : bitboards[K].get_lsb_index(),side))
            // Move is illegal
            return 0;
        else 
            // Return legal move
            return 1;

        return 0;
    }
    else{
        // make sure moves it the capture
        if(get_move_capture(move)){
            return make_move(move, all_moves);
        }
        //other wise move is not a capture
        else return 0;
    }
    
}
