#pragma once

#include "board.hpp"
#include "piece.hpp"

#include <cmath>
#include <iostream>

namespace Chess {

    // utility stuff
    bool isSquareOnTheBoard(Square square);
    Square getKingPos(const Board &board, Piece::Color king_color);
    bool isPieceInBishopPath(const Board &board, Square from, Square to);
    bool isPieceInRookPath(const Board &board, Square from, Square to);

    // check and checkmate detection
    bool isInCheck(const Board &board, Piece::Color player);
    bool isInCheckMate(const Board &board, Piece::Color player);

    // to check if a given move (move_from - move_to) is legal by the chess rule
    bool isValidMove(const Board &board, Square move_from, Square move_to);

    // is the given move (move_from - move_to) puts the self king in check
    bool isLegalSquare(const Board &board, Square move_from, Square move_to);

    // is a given square (move_to) a valid square for the piece on move_from square
    bool isValidSquare(const Board &board, Square move_from, Square move_to);

    namespace Pawn {

        bool isValidSquare(const Board &board, Square move_from, Square move_to);
    } // namespace Pawn

    namespace Knight {

        bool isValidSquare(const Board &board, Square move_from, Square move_to);
    } // namespace Knight

    namespace Bishop {

        bool isValidSquare(const Board &board, Square move_from, Square move_to);
    } // namespace Bishop

    namespace Rook {

        bool isValidSquare(const Board &board, Square move_from, Square move_to);
    } // namespace Rook

    namespace Queen {

        bool isValidSquare(const Board &board, Square move_from, Square move_to);
    } // namespace Queen

    namespace King {

        bool isValidSquare(const Board &board, Square move_from, Square move_to);
    } // namespace King

} // namespace Chess
