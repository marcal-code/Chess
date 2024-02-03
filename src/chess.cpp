
#include "chess.hpp"
#include "piece.hpp"
#include "board.hpp"

#include <cmath>
#include <iostream>

namespace Chess {

bool isSquareOnTheBoard(Square square) {

    return ((square.rank >= 0 && square.rank < 8) &&
            (square.file >= 0 && square.file < 8));
}

Square getKingPos(const Board &board, Piece::Color king_color) {

    Piece king = (king_color == Piece::Color::WHITE) ?
	PIECE::WHITE_KING : PIECE::BLACK_KING;

    Square king_pos;

    for (int rank = 0; rank < 8; rank++) {

        for (int file = 0; file < 8; file++) {

            if (board.getPieceAt({rank, file}) == king) {

                king_pos = {rank, file};
                break;
            }
        }
    }

    return king_pos;
}

bool isPieceInBishopPath(const Board& board, Square from, Square to) {

    int d_rank = (to.rank < from.rank) ? -1 : 1;
    int d_file = (to.file < from.file) ? -1 : 1;

    from.rank += d_rank;
    from.file += d_file;

    while (from != to) {

        // if a piece detected in the path
        if (board.getPieceAt(from) != PIECE::EMPTY_SQUARE) return true;

        // increment towards the to Square
        from.rank += d_rank;
        from.file += d_file;
    }

    return false;  
}

bool isPieceInRookPath(const Board& board, Square from, Square to) {

    // checking horizontally
    if (from.rank == to.rank) {

        int d_file = (to.file < from.file) ? -1 : 1;
    
        from.file += d_file;

        while (from != to) {
	    
            // if there is a piece detected in the path
            if (board.getPieceAt(from) != PIECE::EMPTY_SQUARE) return true;

            // increment towards the to square
            from.file += d_file;
        }
    } 
    

    // checking vertically
    else if (from.file == to.file) {

        int d_rank = (to.rank < from.rank) ? -1 : 1;

        from.rank += d_rank;
	    
        while (from != to) {

            // if there is a piece detected in the path
            if (board.getPieceAt(from) != PIECE::EMPTY_SQUARE) return true;

            // increment towards the to square
            from.rank += d_rank;
        }
    }

    return false;
}

bool isValidMove(const Board &board, Square move_from, Square move_to) {

    // a function to check if a given move is a valid move of the piece and it
    // does not put the self king in danger (in check)

    bool isValid = isValidSquare(board, move_from, move_to);

    bool isLegal = isLegalSquare(board, move_from, move_to);

    return (isValid && isLegal);
}

bool isValidSquare(const Board &board, Square move_from, Square move_to) {

    // is a given square (move_to) a valid square for the piece on move_from square

    Piece piece_to_move = board.getPieceAt(move_from);

    if (!isSquareOnTheBoard(move_from) || !isSquareOnTheBoard(move_to)) {

        return false;
    }

    if ((board.getPieceAt(move_to).color == piece_to_move.color)) {

        return false;
    }

    bool result = false;

    switch (piece_to_move.type) {
    
    case Piece::Type::PAWN:
        result = Pawn::isValidSquare(board, move_from, move_to);
        break; 
    case Piece::Type::KNIGHT:
        result = Knight::isValidSquare(board, move_from, move_to);
        break; 
    case Piece::Type::BISHOP:
        result = Bishop::isValidSquare(board, move_from, move_to);
        break; 
    case Piece::Type::ROOK:
        result = Rook::isValidSquare(board, move_from, move_to);
        break; 
    case Piece::Type::QUEEN:
        result = Queen::isValidSquare(board, move_from, move_to);
        break; 
    case Piece::Type::KING:
        result = King::isValidSquare(board, move_from, move_to);
        break; 
    default:
        std::cerr << "Invalid Piece Type!\n";
        break;
    }

    return result;
}

bool isLegalSquare(const Board &board, Square move_from, Square move_to) {

    // is the given move (move_from - move_to) puts the self king in check

    Board copy_board = board;

    copy_board.movePiece(move_from, move_to);
    Piece::Color player = copy_board.getPieceAt(move_to).color;

    if (isInCheck(copy_board, player)) {

        return false;
    }

    return true;
}

bool Pawn::isValidSquare(const Board &board, Square move_from, Square move_to) {

    Piece piece_to_move = board.getPieceAt(move_from);

    // direction that the Pawn is moving: -1 (bottom to top), 1 (top to bottom)
    int direction = (piece_to_move.color == Piece::Color::WHITE) ? -1 : 1;

    // if the board is flipped then change the direction of Pawn movement
    if (board.isFlipped()) direction = (direction == -1) ? 1 : -1;

    // Forward movement of the Pawn
    // checking validation one square in front of the pawn
    if (move_to.rank == (move_from.rank + direction) && move_to.file == move_from.file) {
        if (board.getPieceAt(move_to) == PIECE::EMPTY_SQUARE) {
            return true;
        }
    }

    // checking validation two square in front of the pawn if the pawn is on the
    // 2nd or the 6th rank
    if (move_from.rank == 1 || move_from.rank == 6) {

        if ((move_to.rank == (move_from.rank + (2 * direction))) &&
            (move_to.file == move_from.file)) {

            // if the pawn is on the starting square and no piece is
            // in front of it, then only allow the douple square push

            if ((board.getPieceAt(move_to) == PIECE::EMPTY_SQUARE) &&
                (board.getPieceAt({move_to.rank - direction, move_to.file}) ==
                 PIECE::EMPTY_SQUARE)) {

                return true;
            }
        }
    }

    // Diagonal captures of the Pawn
    // left diagonal
    if ((move_to.rank == move_from.rank + direction) && 
        (move_to.file == move_from.file - 1)) {

        if ((board.getPieceAt(move_to) != PIECE::EMPTY_SQUARE) && 
            (board.getPieceAt(move_to).color != piece_to_move.color)) {

            return true;
        }
    }

    // right capture
    if ((move_to.rank == move_from.rank + direction) &&
        (move_to.file == move_from.file + 1)) {

        if ((board.getPieceAt(move_to) != PIECE::EMPTY_SQUARE) &&
            (board.getPieceAt(move_to).color != piece_to_move.color)) {

            return true;
        }
    }

    return false;
}

bool Knight::isValidSquare(const Board &board, Square move_from, Square move_to) {

    // all possible square for a knight to move from its position
    int d_rank[] = {-2, -2, -1, -1, 1, 1, 2, 2};
    int d_file[] = {-1, 1, 2, -2, -2, 2, -1, 1};

    // 8 possible squares that a knight can move to
    for (int i = 0; i < 8; i++) {

        if ((move_to.rank == move_from.rank + d_rank[i]) && 
            (move_to.file == move_from.file + d_file[i])) {

            return true;
        }
    }

    return false;
}

bool Bishop::isValidSquare(const Board &board, Square move_from, Square move_to) {

    if  (std::abs(move_from.file - move_to.file) ==
         std::abs(move_from.rank - move_to.rank)) {
        
        if(!isPieceInBishopPath(board, move_from, move_to)) {
            return true;
        }
    }

    return false;
}

bool Rook::isValidSquare(const Board &board, Square move_from, Square move_to) {

    if (move_from.rank == move_to.rank || move_from.file == move_to.file) {

        if(!isPieceInRookPath(board, move_from, move_to)) {
            return true;
        }
    }

    return false;
}

bool Queen::isValidSquare(const Board &board, Square move_from, Square move_to) {

    bool isValidBisop = Bishop::isValidSquare(board, move_from, move_to);
    bool isValidRook = Rook::isValidSquare(board, move_from, move_to);

    if (isValidBisop || isValidRook) {
        return true;
    }

    return false;
}

bool King::isValidSquare(const Board &board, Square move_from, Square move_to) {

    // all possible squares for a king to move from its position
    int d_rank[] = {-1, 0, 1, -1, 1, -1, 0, 1};
    int d_file[] = {-1, -1, -1, 0, 0, 1, 1, 1};

    // 8 possible squares that a king can move to (not considering castling)
    for (int i = 0; i < 8; i++) {

        if ((move_to.rank == move_from.rank + d_rank[i]) && 
            (move_to.file == move_from.file + d_file[i])) {

            return true;
        }
    }

    return false;
}
    
bool isInCheck(const Board &board, Piece::Color player) {

    for (int rank = 0; rank < 8; rank++) {

        for (int file = 0; file < 8; file++) {

            Piece current_piece = board.getPieceAt({rank, file});

            if (current_piece != PIECE::EMPTY_SQUARE) {

                if (current_piece.color != player) {

                    if (isValidSquare(board, {rank, file}, getKingPos(board, player))) {

                        return true;
                    }
                }
            }
        }
    }

    return false;
}

bool isInCheckMate(const Board &board, Piece::Color player) {

    if (!isInCheck(board, player)) {

        return false;
    }

    for (int rank = 0; rank < 8; rank++) {

        for (int file = 0; file < 8; file++) {
            
            Piece piece = board.getPieceAt({rank, file});

            if (piece == PIECE::EMPTY_SQUARE) {

                continue;
            }

            if (piece.color == player) {

                for (int rank2 = 0; rank2 < 8; rank2++) {

                    for (int file2 = 0; file2 < 8; file2++) {

                        if((isValidSquare(board, {rank, file}, {rank2, file2})) &&
                            (isLegalSquare(board, {rank, file}, {rank2, file2}))) {

                            return false;
                        }
                    }
                }
            }
        }
    }

    return true;
}

} // namespace Chess

