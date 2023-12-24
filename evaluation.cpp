#include "evaluation.h"
#include "bitgamestate.h"
#include "bitboard.h"

int evaluate(BitGameState &bgs){
    int score = 0;

    Bitboard bitboard ;
    
    int piece , square;
    for(int bb_piece = P; bb_piece<=k; bb_piece++){
        bitboard = bgs.bitboards[bb_piece];
        while(bitboard.bits.any()){
            piece = bb_piece;
            square = bitboard.get_lsb_index();

            // Evaluating material weights
            score += material_score[piece];

            //Score positional piece scores
            switch(piece)
            {
                case P: score+=pawn_score[square]; break;
                case B: score+=bishop_score[square]; break;
                case N: score+=knight_score[square]; break;
                case R: score+=rook_score[square]; break;
                case K: score+=king_score[square]; break;

                case p: score-=pawn_score[mirror_score[square]]; break;
                case b: score-=bishop_score[mirror_score[square]]; break;
                case n: score-=knight_score[mirror_score[square]]; break;
                case r: score-=rook_score[mirror_score[square]]; break;
                case k: score-=king_score[mirror_score[square]]; break;
            }


            bitboard.bits.reset(square);
        }
    }
    //return final evaluation based on side
    return (bgs.side==white)?score: -score;
}
